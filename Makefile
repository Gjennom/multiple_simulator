CC = g++ -std=c++17
LDFLAGS = -lraylib

TARGET = multiple_simulator.exe
SRC = multiple_simulator.cpp multiple_simulator_functions.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp multiple_simulator.hpp
	$(CC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
