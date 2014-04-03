#
# makefile for pong project
# 31-March-2014
# 

CC = gcc -Wall 


pong: pong.c paddle.o
	$(CC) paddle.o pong.c alarmlib.c -lcurses -o pong

paddle.o: paddle.c
	$(CC) -c paddle.c 
clean:
	rm -f pong *.o