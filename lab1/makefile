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
	./hough -i simplehough1-256x256.raw -o hough2.raw -M 256 -N 256 -T 512 -R 512 -U 2
	./hough -i simplehough1-256x256.raw -o hough3.raw -M 256 -N 256 -T 512 -R 512 -U 3
	./hough -i simplehough1-256x256.raw -o hough4.raw -M 256 -N 256 -T 512 -R 512 -U 4
	./hough -i simplehough1-256x256.raw -o hough5.raw -M 256 -N 256 -T 512 -R 512 -U 5
	./hough -i simplehough1-256x256.raw -o hough6.raw -M 256 -N 256 -T 512 -R 512 -U 6
	./hough -i simplehough2-608x402.raw -o hough7.raw -M 608 -N 402 -T 512 -R 1024 -U 120
	
	
sinclude $(depends)