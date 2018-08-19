gltut6: gltut6.o loadShaders.o texture.o bmp_texture.o dds_texture.o
	clang++ -g2 -o gltut6 -lglfw -lGLEW -lGL gltut6.o loadShaders.o texture.o \
		bmp_texture.o dds_texture.o

gltut6.o: gltut6.cpp cube_array.h texture.h
	clang++ -g2 -c gltut6.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

texture.o: texture.cpp texture.h
	clang++ -g2 -c texture.cpp

bmp_textture.o: bmp_texture.cpp texture.h
	clang++ -g2 -c bmp_texture.cpp

dds_textture.o: dds_texture.cpp texture.h fourcc_codes.h
	clang++ -g2 -c dds_texture.cpp

clean:
	rm -f gltut6 gltut6.o loadShaders.o texture.o bmp_texture.o dds_texture.o
