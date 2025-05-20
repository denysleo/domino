CC := gcc
CFLAGS := -Wall -I. -I./include -I./cli-lib/include
LIBS := -lncurses

PROJECT_SRC := \
    jogador.c \
    jogo.c \
    pedra.c \
    src/main.c

CLI_SRC := \
    cli-lib/src/screen.c \
    cli-lib/src/keyboard.c \
    cli-lib/src/timer.c

SRC := $(PROJECT_SRC) $(CLI_SRC)
OBJ := $(SRC:.c=.o)
EXEC := domino

all: $(EXEC)

# Regra para gerar .o a partir de .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Linkando os objetos para criar o executável
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

clean:
	rm -f $(OBJ) $(EXEC)

