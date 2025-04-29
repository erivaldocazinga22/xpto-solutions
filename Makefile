# Definir o compilador
CC = gcc

# Definir as flags de compilação
CFLAGS = -Iinclude -Wall -std=c99

# Lista de arquivos fonte
SRC = src/main.c src/utils.c src/menu.c src/actions/menu-func-action.c src/actions/menu-empresa-action.c src/actions/menu-comp-action.c  src/actions/menu-posto-action.c src/actions/menu-opera-action.c

# Nome do executável
OUT = programa

# Regra principal para compilar o projeto
all:
	# Cria o executável a partir dos arquivos fonte
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

# Regra para executar o programa após compilação
run: all
	./$(OUT)

# Limpar os arquivos compilados (como o executável)
clean:
	rm -f $(OUT)