CC = g++
CFLAGS = -g

INCLUDE := src
SOURCE := $(wildcard test_src/*.cpp)
OBJ := $(addprefix build/obj/, $(notdir $(SOURCE:.cpp=.o)))
EXE := build/test

all: $(EXE)

run: $(EXE)
	./$(EXE)

$(EXE): build $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

build/obj/%.o: test_src/%.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	@if [ -d "build" ]; then rm -rf build; fi

.PHONY: build
build:
	@if [ ! -d "build" ]; then mkdir build && mkdir build/obj; fi
