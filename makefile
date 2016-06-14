server: server.o slist.o
	gcc server.o slist.o -o server
server.o: server.c slist.h
	gcc -Wall -c server.c
table.o: table.c table.h
	gcc -Wall -c slist.c
clean:	
	rm server.o slist.o server
