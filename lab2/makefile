CC= gcc
CFLAGS = -Wall -fopenmp -lm

depends = .depends

library = -lniceprint
headers = functions.h
source = $(headers:.h=.c) bomb.c
objects = $(source:.c=.o)
out = bomb

build: $(out)

$(out) : $(objects)
	$(CC)  -o $@ $^ $(library) $(CFLAGS) -L .  
$(objects) :
	$(CC)  -c -o $@ $*.c $(CFLAGS) $(library) -lm 
$(depends): $(source) $(headers)
	@$(CC) -MM $(source) > $@

clean:
	$(RM) $(out) $(objects) $(depends)

.PHONY: clean


run:

	./bomb -t 4 -N 35 -i test1_35.txt -o output1.txt -D 0


	
sinclude $(depends)