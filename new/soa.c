#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "soa.h"

Position* init(int numberOfParticles) {
	Velocity velocity;
	Position* position = (Position*)malloc(sizeof(Position));
    if(position == NULL) fprintf(stderr, "Failed to allocate memory for position.\nExiting.\n"), exit(1);
    GLfloat** pirateArrays[] = {&position->x, &position->y, &velocity.x, &velocity.y};

    //int lengthOfVels = sizeof(vels) / sizeof(GLfloat*) / numberOfParticles;
    int numPirates = sizeof(pirateArrays) / sizeof(GLfloat*);
    
    for(int i = 0; i < numPirates; ++i) {
        *pirateArrays[i] = (GLfloat*)malloc(sizeof(GLfloat) * numberOfParticles);
        if(pirateArrays[i] == NULL) fprintf(stderr, "Failed to allocate memory for pirateArrays.\nExiting.\n"), exit(1);
        //printf("pirateArrays[i] = %p", pirateArrays[i]);
    }
    for(int i = 0; i < numPirates / 2; ++i) {
        randomize( (*pirateArrays[i]), (*pirateArrays[i])+numberOfParticles );
    }
    for(int i = numPirates / 2; i < numPirates; ++i) {
        for(int v = 0; v < numberOfParticles; ++v) {
            velocity.x[v] = (GLfloat)0.0;
            velocity.y[v] = (GLfloat)0.0;
        }
    }
    return position;
}

void randomize( GLfloat* iter, GLfloat* end ) {
    while(iter != end) {
		*iter = rand()/(RAND_MAX / 2.0) - 1.0;
        //printf("%.2f, ", *iter);
        ++iter;
	}
};

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
