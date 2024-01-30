PROGRAMS = random.out a01.out

.PHONY: all
all: $(PROGRAMS)

%.out: %.c
				gcc -o $@ $<

.PHONY: clean
clean:
				rm -f *.o *.out *.*~

