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
	$(RM) $(out) $(objects) $(depends)

.PHONY: clean


run:

	./hough -i simplehough1-256x256.raw -o hough1.raw -M 256 -N 256 -T 511 -R 513 -U 1
	./hough -i simplehough2-608x402.raw -o hough2.raw -M 608 -N 402 -T 511 -R 513 -U 180
	./hough -i circuit1-280x272.raw -o hough3.raw -M 280 -N 272 -T 511 -R 513 -U 12


	
sinclude $(depends)