gltut5: gltut5.o loadShaders.o
	clang++ -g2 -o gltut5 -lglfw -lGLEW -lGL gltut5.o loadShaders.o

gltut5.o: gltut5.cpp cube_array.h
	clang++ -g2 -c gltut5.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

clean:
	rm -f gltut5 gltut5.o loadShaders.o
