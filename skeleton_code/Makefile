CC=gcc
CFLAGS=-c
CMATH=-lm

all: clean myOS

myOS: myOS.o shell.o helper.o commandParser.o array.o processCommand.o
	$(CC) -o $@ $^ $(CMATH)

myOS.o: myOS.c myOS.h
	$(CC) $(CFLAGS) -o $@ $<

shell.o: shell.c shell.h commandParser.c commandParser.h array.c array.h processCommand.c processCommand.h
	$(CC) $(CFLAGS) -o $@ $<

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -o $@ $<

commandParser.o: commandParser.c commandParser.h

processCommand.o: processCommand.c processCommand.h

array.o: array.c array.h

test: clean myOS
	bash ./tests/run.sh

clean:
	rm -f *.o myOS report.txt
