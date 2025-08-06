FILES = main.c
FLAGS = -Wall -Wextra -g 

all:
	gcc ${FILES} ${FLAGS} -o interpret 
