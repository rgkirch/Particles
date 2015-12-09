#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <vector>

#define numberOfParticles 1000
#define PI 3.1415926

typedef struct {
	float x[numberOfParticles];
	float y[numberOfParticles];
	float z[numberOfParticles];
} Velocity;

typedef struct {
	float x[numberOfParticles];
	float y[numberOfParticles];
	float z[numberOfParticles];
} Position;

void initialize( float* begin, float* end ) {
	for( float* iter = begin; iter != end; ++iter ) {
		*iter = rand()/(float)RAND_MAX;
	}
};

int main() {
    //srand(time(NULL));
	Velocity velocity;
	Position position;
	initialize( (float*)(&velocity), (float*)(&velocity)+sizeof(velocity)/sizeof(float) );
	initialize( (float*)(&position), (float*)(&position)+sizeof(position)/sizeof(float) );

	return 0;
}

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
