
CXXFLAGS = -std=c++11 -Wall -D DEBUG=true
CFLAGS = -g
CXX = g++

TARGET = main.out

all:
	make clean
	make c1.o
	make c0.o
	make main.o
	$(CXX) -o $(TARGET) main.o c1.o c0.o $(CXXFLAGS) $(CFLAGS)

main.o:
	$(CXX) -c main.cpp $(CFLAGS) $(CXXFLAGS)

c1.o:
	$(CXX) -c c1.cpp $(CFLAGS) $(CXXFLAGS)

c0.o:
	$(CXX) -c c0.cpp $(CFLAGS) $(CXXFLAGS)

clean:
	-rm -rf *.o
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
