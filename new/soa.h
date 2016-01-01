#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	GLfloat* x;
	GLfloat* y;
} Position;

typedef struct {
	GLfloat* x;
	GLfloat* y;
} Velocity;

void randomize( GLfloat* beginingOfArray, GLfloat* endOfArray );
// dimension is like the width of the area
Position* init(int numberOfParticles);


// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
