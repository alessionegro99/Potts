CC = gcc
CFLAGS = -O2 -Wall -Wextra -Werror -pedantic -Wconversion -std=c99
INCLUDES = -Iinclude

# Source and object files
LIB_SOURCES = lib/geometry.c lib/param.c lib/pcg32min.c lib/random.c lib/spin_conf_def.c lib/spin_conf_meas.c lib/spin_conf_upd.c
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)

SRC_SOURCES = src/Ising.c
SRC_OBJECTS = $(SRC_SOURCES:.c=.o)

# Target executable
TARGET = Ising

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(LIB_OBJECTS) $(SRC_OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lm

# Compile .c files into .o files
lib/%.o: lib/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(LIB_OBJECTS) $(SRC_OBJECTS) $(TARGET) dati.dat log.dat template_input.in

.PHONY: all clean
