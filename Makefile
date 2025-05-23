CC := gcc
CFLAGS := -Wall -I. -I./include -I./cli-lib/include
LIBS := -lncurses

PROJECT_SRC := \
    jogador.c \
    jogo.c \
    src/menu.c \
    pedra.c \
    src/main.c \
    tabuleiro.c

CLI_SRC := \
    cli-lib/src/screen.c \
    cli-lib/src/keyboard.c \
    cli-lib/src/timer.c

SRC := $(PROJECT_SRC) $(CLI_SRC)
OBJ := $(SRC:.c=.o)
EXEC := domino

all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

clean:
	rm -f $(OBJ) $(EXEC)
	rm -f src/*.o cli-lib/src/*.o
