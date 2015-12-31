#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "soa.h"

Position* init(int numberOfParticles, int width, int height) {
    //srand(time(NULL));
    //FILE* file = fopen("test", "w");
    //fprintf(file, "%f", rand()/(float)RAND_MAX);
    printf("width is %d\n", width);
	Velocity velocity;
	Position* position = (Position*)malloc(sizeof(Position));
    if(position == NULL) fprintf(stderr, "Failed to allocate memory for position.\nExiting.\n"), exit(1);
    float** pirateArrays[] = {&position->x, &position->y, &velocity.x, &velocity.y};
    int dims[] = {width, height};

    //int lengthOfVels = sizeof(vels) / sizeof(float*) / numberOfParticles;
    int numPirates = sizeof(pirateArrays) / sizeof(float*);
    
    for(int i = 0; i < numPirates; ++i) {
        *pirateArrays[i] = (float*)malloc(sizeof(float) * numberOfParticles);
        if(pirateArrays[i] == NULL) fprintf(stderr, "Failed to allocate memory for pirateArrays.\nExiting.\n"), exit(1);
        //printf("pirateArrays[i] = %p", pirateArrays[i]);
    }
    for(int i = 0; i < numPirates / 2; ++i) {
        randomize( (*pirateArrays[i]), (*pirateArrays[i])+numberOfParticles );
        for(int v = 0; v < numberOfParticles; ++v) {
            (*pirateArrays[i])[v] *= dims[i];
            //position->x[v] *= width;
            //position->y[v] *= height;
        }
    }
    for(int i = numPirates / 2; i < numPirates; ++i) {
        for(int v = 0; v < numberOfParticles; ++v) {
            velocity.x[v] = (float)0.0;
            velocity.y[v] = (float)0.0;
        }
    }
    return position;
}

void randomize( float* iter, float* end ) {
    while(iter != end) {
		*iter = rand()/(RAND_MAX / 2.0) - 1.0;
        ++iter;
	}
};

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
