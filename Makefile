CC = g++
CFLAGS= -g -std=c++11

default: final

final: main.o HTTPBot.o Response.o cURLH.o
	$(CC) $(CFLAGS) -o main main.o HTTPBot.o Response.o cURLH.o -lcurl

HTTPBot.o: HTTPBot.cpp HTTPBot.h 
	$(CC) $(CFLAGS) -c HTTPBot.cpp

cURLH.o:   cURLH.cpp cURLH.h
	$(CC) $(CFLAGS) -c cURLH.cpp

Response.o: Response.cpp Response.h 
	$(CC) $(CFLAGS) -c Response.cpp

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp

clean: 
	$(RM) count *.o *~ main
