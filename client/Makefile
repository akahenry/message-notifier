CXX       := g++
CXX_FLAGS := 

BIN     := ../bin
SRC     := src
INCLUDE := include

LIBRARIES   := 
EXECUTABLE  := client


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
	-rm -r $(TEMP)