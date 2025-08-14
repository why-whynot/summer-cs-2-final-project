# Makefile for mini-kd-database project
# Supports C++14 standard for std::make_unique

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Isrc
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
MAIN_TARGET = kdtree_app
TEST_TARGET = test_kdtree
UPDATE_TEST_TARGET = test_update

# Main executable
$(MAIN_TARGET): main.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) main.cpp $(SOURCES) -o $(MAIN_TARGET)

# Test executable
$(TEST_TARGET): test_kdtree.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) test_kdtree.cpp $(SOURCES) -o $(TEST_TARGET)

# Update test executable
$(UPDATE_TEST_TARGET): test_update_functionality.cpp $(SOURCES)
	$(CXX) $(CXXFLAGS) test_update_functionality.cpp $(SOURCES) -o $(UPDATE_TEST_TARGET)

# Build all targets
all: $(MAIN_TARGET) $(TEST_TARGET) $(UPDATE_TEST_TARGET)

# Clean build artifacts
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) $(UPDATE_TEST_TARGET) $(SRCDIR)/*.o

# Run tests
test: $(TEST_TARGET) $(UPDATE_TEST_TARGET)
	./$(TEST_TARGET)
	./$(UPDATE_TEST_TARGET)

# Run interactive CLI
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Phony targets
.PHONY: all clean test run
