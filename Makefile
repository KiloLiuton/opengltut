cflags = -Iimgui
lflags = -lglfw -lGLEW -lGLU -lGL
deps = shader.cpp loadTexture.cpp controls.cpp
headers = shader.hpp loadTexture.hpp controls.hpp

debug:
	make clean
	g++ main.cpp ${deps} -o run_debug ${cflags} ${lflags} -g -Wall
	./run_debug

release:
	g++ main.cpp ${deps} ${headers} shader.cpp -o run ${cflags} ${lflags} -O3

clean:
	rm -f run_debug run
