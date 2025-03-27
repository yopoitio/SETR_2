# Paulo Pedreiras, Mar/2022
# Adapted from the ThrowTheSwitch.org   
# 
# This makefile assumes that test source and test files are in folders 
#   "src" and "test", respectively
# Moreover, this makefile, "src" and "test" are within the root Unity 
#	folder.
# Finaly, the SW module name has a MODULE_NAME.c file and MODULE_NAME.h
# 	file, and the test file is testMODULENAME.c
# If the rules above are obeyed only MODULE_NAME needs to be adjusted

MODULE_NAME = CMDPROC
FILE_NAME = cmdproc

# Paths
UNITY_ROOT = Unity
SRC_FOLDER = src
TEST_FOLDER = test

# Commands
CLEANUP = rm -f
MKDIR = mkdir -p

#Compiler
C_COMPILER = gcc
CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
#CFLAGS += -Wno-misleading-indentation

MAIN_FILES = $(SRC_FOLDER)/main.c $(SRC_FOLDER)/$(MODULE_NAME)/$(FILE_NAME).c
TEST_FILES = $(UNITY_ROOT)/src/unity.c $(TEST_FOLDER)/test$(MODULE_NAME).c $(SRC_FOLDER)/$(MODULE_NAME)/$(FILE_NAME).c

INC_DIRS=-I$(SRC_FOLDER) -I$(UNITY_ROOT)/src
SYMBOLS=

TEST_EXEC = test$(MODULE_NAME)
MAIN_EXEC = main$(MODULE_NAME)

all: clean test main

.PHONY: clean

test: $(TEST_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(TEST_FILES) -o $(TEST_EXEC) -lm

main: $(MAIN_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(MAIN_FILES) -o $(MAIN_EXEC) -lm

clean:
	$(CLEANUP) $(TARGET) $(MAIN_EXEC)
	


