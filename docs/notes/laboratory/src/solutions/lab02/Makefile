#
# Makefile
#

target :
	gcc -Wall -o e01A e01A.c -lpthread
	gcc -Wall -o e01B e01B.c -lpthread
	gcc -Wall -o e02 e02.c   -lpthread


clean :
	rm -f *.o
	rm -rf e01A e01B e02

distclean : clean
	rm -rf core
	rm -rf *~
