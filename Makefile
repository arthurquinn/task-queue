SHELL:=/bin/bash

CC=g++
CFLAGS=-std=c++98 -pedantic -Wall -g -pthread

STATLIB=lib/recoverable_queue.a

SRC=src
INCLUDE=include
BIN=bin

OBJ=$(BIN)/queue.o $(BIN)/queue_item.o $(BIN)/utils.o $(BIN)/dispatcher.o $(BIN)/task_manager.o

GTEST_TARGET1=gtests/main_test
GTEST_SRC1=gtests/src/main_test.cpp
GTEST_TARGET2=gtests/task_manager_test
GTEST_SRC2=gtests/src/task_manager_test.cpp
GTEST_INCLUDE=gtests/include
GTEST_LIB=gtests/lib/libgtest.a
GTEST_FLAGS=-isystem $(GTEST_INCLUDE) -pthread -I$(INCLUDE)

# make all compiles the static library
all: $(STATLIB)

# make test compiles the test cases into a test executable
# must have the googletest library installed on your machine in order for these to work
test: $(GTEST_TARGET1) $(GTEST_TARGET2)
	cd gtests && ./main_test
	cd gtests && ./task_manager_test


$(STATLIB): $(OBJ)
	@mkdir -p $(@D)
	ar -rcs $@ $^

$(BIN)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $^ -I$(INCLUDE) -o $@ $(CFLAGS)

$(GTEST_TARGET1): $(GTEST_SRC1) $(GTEST_LIB) $(STATLIB)
	$(CC) $^ -o $@ $(GTEST_FLAGS)

$(GTEST_TARGET2): $(GTEST_SRC2) $(GTEST_LIB) $(STATLIB)
	$(CC) $^ -o $@ $(GTEST_FLAGS)

clean:
	@rm -f $(GTEST_TARGET1) $(GTEST_TARGET2) $(STATLIB) bin/*.o

