CXX := g++
CXX_FLAGS := -std=c++17 -lcurl -lcurlpp -ggdb

BIN := bin
SRC := src
INCLUDE := include

LIBRARIES := lib
EXECUTABLE := crawler

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ -I$(LIBRARIES)

clean:
	-rm $(BIN)/*