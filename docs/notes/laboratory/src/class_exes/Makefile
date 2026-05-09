# 可执行文件名
TARGET := exe01

# 源文件
SRCS := exe01.cpp

# 编译器与编译参数
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2

# 默认规则：生成可执行文件
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET)
