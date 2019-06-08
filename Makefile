CPPFLAGS += -I EasyBMP $(G)
LFLAGS += -lOpenCL
CC = g++

all: raytracer

raytracer: main.o EasyBMP.o
	$(CC) $^ $(CPPFLAGS) $(LFLAGS) -o $@

EasyBMP.o: EasyBMP/EasyBMP.cpp
	$(CC) -c $^ $(CPPFLAGS) -o $@

clear:
	rm -f *.o

clean: clear
	rm -f raytracer