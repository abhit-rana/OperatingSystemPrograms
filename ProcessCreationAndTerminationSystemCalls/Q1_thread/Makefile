thread: Q1t.o
	gcc -g -pthread Q1t.o -o thread

Q1t.o: Q1t.s
	gcc -c Q1t.s -o Q1t.o

Q1t.s: Q1t.c
	gcc -S Q1t.c -o Q1t.s
