COMPILADOR= g++

MAIN= main.cpp

TEXTURA= Texture.cpp Texture.h

FLAGS_COMPILADOR= -w

LINKER_FLAGS= -lSDL2 -lSDL2_image

NOME_EXEC= prog

OBJS=  texture main #limpa

all: prog.o limpa

prog.o: textura.o main.o
	$(COMPILADOR) main.o Texture.o $(FLAGS_COMPILADOR) $(LINKER_FLAGS) -o $(NOME_EXEC)

main.o: 
	$(COMPILADOR) $(MAIN) -c

textura.o:
	$(COMPILADOR) Texture.cpp -c

limpa: 
	rm *.o
