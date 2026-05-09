BIN_NAME ?= app
SRC_DIR ?= src
INC_DIR ?= include
BUILD_DIR ?= build
BIN_DIR ?= bin
TARGET := $(BIN_DIR)/$(BIN_NAME)

UNAME_S := $(shell uname -s)

CC := clang
CXX := clang++
CSTD ?= c11
CXXSTD ?= c++17
MODE ?= debug
SAN ?= address
ARGS ?=

C_WARN := -Wall -Wextra -Wpedantic -Wshadow -Wconversion
CXX_WARN := -Wall -Wextra -Wpedantic -Wshadow -Wconversion
CPPFLAGS := -I$(INC_DIR) -MMD -MP

ifeq ($(MODE),release)
  OPT := -O2 -DNDEBUG
else
  OPT := -O0 -g3
endif

ifeq ($(UNAME_S),Darwin)
  THREAD_FLAGS ?=
else
  THREAD_FLAGS ?= -pthread
endif

CFLAGS := -std=$(CSTD) $(OPT) $(C_WARN)
CXXFLAGS := -std=$(CXXSTD) $(OPT) $(CXX_WARN)
LDFLAGS := $(THREAD_FLAGS)
LDLIBS ?=

C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CXX_SRCS))
OBJS := $(C_OBJS) $(CXX_OBJS)
DEPS := $(OBJS:.o=.d)

.PHONY: all run debug release asan clean print

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(if $(CXX_SRCS),$(CXX),$(CC)) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(THREAD_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(THREAD_FLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

run: all
	./$(TARGET) $(ARGS)

debug:
	$(MAKE) MODE=debug all

release:
	$(MAKE) MODE=release all

asan: clean
	$(MAKE) MODE=debug CPPFLAGS="$(CPPFLAGS)" CFLAGS="$(CFLAGS) -fsanitize=$(SAN) -fno-omit-frame-pointer" CXXFLAGS="$(CXXFLAGS) -fsanitize=$(SAN) -fno-omit-frame-pointer" LDFLAGS="$(LDFLAGS) -fsanitize=$(SAN)" all
	./$(TARGET) $(ARGS)

print:
	@echo "CC=$(CC)"
	@echo "CXX=$(CXX)"
	@echo "C_SRCS=$(C_SRCS)"
	@echo "CXX_SRCS=$(CXX_SRCS)"
	@echo "OBJS=$(OBJS)"
	@echo "TARGET=$(TARGET)"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)
