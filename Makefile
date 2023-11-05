# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -O3

# SFML
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRCS = src/main.cpp src/mandel.cpp

# Object files
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

# Header files
HEADERS = $(SRCS:.cpp=.h)

# Executable name
TARGET = mandelbrot

# Default target
all: $(TARGET)

# Compile source files into object files
obj/%.o: src/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(SFML_FLAGS)

# Clean generated files
clean:
	rm -f $(OBJS) $(TARGET)
