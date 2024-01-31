.PHONY: all
all: random.out a01.out

random.out: random.o ../modules/board.o
				indent -kr -ts255 random.c
				gcc -o $@ $^

a01.out: a01.o
				indent -kr -ts255 a01.c
				gcc -o $@ $^

.PHONY: clean
clean:
				rm -f *.o *.out *.*~

