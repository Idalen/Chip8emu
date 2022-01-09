all: main.o cpu.o platform.o
	g++ main.o cpu.o platform.o -o exe `sdl2-config --cflags --libs`
main.o:
	g++ source/main.cpp -c
cpu.o:
	g++ source/cpu.cpp -c
platform.o:
	g++ source/platform.cpp -c `sdl2-config --cflags --libs`
run:
	./exe
clean:
	rm -rf *.o