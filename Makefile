CPP = g++
CPPFLAGS = -g

MEMORY_FLAG = -DMEMORY_CHECK

INCLUDE := src
SOURCE := $(wildcard test_src/*.cpp)
OBJ := $(addprefix build/obj/, $(notdir $(SOURCE:.cpp=.o)))
MEMORY_OBJ := $(addprefix build/mem_obj/, $(notdir $(SOURCE:.cpp=.o)))

EXE := build/test
MEMORY_EXE := build/mem_test

all: $(EXE)

run: $(EXE)
	./$(EXE)

mem_check: $(MEMORY_EXE)
	valgrind ./$(MEMORY_EXE)

$(EXE): build $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(EXE) $(OBJ)

$(MEMORY_EXE): build $(MEMORY_OBJ)
	$(CPP) $(CPPFLAGS) -o $(MEMORY_EXE) $(MEMORY_OBJ)

build/mem_obj/%.o: test_src/%.cpp
	$(CPP) $(CPPFLAGS) $(MEMORY_FLAG) -I$(INCLUDE) -c -o $@ $<

build/obj/%.o: test_src/%.cpp
	$(CPP) $(CPPFLAGS) -I$(INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	@if [ -d "build" ]; then rm -rf build; fi

.PHONY: build
build:
	@if [ ! -d "build" ]; then mkdir build && mkdir build/obj && mkdir build/mem_obj; fi
