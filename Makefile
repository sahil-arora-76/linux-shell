FLAGS=-Wall

COMPILER=gcc

OBJ=terminal.o

FILES=main.c

build/$(OBJ): src/$(FILES)
	$(COMPILER) -o $@ $< $(FLAGS)
clean: 
	rm -rf build/.*o build/$(OBJ)