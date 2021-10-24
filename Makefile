EXECUTAVEL = teste

MAIN = ./main/main.c
INCLUDE = ./include
SRC = ./src
OBJ = ./obj
BIN = ./bin

FLAGS = -O3 -Wall -std=gnu99

all: library app

library:\
	$(OBJ)/red-and-black_tree.o

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc -c $< -I $(INCLUDE) $(FLAGS) -o $@

app:
	gcc $(MAIN) $(OBJ)/*.o $(FLAGS) -I $(INCLUDE) -o $(BIN)/$(EXECUTAVEL)

run:
	$(BIN)/$(EXECUTAVEL)

clearBin:
	rm $(BIN)/*

clearObj:
	rm $(OBJ)/*.o

purge:
	rm $(OBJ)/*.o
	rm $(BIN)/*