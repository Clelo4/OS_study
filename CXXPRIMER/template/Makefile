
CXXFLAGS = -std=c++11 -Wall -D DEBUG=true
CFLAGS = -g
CXX = g++

TARGET = main.out

all:
	make clean
	make extern_template_impl.o
	make main.o
	make template_instan.o
	make t1.o
	make t2.o
	make t3.o
	$(CXX) -o $(TARGET) main.o extern_template_impl.o template_instan.o t1.o t2.o t3.o $(CXXFLAGS) $(CFLAGS)
	./$(TARGET)

main.o:
	$(CXX) -c main.cpp $(CFLAGS) $(CXXFLAGS)

extern_template_impl.o:
	$(CXX) -c extern_template_impl.cpp $(CFLAGS) $(CXXFLAGS)

template_instan.o:
	$(CXX) -c template_instan.cpp $(CFLAGS) $(CXXFLAGS)

t1.o:
	$(CXX) -c t1.cpp $(CFLAGS) $(CXXFLAGS)
t2.o:
	$(CXX) -c t2.cpp $(CFLAGS) $(CXXFLAGS)
t3.o:
	$(CXX) -c t3.cpp $(CFLAGS) $(CXXFLAGS)

clean:
	-rm -rf *.o
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
