CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: cthread clib

cthread:
	$(CC) -Wall -c $(SRC_DIR)/cthread.c -o $(BIN_DIR)/cthread.o

clib:
	ar crs $(LIB_DIR)/libcthread.a $(BIN_DIR)/cthread.o $(BIN_DIR)/support.o

clean:
	find $(BIN_DIR) -name ".o" | grep -v "support" | xargs rm -rf
	rm -rf $(LIB_DIR)/.a $(LIB_DIR)/~ $(INC_DIR)/~ $(BIN_DIR)/~ $(SRC_DIR)/~ *~
