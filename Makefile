cflags = -Iimgui
lflags = -lglfw -lGLEW -lGLU -lGL
deps = shader.cpp loadTexture.cpp
headers = shader.hpp loadTexture.hpp

debug:
	g++ main.cpp ${deps} -o run_debug ${cflags} ${lflags} -g -Wall

release:
	g++ main.cpp ${deps} ${headers} shader.cpp -o run ${cflags} ${lflags} -O3

clean:
	rm -f run_debug run
