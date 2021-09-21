CXX       := g++
CXX_FLAGS := 

BIN     := ../bin
SRC     := src
INCLUDE := include

LIBRARIES   := 
EXECUTABLE  := server
DEBUG_EXECUTABLE := server.debug


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

debug: $(BIN)/$(DEBUG_EXECUTABLE)

$(BIN)/$(DEBUG_EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -g -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)