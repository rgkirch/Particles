#include <stdio.h>
//#include <vector>
#include <random>

#define numberOfParticles 1000

using namespace std;

random_device rd;
default_random_engine generator( rd() );
uniform_real_distribution<float> distribution(-1.0, 1.0);

struct Velocity {
	float x[numberOfParticles];
	float y[numberOfParticles];
	float z[numberOfParticles];
};
struct Position {
	float x[numberOfParticles];
	float y[numberOfParticles];
	float z[numberOfParticles];
};

void initialize( float* begin, float* end) {
	for( float* iter = begin; iter != end; ++iter) {
		*iter = distribution( generator );
		//printf( "%f\n", *iter );
	}
};

int main() {
	Velocity velocity;
	Position position;
	initialize( (float*)(&velocity), (float*)(&velocity)+sizeof(velocity)/sizeof(float) );
	initialize( (float*)(&position), (float*)(&position)+sizeof(position)/sizeof(float) );
	return 0;
}

// thoughts - broadphase among x's, if x's are far enough apart, don't compare y's or z's
// keep track of fastest particle, if the fastest particle couldn't cross half partition size since last repartition then safe to not repartition
