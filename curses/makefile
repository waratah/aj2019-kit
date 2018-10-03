LIBS=-lasan -lncurses 
# LIBS=-Plncurses 
CC=gcc
LINK=-Wl,-Map,output.map
CFLAGS:=${CFLAGS} -DLINUX -fsanitize=address -fno-omit-frame-pointer
# CFLAGS:=${CFLAGS} -DLINUX -ggdb
OBJECTS=main.o led_curses.o random.o machine.o flashValue.o director_curses.o game.o connection-file.o

OBJECTS_HARDWARE=director.o led.o connection.o


game : ${OBJECTS}
	${CC} -o $@ ${OBJECTS} ${LIBS} ${LINK}

main.o : main.c  ../gamecode/led.h ../gamecode/random.h ../gamecode/machine.h ../gamecode/director.h

led_curses.o : led_curses.c ../gamecode/led.h

led.o : ../gamecode/led.c ../gamecode/led.h

random.o : ../gamecode/random.c ../gamecode/random.h
	${CC} -c -o random.o ${CFLAGS} ../gamecode/random.c

flashValue.o : ../gamecode/flashValue.c ../gamecode/flashValue.h ../gamecode/led.h ../gamecode/machine.h
	${CC} -c -o flashValue.o ${CFLAGS} ../gamecode/flashValue.c

machine.o : ../gamecode/machine.c ../gamecode/machine.h ../gamecode/led.h
	${CC} -c -o machine.o ${CFLAGS} ../gamecode/machine.c

director_curses.o: director_curses.c ../gamecode/director.h ../gamecode/led.h ../gamecode/game.h led_curses.h

director.o: ../gamecode/director.c ../gamecode/director.h ../gamecode/led.h ../gamecode/game.h

game.o: ../gamecode/game.c ../gamecode/game.h ../gamecode/led.h ../gamecode/random.h ../gamecode/machine.h
	${CC} -c -o game.o ${CFLAGS} ../gamecode/game.c

connection-file.o: connection-file.c ../gamecode/connection.h connection-file.h led_curses.h ../gamecode/random.h

connection.o: ../gamecode/connection.c ../gamecode/connection.h
	${CC} -c -o connection.o ${CFLAGS} ../gamecode/connection.c

.PHONY: clean all
clean:
	rm game ${OBJECTS} output.map

all: game director.o connection.o director.o

