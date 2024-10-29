CC=gcc
CFLAGS = -Wall
DEPS = stack.h calcProtons.h
DEBUGFLAGS := -g -DDEBUG
TARGETS = parseFormula  stackDebug calcProtonsDebug

# It will make all the executables: parceFormula, stackDebug and calcProtonsDebug.
all: $(TARGETS)
# It will only create the executable for parseFormula.
parseFormula: parseFormula.c stack.c calcProtons.c
	$(CC) $(CFLAGS) -o parseFormula parseFormula.c stack.c calcProtons.c

# It will create the executable, used to debug the stack -> stackDebug.
stackDebug: stack.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o stackDebug stack.c 

# It will create the executable, used to debug the calcProtons ->calcProtons.
calcProtonsDebug: calcProtons.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o calcProtonsDebug calcProtons.c

# It will clean from all the executable files.
clean: 
	rm -f *.o $(TARGETS)
