EXES = game.out

.PHONY: all
all: $(EXES)

.SUFFIXES: .c .out
.c.out:
				gcc -o $@ $<

.PHONY: clean
clean:
				rm -f *.out *.c~
				make -C ./agents clean