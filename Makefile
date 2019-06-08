srcdir = src
CC = g++


raytracer: main.o 

%.o: src/%.cpp

clear:
	rm -f *.o

clean: clear
	rm -f raytracer