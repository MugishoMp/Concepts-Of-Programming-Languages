# Directory layout looks like this
# 	include/
# 		*.h
# 	src/
# 		*.c
# 	tests/
# 		test_*.c
# 	LICENCE (what is this?)
# 	Makefile
# 	README.md

INC_DIR = include
SRC_DIR = src
SOURCES = $(sort $(shell find $(SRC_DIR) -name '*.c' ))
OBJECTS = $(SOURCES:.c=.o)
DEPS    = $(OBJECTS:.o=.d)
TARGET = lexer
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
CPPFLAGS = $(addprefix -I, $(INC_DIR))

.PHONY: all clean debug release

release: CFLAGS += -O2 -DNDEBUG
release: all

debug: CFLAGS += -O0 -g
debug: all

all: $(TARGET)
    
clean: 
	$(RM) $(OBJECTS) $(DEPS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<

-include $(DEPS)
