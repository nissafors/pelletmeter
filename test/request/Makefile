PROJ_DIR=../..
BUILD_NAME=request_test.exe
BUILD_DIR=./build

INCLUDES =\
-I. \
-I$(PROJ_DIR)/cpputest/include \
-I$(PROJ_DIR)/src

SOURCES =\
./request_test.cpp \
$(PROJ_DIR)/src/request.cpp

LIB_DIRS =-L$(PROJ_DIR)/cpputest/cpputest_build/lib

LIBS=-lCppUTest

CC=g++
CXXFLAGS = -Wall -Wextra -Wunreachable-code -std=gnu++11 -ggdb -O0 -D'UNUSED(x)=(void)(x)' -DCPPUTEST_MEM_LEAK_DETECTION_DISABLED

VPATH = $(dir $(SOURCES))

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.cpp=.o)))

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(INCLUDES) $(CXXFLAGS)

all: $(OBJECTS)
	$(CC) -o $(BUILD_DIR)/$(BUILD_NAME) $^ $(CXXFLAGS) $(INCLUDES) $(LIB_DIRS) $(LIBS)

run-tests: all
	$(BUILD_DIR)/$(BUILD_NAME)

clean:
	rm -rf build

.PHONY: all run-tests clean