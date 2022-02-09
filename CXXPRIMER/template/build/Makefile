# Refrence: https://seisman.github.io/how-to-write-makefile/index.html

# 伪目标
.PHONY : clean

# 文件搜寻
# VPATH = ../
vpath %.h ../include
# 按照vpath语句的先后顺序来执行搜索。
vpath %.h ../../include
 # 先在../sr搜索，再在../test搜索
vpath %.cpp ../src:../test

nullstring :=
# 而后面采用“#”注释符来表示变量定义的终止
space := $(nullstring) # end of the line

CXXFLAGS = -std=c++11 -Wall -D DEBUG=true -I ../include -g
CXX = g++

TARGET = main.out
OBJ = extern_template_impl.o main.o template_instan.o t1.o t2.o t3.o
# OBJ := $(wildcard *.o)

$(TARGET): $(OBJ)
	@echo $(SHELL)
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS) $(CFLAGS); ./$(TARGET)

main.o: extern_template.h specializations.h template.h template_proxy.h
extern_template_impl.o: extern_template.cpp
# 多目标
template_instan.o t1.o t2.o t3.o: %.o : %.cpp template.h
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -rf $(OBJ)
	-rm -rf $(TARGET)
	-rm -rf *.dSYM
