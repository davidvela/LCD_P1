#/* 
# * Author: David Vela Tirado
# * Software Developer
# * email:  david.vela.tirado@gmail.com
# * 
# */

PHONY: clean
CFLAGS= -g -Wall -c
all: servidor cliente

servidor: servidor.c
	gcc servidor.c   -o servidor


cliente: cliente.c
	gcc cliente.c -o cliente

clean: 
	-rm -f *.o
	-rm -f cliente servidor
	
