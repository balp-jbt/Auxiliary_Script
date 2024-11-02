CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O3 --static

SRC_DIR := src
BUILD_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.cpp)

OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

TEST := $(BUILD_DIR)/test

all: $(TEST)

$(TEST): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@rm -f $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean