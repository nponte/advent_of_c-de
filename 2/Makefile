# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread

# Include paths
INCLUDES = -I/usr/local/include -I/usr/include

# Library paths
LIBPATHS = -L/usr/local/lib -L/usr/lib

# Libraries to link
LIBS =  -lglog -ldouble-conversion -lz  -lsnappy -llz4 -lzstd -ljemalloc

# Source files and output binary
SRCS = read_reports.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = read_reports

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPATHS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
