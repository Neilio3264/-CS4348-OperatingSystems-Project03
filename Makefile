BIN_DIR = /bin
OBJS_DIR = /lib
SRC_DIR = /src

OBJS	= $(OBJS_DIR)/main.o
SOURCE	= $(SRC_DIR)/main.cpp
OUT	= $(BIN_DIR)/main
CC	 = g++
FLAGS	 = -g -c -std=c++11 -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

$(OBJS_DIR)/main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUT)