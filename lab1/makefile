CC= gcc
CFLAGS = -Wall -msse3

depends = .depends

headers = functions.h
source = $(headers:.h=.c) hough.c
objects = $(source:.c=.o)
out = hough

build: $(out)

$(out) : $(objects)
	$(CC)  -o $@ $^ $(CFLAGS) -lm  
$(objects) :
	$(CC)  -c -o $@ $*.c $(CFLAGS) -lm 
$(depends): $(source) $(headers)
	@$(CC) -MM $(source) > $@

clean:
	$(RM) $(out) $(obj) $(depends)

.PHONY: clean


run:
	./hough -i simplehough1-256x256.raw -o hough1.raw -M 256 -N 256 -T 180 -R 1000 -U 1
	
	
sinclude $(depends)