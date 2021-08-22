NAME=lab1
CLEANUP = rm -fR
MKDIR = mkdir -p
TARGET_EXTENSION=.out
CC=gcc
CFLAGS=-Wall
OUT_DIR=out

ifeq ($(OS),Windows_NT)
CLEANUP = del /q
MKDIR = mkdir
TARGET_EXTENSION=.exe
endif

all: ex1 ex2 ex3 ex4 ex5

ex1: $(OUT_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex1.c -o $(OUT_DIR)/$(NAME)_ex1$(TARGET_EXTENSION)

ex2: $(OUT_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex2.c -o $(OUT_DIR)/$(NAME)_ex2$(TARGET_EXTENSION)

ex3: $(OUT_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex3.c -o $(OUT_DIR)/$(NAME)_ex3$(TARGET_EXTENSION)

ex4: $(OUT_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex4.c -o $(OUT_DIR)/$(NAME)_ex4$(TARGET_EXTENSION)

ex5: $(OUT_DIR)
	$(CC) $(CFLAGS) $(NAME)_ex5.c -o $(OUT_DIR)/$(NAME)_ex5$(TARGET_EXTENSION)

clean:
	-@$(CLEANUP) $(OUT_DIR)

$(OUT_DIR):
	-@$(MKDIR) $@