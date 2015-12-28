target = 
GLEW = -lGLEW -lGLU -lGL
GLFW = -lglfw
INCLUDE = -I/usr/include
INCCLUDE_GLM = -I/usr/local/include
DEBUG = -g

app.exe: $(target)
	g++ $(DEBUG) -o app.exe $(target) $(INCLUDE) $(INCLUDE_STB) $(INCCLUDE_GLM)  $(GLFW) $(GLEW)

soa.out: soa.cpp
	g++ -std=c++11 -o soa.out soa.cpp
