CC=g++
CFLAGS=-c -Wall
LDFLAGS=

SOURCES=main.cpp stack_funcs_hash.cpp error_func.cpp log_funcs.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# TODO: What if header changes? Will your necessary file automatically recompile, or won't it?
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf hello *.o
