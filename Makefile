cflags = -Iimgui
lflags = -lglfw -lGLEW -lGLU -lGL
deps = shader.cpp loadTexture.cpp controls.cpp objloader.cpp
headers = shader.hpp loadTexture.hpp controls.hpp objloader.hpp

debug:
	make clean
	g++ main.cpp ${deps} -o run_debug ${cflags} ${lflags} -g -Wall
	./run_debug

release:
	g++ main.cpp ${deps} -o run ${cflags} ${lflags} -O3

clean:
	rm -f run_debug run
