BIN_DIR = ./bin
OBJS_DIR = ./obj
SRC_DIR = ./src
INCLUDE_DIR = ./include

OBJS	= $(OBJS_DIR)/main.o
SOURCE	= $(SRC_DIR)/main.cpp
HEADER = $(INCLUDE_DIR)/process.h $(INCLUDE_DIR)/event.h $(INCLUDE_DIR)/fcfs.h $(INCLUDE_DIR)/vrr.h $(INCLUDE_DIR)/srt.h $(INCLUDE_DIR)/hrrn.h $(INCLUDE_DIR)/feedback.h
OUT	= $(BIN_DIR)/main
CC	 = g++
FLAGS	 = -g -c -std=c++11 -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

$(OBJS_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUT)