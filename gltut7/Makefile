gltut7: gltut7.o loadShaders.o texture.o bmp_texture.o dds_texture.o controls.o \
	    model.o
	clang++ -g2 -o gltut7 -lglfw -lGLEW -lGL gltut7.o loadShaders.o texture.o \
		bmp_texture.o dds_texture.o controls.o model.o

gltut7.o: gltut7.cpp cube_array.h texture.h model.h controls.h
	clang++ -g2 -c gltut7.cpp

loadShaders.o: loadShaders.cpp
	clang++ -g2 -c loadShaders.cpp

texture.o: texture.cpp texture.h
	clang++ -g2 -c texture.cpp

bmp_texture.o: bmp_texture.cpp texture.h
	clang++ -g2 -c bmp_texture.cpp

dds_texture.o: dds_texture.cpp texture.h fourcc_codes.h
	clang++ -g2 -c dds_texture.cpp

controls.o: controls.cpp controls.h
	clang++ -g2 -c controls.cpp

model.o: model.cpp model.h
	clang++ -g2 -c model.cpp

clean:
	rm -f gltut7 gltut7.o loadShaders.o texture.o bmp_texture.o dds_texture.o
