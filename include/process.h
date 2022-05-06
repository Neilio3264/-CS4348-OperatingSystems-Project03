#pragma once

#include <vector>

class Process
{
private:
    int arrivalTime;
    int nextEventIndex;

    std::vector<std::vector<int>> processEventList;

public:
    Process();
    Process(int time);
    ~Process();

    void setArrivalTime(int time);
    void setNextTime(int time);
    void addEvent(bool cpu, int time);

    int getNextEvent();
    int getNextTime();
    void updateEventIndex();
};

Process::Process()
{
    arrivalTime = 0;
    nextEventIndex = 0;
}

Process::Process(int time)
{
    arrivalTime = time;
    nextEventIndex = 0;
}

Process::~Process()
{
}

void Process::setArrivalTime(int time)
{
    arrivalTime = time;
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
}

int Process::getNextEvent()
{
    return processEventList[nextEventIndex][0];
}

int Process::getNextTime()
{
    int t = processEventList[nextEventIndex][1];
    return t;
}

void Process::updateEventIndex()
{
    nextEventIndex++;
}