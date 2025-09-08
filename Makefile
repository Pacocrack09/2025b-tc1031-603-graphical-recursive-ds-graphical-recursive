CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall
TARGET = hilbert
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o lines.txt
