.PHONY: clean all
NAME=lab1
CLEANUP = rm -fR
MKDIR = mkdir -p
TARGET_EXTENSION=
CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -g
BUILD_DIR=build

ifeq ($(OS),Windows_NT)
CLEANUP = del /q
MKDIR = mkdir
TARGET_EXTENSION=.exe
endif

all: ex1 ex2 ex3 ex4 ex5

ex1: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex1.c -o $(BUILD_DIR)/$(NAME)_ex1$(TARGET_EXTENSION)

ex2: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex2.c -o $(BUILD_DIR)/$(NAME)_ex2$(TARGET_EXTENSION)

ex3: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex3.c -o $(BUILD_DIR)/$(NAME)_ex3$(TARGET_EXTENSION)

ex4: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex4.c -o $(BUILD_DIR)/$(NAME)_ex4$(TARGET_EXTENSION)

ex5: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex5.c -o $(BUILD_DIR)/$(NAME)_ex5$(TARGET_EXTENSION)

clean:
	-@$(CLEANUP) $(BUILD_DIR)

$(BUILD_DIR):
	-@$(MKDIR) $@