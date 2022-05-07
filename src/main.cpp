#include "../include/fcfs.h"

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
    while (std::getline(scheduleStream, line))
    {
        std::istringstream iss(line);

        if (!(iss >> scheduleName))
        {
            break;
        }
    }

    if (scheduleName == "FCFS")
    {
        FCFS fcfs(processFilename);
    }

    return 0;
}