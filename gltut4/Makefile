gltut4: gltut4.o loadShaders.o
	clang++ -g2 -o gltut4 -lglfw -lGLEW -lGL gltut4.o loadShaders.o

gltut4.o: gltut4.cpp cube_array.h
	clang++ -g2 -c gltut4.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

clean:
	rm -f gltut4 gltut4.o loadShaders.o
