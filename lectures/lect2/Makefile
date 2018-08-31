
include ../shared/mdr32f9q2i.conf
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -D__NO_SYSTEM_INIT
LDSCRIPTS=-L. -L$(BASE) -T mdr32f9q2i.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) -I../shared

all:
	@echo "choose target ex1, ex2 or ex3"

ex1: es_lect2_ex1.c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -g -o $@.axf

ex2: es_lect2_ex2.c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -g -o $@.axf

ex3: es_lect2_ex3.c $(STARTUP)
	@echo "you should include SPL package for compiling this example"

clean:
	-rm *.exe *.axf *.map *.o
