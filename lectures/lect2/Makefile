
SRC_PREFIX=es_lect2_

BUILD_DIR=build

all:
	@echo "choose target ex1, ex2 or ex3"

ex1: $(SRC_PREFIX)ex1.c
	-mkdir -p $(BUILD_DIR)
	gcc $(SRC_PREFIX)ex1.c -o $(BUILD_DIR)/ex1

ex2: $(SRC_PREFIX)ex2.c
	-mkdir -p $(BUILD_DIR)
	gcc $(SRC_PREFIX)ex2.c -o $(BUILD_DIR)/ex2

ex3: $(SRC_PREFIX)ex3.c
	-mkdir -p $(BUILD_DIR)
	gcc $(SRC_PREFIX)ex3.c -o $(BUILD_DIR)/scat

clean:
	-rm -rf $(BUILD_DIR)
