
.PHONY: all

all: \
	$(patsubst %.c, %, $(wildcard *.c)) \
	$(patsubst %.cpp, %, $(wildcard *.cpp))

%: %.c
	gcc -Wall -O3 $< -o $@

%: %.cpp
	g++ -Wall -O3 $< -o $@

xinfo: xinfo.c
	gcc -Wall -O3 -L/usr/X11R6/lib/ -lX11 -lXext -lXss $< -o $@
