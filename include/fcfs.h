#pragma once

#include "event.h"

class FCFS
{
private:
    std::queue<int> readyQueue;
    std::vector<Process> processList;
    EventHandler handler;

    unsigned int processNumber = 0;
    unsigned int processExitCount = 0;

    void print();

public:
    FCFS(std::string filename);
    ~FCFS();
};

FCFS::FCFS(std::string filename)
{
    processExitCount = 0;

    handler.readProcessFile(processList, filename);

    processNumber = processList.size();
    while (processExitCount < processNumber)
    {
        EventInfo event = handler.getNextElement();
        int processId = event.process.id;
        int runtime = processList.at(processId).getNextTime();
        if (event.action == Event::ARRIVE)
        {
            readyQueue.push(processId);

            // Statistics
            processList.at(processId).stats.responseCount++;
            processList.at(processId).stats.responseLast = handler.getTime();
        }
        else if (event.action == Event::BLOCK)
        {
            EventInfo unblock;
            unblock.action = Event::UNBLOCK;
            unblock.process = processList.at(processId);
            unblock.timestamp = (handler.getTime() + runtime);
            handler.addEvent(unblock);

            handler.cpuIdle = true;
        }
        else if (event.action == Event::UNBLOCK)
        {
            processList.at(processId).updateEventIndex();
            readyQueue.push(processId);

            processList.at(processId).stats.responseCount++;
            processList.at(processId).stats.responseLast = handler.getTime();
        }
        else if (event.action == Event::EXIT)
        {
            processExitCount++;
            handler.cpuIdle = true;
            processList.at(processId).stats.finishTime = handler.getTime();

            // Process statistics
            processList.at(processId).calculateStats();
        }

        if (handler.is_Idle() && !readyQueue.empty())
        {
            processId = readyQueue.front();
            readyQueue.pop();

            if (processList.at(processId).stats.startTime == -1)
            {
                processList.at(processId).stats.startTime = handler.getTime();
            }

            handler.cpuIdle = false;
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

FCFS::~FCFS()
{
}

void FCFS::print()
{
    double totalTurnaroundTime = 0;
    double totalNormTurnaroundTime = 0;
    double totalavg_responseTime = 0;
    for (unsigned int i = 0; i < processList.size(); i++)
    {
        processList.at(i).print("../out/fcfs.out");
        totalTurnaroundTime += processList.at(i).stats.turnaroundTime;
        totalNormTurnaroundTime += processList.at(i).stats.normalized_turnaroundTime;
        totalavg_responseTime += processList.at(i).stats.avg_responseTime;
    }

    double meanT = totalTurnaroundTime / (double)processList.size();
    double meanNT = totalNormTurnaroundTime / (double)processList.size();
    double meanAvg = totalavg_responseTime / (double)processList.size();

    std::ofstream outFile;
    outFile.open("../out/fcfs.out", std::ios::app);
    if (outFile.is_open())
    {
        outFile << std::fixed << std::setprecision(2);
        outFile << "           Mean Turnaround Time: " << meanT << std::endl;
        outFile << "Mean Normalized Turnaround Time: " << meanNT << std::endl;
        outFile << "     Mean Average Response Time: " << meanAvg << std::endl;
        outFile.close();
    }
}