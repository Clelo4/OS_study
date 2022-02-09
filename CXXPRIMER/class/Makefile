.PHONY : clean

CXXFLAGS = -std=c++11 -Wall -D DEBUG=true -g
CXX = g++

TARGET = main.out
OBJ = main.o c1.o c0.o

$(TARGET): $(OBJ) class.h
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS)

clean:
	-rm -rf $(OBJ)
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
