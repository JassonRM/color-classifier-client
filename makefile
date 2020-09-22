# Build color classifier client
CC = gcc

program: main.c
	$(CC) -o client main.c