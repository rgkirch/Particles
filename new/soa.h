#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <vector>

#define DEFAULTNUMBEROFPARTICLES 500
#define PI 3.1415926

typedef struct {
	float* x;
	float* y;
	float* z;
} Velocity;

typedef struct {
	float* x;
	float* y;
	float* z;
} Position;

void randomize( float* beginingOfArray, float* endOfArray );
Velocity* velocity_constructor( void );
Position* position_constructor( void );

int main(int argc, char** argv) {
    //srand(time(NULL));
    //FILE* file = fopen("test", "w");
    //fprintf(file, "%f", rand()/(float)RAND_MAX);
    int numberOfParticles;
    //scanf("%d", &num);
    if( argc > 1 ) {
        numberOfParticles = atoi(argv[1]);
    } else {
        numberOfParticles = DEFAULTNUMBEROFPARTICLES;
    }

	Velocity velocity;
	Position position;
    for(int i = 0; i < 3; ++i) {
        randomize( (float*)(&velocity), (float*)(&velocity)+sizeof(velocity)/sizeof(float) );
    }

	return 0;
}

void randomize( float* begin, float* end ) {
	for( float* iter = begin; iter != end; ++iter ) {
		*iter = rand()/(float)RAND_MAX;
	}
};

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
