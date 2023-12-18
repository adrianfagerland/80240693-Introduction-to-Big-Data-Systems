# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Target executable names
TARGET2 = edge-cut
TARGET3 = vertex-cut

all: $(TARGET2) $(TARGET3)
$(TARGET2): edge-cut.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET2) edge-cut.cpp

$(TARGET3): vertex-cut.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET3) vertex-cut.cpp

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)