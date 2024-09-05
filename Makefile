COMPILER_FLAGS=--std=c89 -Wall
COMPILER=gcc

%.o: %.c
	$(COMPILER) $(COMPILER_FLAGS) $^ -c

connect_four: main.c ui.o
	$(COMPILER) $^ -o $@
