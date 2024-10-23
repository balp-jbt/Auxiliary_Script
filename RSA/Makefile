CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 

SRC_DIR := src
BUILD_DIR := build

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
	rm -rf $(BUILD_DIR)/$(TEST)

.PHONY: all clean