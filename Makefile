.PHONY: all
all: indent game subdirmake

.PHONY: allclean
allclean: clean subdirclean

game: game.c
				gcc -o game game.c

.PHONY: indent
indent:
				indent -kr -ts255 game.c

.PHONY: clean
clean:
				rm -f *.c~ game

.PHONY: subdirmake
subdirmake:
				make -C agents/

.PHONY: subdirclean
subdirclean:
				make -C agents/ clean
