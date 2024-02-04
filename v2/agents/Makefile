.PHONY: all
all: random.out a01.out a02.out a03.out

random.out: random.o ../modules/board.o
				indent -kr -ts255 random.c
				gcc -o $@ $^

a01.out: a01.o
				indent -kr -ts255 a01.c
				gcc -o $@ $^

a02.out: a02.o ../modules/board.o
				indent -kr -ts255 a02.c
				gcc -o $@ $^

a03.out: a03.o ../modules/board.o
				indent -kr -ts255 a03.c
				gcc -o $@ $^

.PHONY: clean
clean:
				rm -f *.o *.out *.*~

