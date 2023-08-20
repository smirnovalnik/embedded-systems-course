
include ../shared/mdr32f9q2i.conf
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -D__NO_SYSTEM_INIT
LDSCRIPTS=-L. -L$(BASE) -T mdr32f9q2i.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) -I../shared

RISCV_PREFIX=riscv-none-elf-

SRC_PREFIX=es_lect3_

BUILD_DIR=build

all:
	@echo "choose target ex1, ex2, ex3, ex4 or ex5"

ex1: $(SRC_PREFIX)ex1.c $(STARTUP)
	-mkdir -p $(BUILD_DIR)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -g -o $(BUILD_DIR)/$@.axf

ex2: $(SRC_PREFIX)ex2.c $(STARTUP)
	-mkdir -p $(BUILD_DIR)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -g -o $(BUILD_DIR)/$@.axf

ex3: $(SRC_PREFIX)ex3.c $(STARTUP)
	@echo "you should include SPL package for compiling this example"

ex4: $(SRC_PREFIX)ex4.c $(STARTUP)
	-mkdir -p $(BUILD_DIR)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -g -O0 -o $(BUILD_DIR)/$@.axf
	$(OBJDUMB) -S --disassemble -d $(BUILD_DIR)/$@.axf > $(BUILD_DIR)/$@.S

ex5: $(SRC_PREFIX)ex4.c
	-mkdir -p $(BUILD_DIR)
	$(RISCV_PREFIX)gcc $^ -g -O0 -o $(BUILD_DIR)/$@.axf
	$(RISCV_PREFIX)objdump -S --disassemble -d $(BUILD_DIR)/$@.axf > $(BUILD_DIR)/$@.S

clean:
	-rm -rf $(BUILD_DIR)
