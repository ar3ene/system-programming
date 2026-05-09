#
# Makefile
#

target :
	gcc -Wall -o e04 e04.c
	gcc -Wall -o e05A e05A.c
	gcc -Wall -o e05B e05B.c
	gcc -Wall -o e06 e06.c
	gcc -Wall -o e07 e07.c -lpthread
	gcc -Wall -o e08 e08.c -lpthread


clean :
	rm -f *.o
	rm -rf e04 e05A e05B e06 e07 e08

distclean : clean
	rm -rf core
	rm -rf *~
