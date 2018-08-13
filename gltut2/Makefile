gltut2: gltut2.o loadShaders.o
	clang++ -g2 -o gltut2 -lglfw -lGLEW -lGL gltut2.o loadShaders.o

gltut2.o: gltut2.cpp
	clang++ -g2 -c gltut2.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

clean:
	rm -f gltut2 gltut2.o loadShaders.o
