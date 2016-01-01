#! /bin/sh
g++ -std=c++11 -g -o app.exe visualize.cpp soa.c -I/usr/include  -I/usr/local/include  -lglfw -lGLEW -lGLU -lGL
./app.exe 10000
