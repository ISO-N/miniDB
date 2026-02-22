program: main.o db.o io.o utils.o
	gcc -o program.exe main.o db.o io.o utils.o

main.o: main.c db.h utils.h config.h
	gcc -c main.c

db.o: db.c db.h utils.h config.h
	gcc -c db.c

io.o: io.c io.h db.h config.h
	gcc -c io.c

utils.o: utils.c utils.h config.h
	gcc -c utils.c

.PHONY: clean
clean:
	-del /Q *.o program.exe 2>NUL
