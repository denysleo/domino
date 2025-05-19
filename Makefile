CC      := gcc
CFLAGS  := -Wall -I. -I./include -I./cli-lib/include

PROJECT_SRC := \
    jogador.c \
    jogo.c \
    pedra.c \
    src/main.c

CLI_SRC := \
    cli-lib/src/screen.c \
    cli-lib/src/keyboard.c \
    cli-lib/src/timer.c

SRC   := $(PROJECT_SRC) $(CLI_SRC)
OBJ   := $(SRC:.c=.o)
EXEC  := domino
LIBS  := -lncurses

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

