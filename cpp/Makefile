CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC = src/Instance.cpp src/Baseline.cpp src/MainAlgorithm.cpp src/Verifier.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = procesamiento_mineral

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
