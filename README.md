# -CS4348-OperatingSystems-Project03
Program to test various scheduling algorithms

# Directory Overview

1. include
   1. Houses all the include files for the programs. Each scheduling algorithm is implemented in its own include file.
2. out
   1. Houses the out file created through the program print
3. src
   1. Houses all the source .cpp files for the programs. The Driver Program main resides in the root of /src
4. obj
   1. Houses all the .obj files that are created by the [Makefile]()
5. bin
   1. Houses the program executable. The [Makefile]() will always build the exe to this folder.
6. Process File
   1. Houses the process event list that will be read in by the program.
7. Scheduler File
   1. Houses the scheduling file that the program will read in. Program will only read one at a time.

# File List

1. **Driver Program**
   1. `main.cpp`: the only file for the driver program. Responsible for creating the correct scheduling object. Only houses the logic for reading in the files and passing on the correct parameters.
2. **Scheduling Program**
   1. `event.h`: the goal of this file is to define the event handler that handles the priority queue of events. The logic for creating an event and also reading the next are included here. Also houses common parameters like the cpuIdle case.
   2. `process.h`: the goal of this file is to house a single line of input from the Process File into a custom data structure. This makes it easier to handle the next event and timings for a single process about to enter the queue.
   3. `fcfs.h`: this file defines the logic for the First Come First Serve Scheduling algorithm. The code loops until all processes exit within the constructor so only one call to the object creation has to be made. Includes the process and event files to create local instances that the code can reference.
   4. `feedback.h`: this file defines the logic for the Feedback Scheduling algorithm. The code loops until all processes exit within the constructor so only one call to the object creation has to be made. Includes the process and event files to create local instances that the code can reference.
   5. `hrrn.h`: this file defines the logic for the Highest Response Ratio Next Scheduling algorithm. The code loops until all processes exit within the constructor so only one call to the object creation has to be made. Includes the process and event files to create local instances that the code can reference.
   6. `srt.h`: this file defines the logic for the Shortest Remaining Time Scheduling algorithm. The code loops until all processes exit within the constructor so only one call to the object creation has to be made. Includes the process and event files to create local instances that the code can reference.
   7. `vrr.h`: this file defines the logic for the Virtual Round Robin Scheduling algorithm. The code loops until all processes exit within the constructor so only one call to the object creation has to be made. Includes the process and event files to create local instances that the code can reference.

# Compile and Run

NOTE: All files are included for run in the zip. To skip compile, go to /bin and run ./main

The program was run and tested on **Ubuntu 20.04 using g++ version 9.3**. The program should compile on any UNIX based system. In order to compile the program, `cd` to the `root` directory. Run the following command:
>`make` *(may want to run `make clean` first)*

If the compile is successful, an output file should be created within the same directory. To run the exe, run `./[exe] [path to schedule file <relative to exe>] [path to process file <relative to exe>]` (**assuming UNIX system**)
>(*EXAMPLE: `./main ../Scheduler\ File/fcfs.sf ../Process\ File/example.pf`*).

The output of the file will show within `out/[algorithm].out` rather than standard output. This is simply done to format the output neatly utilizing existing development. When running, ensure that *[algorithm].out* is deleted as the program opens the file for append instead of creating a new file each run.
