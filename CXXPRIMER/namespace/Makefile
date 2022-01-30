
CXXFLAGS = -std=c++11 -Wall -D DEBUG=true
CFLAGS = -g
CXX = g++

TARGET = main.out

all:
	make clean
	make un1.o
	make un2.o
	make namespace.o
	$(CXX) -o $(TARGET) namespace.o un1.o un2.o $(CXXFLAGS) $(CFLAGS)

namespace.o:
	$(CXX) -c namespace.cpp $(CFLAGS) $(CXXFLAGS)

un1.o:
	$(CXX) -c un1.cpp $(CFLAGS) $(CXXFLAGS)

un2.o:
	$(CXX) -c un2.cpp $(CFLAGS) $(CXXFLAGS)

clean:
	-rm -rf *.o
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
