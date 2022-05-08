#include "../include/fcfs.h"
#include "../include/vrr.h"
#include "../include/feedback.h"
#include "../include/hrrn.h"
#include "../include/srt.h"

using namespace std;

int main(int argc, char **argv)
{
    string scheduleFileName = "";
    string processFilename = "";

    if (argc == 3)
    {
        scheduleFileName = argv[1];
        processFilename = argv[2];
    }

    std::ifstream scheduleStream(scheduleFileName);
    std::string scheduleName = "";
    std::string line = "";
    std::getline(scheduleStream, line);
    std::istringstream iss(line);

    iss >> scheduleName;

    if (scheduleName == "FCFS")
    {
        FCFS fcfs(processFilename);
    }
    else if (scheduleName == "VRR")
    {
        std::getline(scheduleStream, line);
        std::istringstream iss(line);
        string quantum = "";
        iss >> quantum;

        VRR vrr(processFilename, quantum.back() - '0');
    }
    else if (scheduleName == "FEEDBACK")
    {
        std::getline(scheduleStream, line);
        std::istringstream iss(line);
        string priority = "";
        iss >> priority;

        std::getline(scheduleStream, line);
        std::istringstream iss2(line);
        string quantum = "";
        iss2 >> quantum;

        FEEDBACK feedback(processFilename, priority.back() - '0', quantum.back() - '0');
    }
    else if (scheduleName == "HRRN")
    {
        string delimiter = "=";
        std::getline(scheduleStream, line);
        std::istringstream iss(line);
        string service = "";
        iss >> service;

        size_t pos = 0;
        string token;
        while ((pos = service.find(delimiter)) != string::npos)
        {
            token = service.substr(0, pos);
            service.erase(0, pos + delimiter.length());
        }

        std::getline(scheduleStream, line);
        std::istringstream iss2(line);
        string alpha = "";
        iss2 >> alpha;

        pos = 0;
        while ((pos = alpha.find(delimiter)) != string::npos)
        {
            token = alpha.substr(0, pos);
            alpha.erase(0, pos + delimiter.length());
        }

        HRRN hrrn(processFilename, (service == "true"), stof(alpha));
    }
    else if (scheduleName == "SRT")
    {
        string delimiter = "=";
        std::getline(scheduleStream, line);
        std::istringstream iss(line);
        string service = "";
        iss >> service;

        size_t pos = 0;
        string token;
        while ((pos = service.find(delimiter)) != string::npos)
        {
            token = service.substr(0, pos);
            service.erase(0, pos + delimiter.length());
        }

        std::getline(scheduleStream, line);
        std::istringstream iss2(line);
        string alpha = "";
        iss2 >> alpha;

        pos = 0;
        while ((pos = alpha.find(delimiter)) != string::npos)
        {
            token = alpha.substr(0, pos);
            alpha.erase(0, pos + delimiter.length());
        }

        SRT srt(processFilename, (service == "true"), stof(alpha));
    }
    return 0;
}