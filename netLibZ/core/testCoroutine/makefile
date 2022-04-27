# $^ 依赖 不重复
# $@ 目标
# @ 不显示命令执行 
# -rm -:失败不停止
# := 覆盖之前的值
# ?= 如果没有被赋值过就赋予等号后面的值
# += 添加等号后面的值
# ${HOME} /root
NETLIBZ_DIRECTORY ?= $(HOME)/netLibZ
NETLIBZ_INCLUDE = $(NETLIBZ_DIRECTORY)
NETLIBZ_OBJ_BASE = $(NETLIBZ_INCLUDE)/netLibZ/base
NETLIBZ_OBJ_CORE = $(NETLIBZ_INCLUDE)/netLibZ/core
TARGET=testCoroutine
LIBS=-pthread
# BENCHMARK=-lbenchmark
BENCHMARK=-lbenchmark
GDB=-g

OBJS=testCoroutine.o $(NETLIBZ_OBJ_CORE)/coctx.o $(NETLIBZ_OBJ_CORE)/Coroutine.o \
	$(NETLIBZ_OBJ_CORE)/CurrentThread.o $(NETLIBZ_OBJ_CORE)/ShareStack.o $(NETLIBZ_OBJ_CORE)/coctx_swap.o
CXXFLAGS=-I$(NETLIBZ_INCLUDE)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS) $(BENCHMARK) $(GDB) 
%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@ $(LIBS) $(GDB)
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@ $(LIBS) $(GDB)
clean :
	rm $(OBJS) $(TARGET)
.PHONY: clean