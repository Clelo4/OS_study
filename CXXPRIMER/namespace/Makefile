.PHONY : clean

CXXFLAGS = -std=c++11 -Wall -D DEBUG=true -g
CXX = g++

TARGET = main.out
OBJ = namespace.o un1.o un2.o

$(TARGET): $(OBJ)
	echo $^
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	-rm -rf *.o
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
