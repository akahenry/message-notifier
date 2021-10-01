CXX       := g++
CXX_FLAGS := -std=c++17

BIN     := ../bin
SRC     := src
INCLUDE := include

COMMON_PATH := ../common
COMMON_INCLUDE := $(COMMON_PATH)/include
COMMON_SRC := $(COMMON_PATH)/src

LIBRARIES   := -lpthread
EXECUTABLE  := server
DEBUG_EXECUTABLE := server.debug


all: $(BIN)/$(EXECUTABLE)
	-mkdir data

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(COMMON_SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(COMMON_INCLUDE) $^ -o $@ $(LIBRARIES)

debug: $(BIN)/$(DEBUG_EXECUTABLE)

$(BIN)/$(DEBUG_EXECUTABLE): $(SRC)/*.cpp $(COMMON_SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -g -I$(INCLUDE) -I$(COMMON_INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)