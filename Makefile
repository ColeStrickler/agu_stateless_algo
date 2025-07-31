# Makefile for building a C++ program using magic number division

CXX = g++
CXXFLAGS = -O2 -Wall -std=c++17
TARGET = magic_div
SRC = main.cpp


run: all
	chmod +x ./$(TARGET)
	./$(TARGET)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
