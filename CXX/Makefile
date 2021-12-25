
CFLAGS = -g -std=c++11 -D DEBUG=true
CC = gcc
CXX = clang++

TARGET = 06
SOURCE = $(TARGET).cpp

all:
	$(CXX) $(CFLAGS) $(SOURCE) -o $(TARGET)
	./$(TARGET)
	make clean

clean:
	-rm -rf *.out
	-rm -rf $(TARGET)
