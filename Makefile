
GCC= gcc -Wall

all: mapper
	
clean:
	rm -f *.o
	rm -f *.a
	rm -f *~
	
mapper: mapper.o split.o
	$(GCC) -o mapper mapper.o split.o -lpthread

mapper.o: mapper.c conf.h split.h
	$(GCC) -c mapper.c

split.o: split.c split.h
	$(GCC) -c split.c


