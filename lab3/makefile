CC= nvcc

depends = .depends

headers = functions.h
source = $(headers:.h=.cu) hist.cu
objects = $(source:.cu=.o)
out = hist

build: $(out)

$(out) : $(objects)
	$(CC)  -o $@ $^ 
$(objects) :
	$(CC)  -c $@ $*.cu -arch all-major
$(depends): $(source) $(headers)
	@$(CC) -MM $(source) > $@

clean:
	$(RM) $(out) $(objects) $(depends)

.PHONY: clean


run:

	./hist -i img1lab3-1151x976.raw -m 1151 -n 976 -o out.txt -t 200 -d 0

	
sinclude $(depends)