#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "soa.h"

int numberOfParticles;
unsigned int screenWidth;
unsigned int screenHeight;
struct Position position;
struct Velocity velocity;
float timeStep;
void randomize( GLfloat* beginingOfArray, GLfloat* endOfArray );
unsigned int totalNumberOfCollisions = 0;
unsigned int closeCalls = 0;

void step() {
    timeStep = 0xFFFFFFFF;
    for(int i = 0; i < numberOfParticles; ++i) {
        for(int k = i+1; k < numberOfParticles; ++k) {
            float xp1, yp1, xv1, yv1, xp2, yp2, xv2, yv2;
            xp1 = position.x[i];
            yp1 = position.y[i];
            xv1 = velocity.x[i];
            yv1 = velocity.y[i];
            xp2 = position.x[k];
            yp2 = position.y[k];
            xv2 = velocity.x[k];
            yv2 = velocity.y[k];
            float b1 = position.y[i] - velocity.y[i] / velocity.x[i] * position.x[i];
            float b2 = position.y[k] - velocity.y[k] / velocity.x[k] * position.x[k];
            float x = (b1 - b2) / (yv2 / xv2 - yv1 / xv1);
            float y = yv1 / xv1 * x + b1;
            float t1 = (x - xp1) / xv1;
            if(t1 < 0) break; // no collision
            float t2 = (x - xp2) / xv2;
            if(t2 < 0) break; // no collision
            float t3 = t2 - t1;
            if(t3 == 0) {
                ++totalNumberOfCollisions;
                timeStep = timeStep < t3 ? timeStep : t3;
            } else if(fabs(t3) < 0.01f) {
                ++closeCalls;
            }
        }
    }
    for(int i = 0; i < numberOfParticles; ++i) {
        position.x[i] += velocity.x[i]/50;
        position.y[i] += velocity.y[i]/50;
    }
    printf("collisions %d\tclose %d\n", totalNumberOfCollisions, closeCalls);
}

void particleInit() {
	//position = (Position*)malloc(sizeof(Position));
	//velocity = (Velocity*)malloc(sizeof(Velocity));
    //if(position == NULL) fprintf(stderr, "Failed to allocate memory for position.\nExiting.\n"), exit(1);
    GLfloat** pirateArrays[] = {&position.x, &position.y, &velocity.x, &velocity.y};

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
            velocity.x[v] = (GLfloat)(rand()/(RAND_MAX / 2.0) - 1.0);
            velocity.y[v] = (GLfloat)(rand()/(RAND_MAX / 2.0) - 1.0);
        }
    }
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
