# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread

# Include paths
INCLUDES = -I/usr/local/include -I/usr/include

# Library paths
LIBPATHS = -L/usr/local/lib -L/usr/lib
LIBS =  -lglog -ldouble-conversion -lz  -lsnappy -llz4 -lzstd -ljemalloc -ltbb


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = stars


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBPATHS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
