#pragma once

#include <vector>
#include <iomanip>

class Process
{
private:
    int arrivalTime;
    unsigned int size;
    unsigned int nextEventIndex;
    bool complete;

    struct Statistics
    {
        int arrivalTime;
        int startTime;
        int finishTime;
        int serviceTime;
        int turnaroundTime;
        double normalized_turnaroundTime;
        int responseTimeTotal;
        int responseLast;
        int responseCount;
        bool ready = false;
        int waitCount = 0;
        double avg_responseTime;
    };

    std::vector<std::vector<int>> processEventList;

public:
    int id;
    Statistics stats;

    Process();
    Process(int i, int time);
    ~Process();

    void initializeStat();

    void setArrivalTime(int time);
    void setNextTime(int time);
    void addEvent(bool cpu, int time);

    void calculateStats();

    int getNextEvent();
    int getNextTime();
    int getId();
    void updateEventIndex();

    void print(std::string filename);
};

void Process::initializeStat()
{
    stats.startTime = -1;
    stats.finishTime = -1;
    stats.serviceTime = 0;
    stats.turnaroundTime = -1;
    stats.normalized_turnaroundTime = -1;
    stats.responseTimeTotal = -1;
    stats.responseLast = -1;
    stats.responseCount = 0;
    stats.avg_responseTime = -1;
}

Process::Process()
{
    arrivalTime = 0;
    nextEventIndex = 0;
    size = 0;
    complete = false;
    initializeStat();
}

Process::Process(int i, int time)
{
    arrivalTime = time;
    stats.arrivalTime = time;
    nextEventIndex = 0;
    size = 0;
    id = i;
    complete = false;
    initializeStat();
}

Process::~Process()
{
}

void Process::setArrivalTime(int time)
{
    arrivalTime = time;
    stats.arrivalTime = time;
}

void Process::setNextTime(int time)
{
    processEventList[nextEventIndex][1] = time;
}

void Process::addEvent(bool cpu, int time)
{
    std::vector<int> eventData;
    if (cpu)
    {
        eventData.push_back(0);
    }
    else
    {
        eventData.push_back(1);
    }

    eventData.push_back(time);
    processEventList.push_back(eventData);

    size++;
}

int Process::getNextEvent()
{
    if (complete)
    {
        return -1;
    }
    return processEventList[nextEventIndex][0];
}

int Process::getNextTime()
{
    if (complete)
    {
        return -1;
    }
    int t = processEventList[nextEventIndex][1];
    return t;
}

int Process::getId()
{
    return id;
}

void Process::updateEventIndex()
{
    nextEventIndex++;

    if (nextEventIndex >= (size - 1))
    {
        complete = true;
    }
}

void Process::calculateStats()
{
    stats.turnaroundTime = stats.finishTime - stats.arrivalTime;
    stats.normalized_turnaroundTime = (double)stats.turnaroundTime / (double)stats.serviceTime;
    stats.avg_responseTime = (double)stats.responseTimeTotal / (double)stats.responseCount;
}

void Process::print(std::string filename)
{
    std::ofstream outFile;
    outFile.open(filename, std::ios::app);
    if (outFile.is_open())
    {
        outFile << std::fixed << std::setprecision(2);
        outFile << "Process " << id << ":" << std::endl;
        outFile << "                Arrival Time: " << stats.arrivalTime << std::endl;
        outFile << "                  Start Time: " << stats.startTime << std::endl;
        outFile << "                 Finish Time: " << stats.finishTime << std::endl;
        outFile << "                Service Time: " << stats.serviceTime << std::endl;
        outFile << "             Turnaround Time: " << stats.turnaroundTime << std::endl;
        outFile << "  Normalized Turnaround Time: " << stats.normalized_turnaroundTime << std::endl;
        outFile << "       Average Response Time: " << stats.avg_responseTime << std::endl;
        outFile << "---------------------------------------------" << std::endl;
        outFile.close();
    }
}