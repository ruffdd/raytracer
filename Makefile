CPPFLAGS += -I EasyBMP $(G) $(DEFINED) -std=c++17 -I/usr/include -I./include -I./EasyBMP
LFLAGS += -lOpenCL
VPATH= src EasyBMP

all: bin obj bin/raytracer bin/mainkernel.cl

bin/raytracer: obj/main.o obj/EasyBMP.o
	$(CXX) $^ $(CPPFLAGS) $(LFLAGS) -o $@

bin/mainkernel.cl: src/mainkernel.cl
	gcc -fsyntax-only src/mainkernel.cl
	cp src/mainkernel.cl bin/

obj/EasyBMP.o: EasyBMP.cpp
	$(CXX) -c $^ $(CPPFLAGS) -o $@

obj/main.o: main.cpp
	$(CXX) -c $^ $(CPPFLAGS) -o $@

bin:
	mkdir bin

obj: 
	mkdir obj

clear:
	rm -rf obj/

clean: clear
	rm -rf bin