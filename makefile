# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source file
SRC = scomp.c

# Output executable
OUT = scomp

# Build target
all: $(OUT)

# Compile source file into an executable
$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean target
clean:
	rm -f $(OUT)