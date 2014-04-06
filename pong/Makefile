#
# makefile for pong project
# 31-March-2014
# 

CC = gcc -Wall 


pong: pong.c paddle.o clocktimer.o
	$(CC) paddle.o clocktimer.o pong.c alarmlib.c -lcurses -o pong

paddle.o: paddle.c
	$(CC) -c paddle.c

clocktimer.o: clocktimer.c
	$(CC) -c clocktimer.c

clean:
	rm -f pong *.o