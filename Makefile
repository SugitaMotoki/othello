game: game.c
				gcc -o game game.c

.PHONY: indent
indent:
				indent -kr -ts255 game.c

.PHONY: clean
clean:
				rm -f a.out *.c~