
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g 
GLOBAL_DEPS = utils.h macro.h program_structures.h preprocessor.h first_run.h program_tables.h second_run.h directives.h parser.h outputFile.h globals.h
LDFLAGS = -lasan

# List of source files
SRCS = preprocessor.c utils.c macro.c parser.c directives.c outputFile.c compiler.c first_run.c program_tables.c second_run.c


# List of object files corresponding to source files
OBJS = $(SRCS:.c=.o)

# The target executable
TARGET = assembler

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rules to compile each source file into an object file
%.o: %.c $(GLOBAL_DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target to clean up generated files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.am *.ob *.ent *.ext