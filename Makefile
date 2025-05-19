CC = gcc
CFLAGS = -Wall -I./src -I./include -I./cli-lib/include
SRC = $(filter-out cli-lib/src/main.c, $(wildcard src/*.c cli-lib/src/*.c))
OBJ = $(SRC:.c=.o)
EXEC = domino

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lncurses

clean:
	rm -f $(OBJ) $(EXEC)


