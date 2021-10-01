CC= gcc
CFLAGS = -Wall -fopenmp

depends = .depends

headers = functions.h
source = $(headers:.h=.c) bomb.c
objects = $(source:.c=.o)
out = bomb

build: $(out)

$(out) : $(objects)
	$(CC)  -o $@ $^ $(CFLAGS) -lm  
$(objects) :
	$(CC)  -c -o $@ $*.c $(CFLAGS) -lm 
$(depends): $(source) $(headers)
	@$(CC) -MM $(source) > $@

clean:
	$(RM) $(out) $(objects) $(depends)

.PHONY: clean


run:

	./bomb -t 4 -N 35 -i input.txt -o output.txt -D


	
sinclude $(depends)