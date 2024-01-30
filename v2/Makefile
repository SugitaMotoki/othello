NAME = game
PROGRAM = $(NAME).out
OBJS = $(NAME).o modules/board.o

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
				gcc -o $@ $^

indent: $(NAME).c
				indent -kr -ts255 $^
				make -C ./modules indent

.PHONY: clean
clean:
				rm -f *.o *.out *.*~
				make -C ./agents clean
				make -C ./modules clean