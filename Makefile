GCC_OPTIONS=-Wall -pedantic -I include
GL_OPTIONS=-lGLEW -lGL -lglut
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)

default: scara

scara: scara.cpp
	cd Common; make;
	cd ..;
	g++ $@.cpp Common/InitShader.o $(OPTIONS) -o $@  


all: scara  run clean

run: scara 
	./scara 


PHONY: all run

clean:
	rm scara
	rm Common/InitShader.o
