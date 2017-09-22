SHELL:=/bin/bash

CC=g++
CFLAGS=-std=c++98 -pedantic -Wall -g -pthread

STATLIB=lib/recoverable_queue.a


INCLUDE=include
BIN=bin

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,bin/%.o,$(SRC))


GTEST_SRC := $(wildcard gtests/src/*.cpp)
GTEST_TARGET := $(patsubst gtests/src/%.cpp,gtests/%,$(GTEST_SRC))
GTEST_INCLUDE=gtests/include
GTEST_LIB=gtests/lib/libgtest.a
GTEST_FLAGS=-isystem $(GTEST_INCLUDE) -pthread -I$(INCLUDE)

# make all compiles the static library
all: $(STATLIB)

# make test compiles the test cases into a test executable
test: $(GTEST_TARGET)
	cd gtests && ./main_test
	cd gtests && ./task_manager_test


$(STATLIB): $(OBJ)
	@mkdir -p $(@D)
	ar -rcs $@ $^

$(OBJ): $(SRC)
	@mkdir -p $(@D)
	$(CC) -c $(patsubst bin/%.o,src/%.cpp,$@) -I$(INCLUDE) -o $@ $(CFLAGS)

$(GTEST_TARGET): $(GTEST_SRC) $(GTEST_LIB) $(STATLIB)
	$(CC) $(patsubst gtests/%,gtests/src/%.cpp,$@) -o $@ $(GTEST_FLAGS) $(filter-out %.cpp,$^)

clean:
	@rm -f $(GTEST_TARGET1) $(GTEST_TARGET2) $(STATLIB) bin/*.o

