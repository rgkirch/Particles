#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "soa.h"

Position* init(int numberOfParticles, int width, int height) {
    //srand(time(NULL));
    //FILE* file = fopen("test", "w");
    //fprintf(file, "%f", rand()/(float)RAND_MAX);
	Velocity velocity;
	Position* position = (Position*)malloc(sizeof(Position));
    float* pirateArrays[] = {position->x, position->y, velocity.x, velocity.y};
    int dims[] = {width, height};

    //int lengthOfVels = sizeof(vels) / sizeof(float*) / numberOfParticles;
    int numPirates = sizeof(pirateArrays) / sizeof(float*);
    
    for(int i = 0; i < numPirates; ++i) {
        pirateArrays[i] = (float*)malloc(sizeof(float) * numberOfParticles);
    }
    for(int i = 0; i < numPirates / 2; ++i) {
        randomize( (float*)(&pirateArrays[i]), (float*)(&pirateArrays[i])+numberOfParticles );
        for(int v = 0; v < numberOfParticles; ++v) {
            //pirateArrays[i][v] *= dims[i];
            position->x[v] *= width;
            position->y[v] *= height;
        }
    }
    for(int i = numPirates / 2; i < numPirates; ++i) {
        for(int v = 0; v < numberOfParticles; ++v) {
            position->x[v] = (float)0.0;
            position->y[v] = (float)0.0;
        }
    }
    return position;
}

void randomize( float* iter, float* end ) {
    while(iter++ != end) {
		*iter = rand()/(RAND_MAX / 2.0) - 1.0;
	}
};

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
