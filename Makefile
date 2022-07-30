cflags = -Iimgui
lflags = -lglfw -lGLEW -lGLU -lGL

debug:
	g++ main.cpp -o run_debug ${cflags} ${lflags} -g -Wall

release:
	g++ main.cpp -o run ${cflags} ${lflags} -O3
