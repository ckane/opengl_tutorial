gltut3: gltut3.o loadShaders.o
	clang++ -g2 -o gltut3 -lglfw -lGLEW -lGL gltut3.o loadShaders.o

gltut3.o: gltut3.cpp
	clang++ -g2 -c gltut3.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

clean:
	rm -f gltut3 gltut3.o loadShaders.o
