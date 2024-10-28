SRC_DIR := src
TEST_DIR := test

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(SRC_DIR)/*.h)


TEST_HEADERS := $(wildcard $(TEST_DIR)/*.h)
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)

build/test.exe: test/test.h test/*.cpp $(HEADERS)
	@g++ -o build/test.exe -g $(SOURCES) $(TEST_SOURCES)

build/sudoku.exe: $(SOURCES) $(HEADERS) examples/sudoku.cpp
	@g++ -o build/sudoku.exe -g $(SOURCES) $(HEADERS) examples/sudoku.cpp

run_tests: build/test.exe
	@build/test.exe

run_sudoku: build/sudoku.exe
	@build/sudoku.exe

clean:
	rm -r build

.PHONY: run_tests run_sudoku