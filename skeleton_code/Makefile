CC=gcc
CFLAGS=-c
CMATH=-lm

all: clean myOS

myOS: myOS.o shell.o helper.o commandParser.o
	$(CC) -o $@ $^ $(CMATH)

myOS.o: myOS.c myOS.h
	$(CC) $(CFLAGS) -o $@ $<

shell.o: shell.c shell.h commandParser.c commandParser.h
	$(CC) $(CFLAGS) -o $@ $<

helper.o: helper.c helper.h
	$(CC) $(CFLAGS) -o $@ $<

commandParser.o: commandParser.c commandParser.h

test: clean myOS
	bash ./tests/run.sh

clean:
	rm -f *.o myOS report.txt
