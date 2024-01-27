SOURCES=random a01 a02

all: $(SOURCES)

random: random.c
				indent -kr -ts255 random.c
				gcc -o random random.c
				
a01: a01.c
				indent -kr -ts255 a01.c
				gcc -o a01 a01.c

a02: a02.c
				indent -kr -ts255 a02.c
				gcc -o a02 a02.c

.PHONY: clean
clean:
				rm *.c~ $(SOURCES)