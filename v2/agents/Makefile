EXES = random.out a01.out

.PHONY: all
all: $(EXES)

.SUFFIXES: .c .out
.c.out:
				gcc -o $@ $<

.PHONY: clean
clean:
				rm -f *.out *.c~

