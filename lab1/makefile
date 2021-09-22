CC= gcc
CFLAGS = -Wall 

depends = .depends

headers = functions.h
source = $(headers:.h=.c) hough.c
objects = $(source:.c=.o)
out = hough

build: $(out)

$(out) : $(objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c

$(depends): $(source) $(headers)
	@$(CC) -MM $(source) > $@

clean:
	$(RM) $(out) $(obj) $(depends)

.PHONY: clean


run:
	./hough -i simplehough1-256x256.raw -o hough1.raw -M 256 -N 256 -T 180 -R 1000 -U 20
	./hough -i circuit1-280x272.raw -o circuit1.raw -M 272 -N 280 -T 180 -R 1000 -U 3

sinclude $(depends)