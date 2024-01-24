CXX := g++
CXXFLAGS := -Wall -std=c++17
LDFLAGS := -lpthread

SRC_DIR := src
BUILD_DIR := build

all: Basic Producer_Consumer_Pattern

Basic: $(SRC_DIR)/Basic.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$@.exe $^

Producer_Consumer_Pattern: $(BUILD_DIR)/Producer_Consumer_Pattern.o $(BUILD_DIR)/Source.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/$@.exe $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all Basic Producer_Consumer_Pattern