gltut5: gltut5.o loadShaders.o texture.o bmp_texture.o dds_texture.o
	clang++ -g2 -o gltut5 -lglfw -lGLEW -lGL gltut5.o loadShaders.o texture.o \
		bmp_texture.o dds_texture.o

gltut5.o: gltut5.cpp cube_array.h texture.h
	clang++ -g2 -c gltut5.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

texture.o: texture.cpp texture.h
	clang++ -g2 -c texture.cpp

bmp_texture.o: bmp_texture.cpp texture.h
	clang++ -g2 -c bmp_texture.cpp

dds_texture.o: dds_texture.cpp texture.h fourcc_codes.h
	clang++ -g2 -c dds_texture.cpp

clean:
	rm -f gltut5 gltut5.o loadShaders.o texture.o bmp_texture.o dds_texture.o
