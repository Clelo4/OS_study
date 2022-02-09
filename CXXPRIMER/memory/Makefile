.PHONY : clean

CXXFLAGS = -std=c++11 -Wall -D DEBUG=true -g
CXX = g++

TARGET = main.out
OBJ = main.o

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS)

clean:
	-rm -rf $(OBJ)
	-rm -rf $(TARGET)
	-rm -rf *.dSYM