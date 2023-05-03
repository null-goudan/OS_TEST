# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic

# 目标文件名和源文件列表
TARGET = main
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 生成目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 清除生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
