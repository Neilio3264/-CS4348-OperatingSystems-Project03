#include "../include/process.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void readProcessFile(vector<Process> &processes, string file);

int main(int argc, char **argv)
{
    string filename = "";

    if (argc == 3)
    {
        filename = argv[2];
    }

    vector<Process> processList;
    readProcessFile(processList, filename);

    for (int i = 0; i < processList.size(); i++)
    {
        cout << processList[i].getNextTime() << endl;
    }

    return 0;
}

void readProcessFile(vector<Process> &processes, string file)
{
    ifstream inputStream(file);
    string line = "";
    while (getline(inputStream, line))
    {
        istringstream iss(line);

        while (iss)
        {
            int arrivalTime;
            string eventName;
            int eventTime;

            if (!(iss >> arrivalTime))
            {
                break;
            }

            Process temp(arrivalTime);
            iss >> eventName >> eventTime;
            if (eventName == "CPU")
            {
                temp.addEvent(1, eventTime);
            }
            else
            {
                temp.addEvent(0, eventTime);
            }

            processes.push_back(temp);
        }
    }
}