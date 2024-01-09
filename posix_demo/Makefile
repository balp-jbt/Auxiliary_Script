CXX := g++
CXXFLAGS := -std=c++11
LDFLAGS := -lpthread

SRC_DIR := src
BUILD_DIR := build

basic_example: $(SRC_DIR)/basic_example.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@.exe

producer_consumer_pattern: $(SRC_DIR)/producer_consumer_pattern.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@.exe

all: basic_example producer_consumer_pattern

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean basic_example producer_consumer_pattern