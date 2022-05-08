#pragma once

#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "process.h"

enum Event
{
    BLOCK,
    EXIT,
    ARRIVE,
    UNBLOCK,
    TIMEOUT
};

struct EventInfo
{
    int action = 0;
    Process process;
    int timestamp = 0;
};

struct CompareEvent
{
    bool operator()(EventInfo const &e1, EventInfo const &e2)
    {
        return e1.timestamp > e2.timestamp;
    }
};

class EventHandler
{
private:
    int clock;

public:
    bool cpuIdle;
    std::priority_queue<EventInfo, std::vector<EventInfo>, CompareEvent> Q;
    EventHandler();
    ~EventHandler();

    void readProcessFile(std::vector<Process> &processes, std::string file);

    void addEvent(EventInfo event);
    void updateIdle();
    void showpq();
    int getTime();
    bool is_Idle();

    EventInfo getNextElement();
};

EventHandler::EventHandler()
{
    cpuIdle = true;
    clock = 0;
}

EventHandler::~EventHandler()
{
}

void EventHandler::readProcessFile(std::vector<Process> &processes, std::string file)
{
    std::ifstream inputStream(file);
    std::string line = "";
    int processId = 0;
    while (std::getline(inputStream, line))
    {
        std::istringstream iss(line);

        int arrivalTime;
        std::string eventName;
        int eventTime;

        if (!(iss >> arrivalTime))
        {
            break;
        }

        Process temp(processId, arrivalTime);

        while (iss)
        {
            iss >> eventName >> eventTime;
            if (eventName == "CPU")
            {
                temp.addEvent(1, eventTime);
            }
            else
            {
                temp.addEvent(0, eventTime);
            }
        }

        processes.push_back(temp);

        EventInfo info;
        info.action = Event::ARRIVE;
        info.process = processes.back();
        info.timestamp = arrivalTime;

        addEvent(info);
        processId++;
    }
}

void EventHandler::addEvent(EventInfo event)
{
    Q.push(event);
}

void EventHandler::updateIdle()
{
    cpuIdle = !cpuIdle;
}

void EventHandler::showpq()
{
    auto copy = Q;
    while (!copy.empty())
    {
        std::cout << "Action: " << copy.top().action << ", Process: " << copy.top().process.id << ", Timestamp: " << copy.top().timestamp;
        copy.pop();
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int EventHandler::getTime()
{
    return clock;
}

bool EventHandler::is_Idle()
{
    return cpuIdle;
}

EventInfo EventHandler::getNextElement()
{
    EventInfo temp = Q.top();

    Q.pop();

    if (temp.timestamp > clock)
    {
        clock = temp.timestamp;
    }

    return temp;
}