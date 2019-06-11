CPPFLAGS += -I EasyBMP $(G) -std=c++17
LFLAGS += -lOpenCL
CC = g++

all: raytracer

raytracer: main.o EasyBMP.o
	$(CC) $^ $(CPPFLAGS) $(LFLAGS) -o $@

EasyBMP.o: EasyBMP/EasyBMP.cpp
	$(CC) -c $^ $(CPPFLAGS) -o $@

main.o: main.cpp helper.hpp

clear:
	rm -f *.o

clean: clear
	rm -f raytracer