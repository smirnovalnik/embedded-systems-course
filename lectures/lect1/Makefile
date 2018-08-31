
all:
	@echo "choose target ex1 or ex2"

ex1: es_lect1_ex1.c
	gcc es_lect1_ex1.c -o ex1

NAME=ex2
include ../shared/mdr32f9q2i.conf
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -D__NO_SYSTEM_INIT
LDSCRIPTS=-L. -L$(BASE) -T mdr32f9q2i.ld
LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

$(NAME): es_lect1_ex2.c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@.axf

$(NAME)_dis: $(NAME).axf
	$(OBJDUMB) -S --disassemble -d $(NAME).axf > $(NAME).S

clean:
	-rm *.exe *.axf *.map *.o
