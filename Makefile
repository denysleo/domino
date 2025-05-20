CC = gcc
CFLAGS = -Wall -I./cli-lib/include -I./src
LDFLAGS = -lncurses

SRC = $(filter-out cli-lib/src/main.c, $(wildcard src/*.c cli-lib/src/*.c))
OBJ = $(SRC:.c=.o)
EXEC = domino

all: $(EXEC)

# Regra para gerar .o a partir de .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

