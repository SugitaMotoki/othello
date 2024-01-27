a01: a01.c
				indent -kr -ts255 a01.c
				gcc -o a01 a01.c

random: random.c
				gcc -o random random.c

.PHONY: indent
indent:
				indent -kr -ts255 random.c

.PHONY: clean
clean:
				rm -f a.out *.c~