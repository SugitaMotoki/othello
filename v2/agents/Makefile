.PHONY: all
all: random.out a01.out

random.out: random.o ../modules/board.o
				gcc -o $@ $^

a01.out: a01.o
				gcc -o $@ $^

.PHONY: clean
clean:
				rm -f *.o *.out *.*~

