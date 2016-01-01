#ifndef SOA_H
#define SOA_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Position {
	GLfloat* x;
	GLfloat* y;
};

struct Velocity {
	GLfloat* x;
	GLfloat* y;
};

extern int numberOfParticles;
extern struct Position position;
extern struct Velocity velocity;

// dimension is like the width of the area
void particleInit(void);
void step(void);

#endif


// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
