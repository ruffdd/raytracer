CPPFLAGS += -I EasyBMP $(G)
CC = g++

all: raytracer

raytracer: main.o EasyBMP.o
	$(CC) $^ $(CPPFLAGS) -o $@

EasyBMP.o: EasyBMP/EasyBMP.cpp
	$(CC) -c $^ $(CPPFLAGS) -o $@

clear:
	rm -f *.o

clean: clear
	rm -f raytracer