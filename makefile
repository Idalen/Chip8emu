all: main.o cpu.o platform.o
	g++ main.o cpu.o platform.o -o exe -lSDL2main -lSDL2
main.o:
	g++ source/main.cpp -c -lSDL2main -lSDL2
cpu.o:
	g++ source/cpu.cpp -c -lSDL2main -lSDL2
platform.o:
	g++ source/platform.cpp -c -lSDL2main -lSDL2
run:
	./exe
clean:
	rm -rf *.o