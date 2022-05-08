#pragma once

#include <algorithm>
#include "event.h"

class SRT
{
private:
    std::vector<int> readyQueue;
    std::vector<Process> processList;
    EventHandler handler;

    unsigned int processNumber = 0;
    unsigned int processExitCount = 0;
    bool service = false;
    float alpha = 0;

    int processRunning = -1;
    std::vector<int> previousProcess;
    bool interrupted = false;
    bool skip = false;

    void print();
    int calcNext();

public:
    SRT(std::string filename, bool given, float a);
    ~SRT();
};

SRT::SRT(std::string filename, bool given, float a)
{
    service = given;
    alpha = a;
    processExitCount = 0;

    handler.readProcessFile(processList, filename);

    for (size_t i = 0; i < processList.size(); i++)
    {
        processList.at(i).initS0();
    }

    processNumber = processList.size();
    while (processExitCount < processNumber)
    {
        EventInfo event = handler.getNextElement();
        int processId = event.process.id;
        int runtime = processList.at(processId).getNextTime();
        if (event.action == Event::ARRIVE)
        {
            readyQueue.push_back(processId);

            // Statistics
            processList.at(processId).stats.responseCount++;
            processList.at(processId).stats.responseLast = handler.getTime();

            handler.cpuIdle = true;
            interrupted = true;
        }
        else if (event.action == Event::BLOCK)
        {
            std::vector<int>::iterator it;
            it = std::find(previousProcess.begin(), previousProcess.end(), processId);
            if (it == previousProcess.end())
            {
                EventInfo unblock;
                unblock.action = Event::UNBLOCK;
                unblock.process = processList.at(processId);
                unblock.timestamp = (handler.getTime() + runtime);
                handler.addEvent(unblock);

                handler.cpuIdle = true;
                processRunning = -1;
            }
            else
            {
                previousProcess.erase(it);
            }
        }
        else if (event.action == Event::UNBLOCK)
        {
            processList.at(processId).updateEventIndex();
            readyQueue.push_back(processId);

            processList.at(processId).stats.responseCount++;
            processList.at(processId).stats.responseLast = handler.getTime();
        }
        else if (event.action == Event::EXIT)
        {
            processExitCount++;
            handler.cpuIdle = true;
            processRunning = -1;
            processList.at(processId).stats.finishTime = handler.getTime();

            // Process statistics
            processList.at(processId).calculateStats();
        }

        if (interrupted && processRunning != -1)
        {
            processList.at(processRunning).nextEventIndex--;
            int duration = processList.at(processRunning).getNextTime();
            if (duration == (handler.getTime() - processList.at(processRunning).clockStart))
            {
                processList.at(processRunning).nextEventIndex++;
            }
            else if (duration > (handler.getTime() - processList.at(processRunning).clockStart))
            {
                processList.at(processRunning).setNextTime(duration - (handler.getTime() - processList.at(processRunning).clockStart));
                previousProcess.push_back(processRunning);
                readyQueue.push_back(processId);

                handler.cpuIdle = true;
                processRunning = -1;
                interrupted = false;

                processList.at(processRunning).stats.responseCount++;
                processList.at(processRunning).stats.responseLast = handler.getTime();
            }
        }

        if (handler.is_Idle() && !readyQueue.empty())
        {
            // Calculate whihc
            processId = calcNext();

            if (processList.at(processId).stats.startTime == -1)
            {
                processList.at(processId).stats.startTime = handler.getTime();
            }

            handler.cpuIdle = false;
            processRunning = processId;
            processList.at(processId).clockStart = handler.getTime();
            processList.at(processId).stats.responseTimeTotal += (handler.getTime() - processList.at(processId).stats.responseLast);
            processList.at(processId).stats.responseLast = -1;

            runtime = processList.at(processId).getNextTime();

            processList.at(processId).stats.serviceTime += runtime;

            processList.at(processId).updateEventIndex();
            int event = processList.at(processId).getNextEvent();

            if (event == 1)
            {
                EventInfo block;
                block.action = Event::BLOCK;
                block.process = processList.at(processId);
                block.timestamp = (handler.getTime() + runtime);
                handler.addEvent(block);
            }
            else if (event == -1)
            {
                EventInfo exit;
                exit.action = Event::EXIT;
                exit.process = processList.at(processId);
                exit.timestamp = (handler.getTime() + runtime);
                handler.addEvent(exit);
            }
        }
    }

    print();
}

SRT::~SRT()
{
}

void SRT::print()
{
    double totalTurnaroundTime = 0;
    double totalNormTurnaroundTime = 0;
    double totalavg_responseTime = 0;
    for (unsigned int i = 0; i < processList.size(); i++)
    {
        processList.at(i).print("../out/srt.out");
        totalTurnaroundTime += processList.at(i).stats.turnaroundTime;
        totalNormTurnaroundTime += processList.at(i).stats.normalized_turnaroundTime;
        totalavg_responseTime += processList.at(i).stats.avg_responseTime;
    }

    double meanT = totalTurnaroundTime / (double)processList.size();
    double meanNT = totalNormTurnaroundTime / (double)processList.size();
    double meanAvg = totalavg_responseTime / (double)processList.size();

    std::ofstream outFile;
    outFile.open("../out/srt.out", std::ios::app);
    if (outFile.is_open())
    {
        outFile << std::fixed << std::setprecision(2);
        outFile << "           Mean Turnaround Time: " << meanT << std::endl;
        outFile << "Mean Normalized Turnaround Time: " << meanNT << std::endl;
        outFile << "     Mean Average Response Time: " << meanAvg << std::endl;
        outFile.close();
    }
}

int SRT::calcNext()
{
    if (service)
    {
        int indexRemove = 0;
        int min = 100000;
        for (unsigned int i = 0; i < readyQueue.size(); i++)
        {
            int id = readyQueue.at(i);
            int temp = processList.at(id).remainingTotal;
            if (temp < min)
            {
                min = temp;
                indexRemove = i;
            }
        }

        int processId = readyQueue.at(indexRemove);
        readyQueue.erase(readyQueue.begin() + indexRemove);
        return processId;
    }
    else
    {
        int indexRemove = 0;
        int min = 100000;
        for (unsigned int i = 0; i < readyQueue.size(); i++)
        {
            int id = readyQueue.at(i);
            int temp;
            if (processList.at(id).first)
            {
                temp = processList.at(id).lastPredicted;
                processList.at(id).first = false;
            }
            else
            {
                temp = ((alpha * processList.at(id).getNextTime()) + ((1 - alpha) * processList.at(id).lastPredicted));
            }

            if (temp < min)
            {
                min = temp;
                indexRemove = i;
            }
        }

        int processId = readyQueue.at(indexRemove);
        readyQueue.erase(readyQueue.begin() + indexRemove);
        processList.at(processId).lastPredicted = min;
        return processId;
    }
}