.PHONY : clean

# INCLUDE :=
BUILDDIR = ./build
CXXFLAGS := -std=c++11 -Wall \
	-D DEBUG=true -g
CXX := g++

TARGET := $(BUILDDIR)/main.out
OBJ := $(TARGET:%.out=%.o)

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(BUILDDIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY : run
run:
	@echo ---$(shell date)--- > output
	@./$(TARGET) <input >output 
# 2>&1

clean:
	-rm -rf *.out
	-rm -rf *.o
	-rm -rf $(TARGET)
	-rm -rf $(OBJ)
	-rm -rf *.dSYM
