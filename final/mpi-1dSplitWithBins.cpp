// Richard Kirchofer

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include "common.h"

#define density 0.0005
#define cutoff  0.01

using namespace std;


int main( int argc, char **argv )
{
	int navg, nabsavg=0;
	double dmin, absmin=1.0,davg,absavg=0.0;
	double rdavg,rdmin;
	int rnavg;

	//	process command line parameters
	if( find_option( argc, argv, "-h" ) >= 0 )
	{
		printf( "Options:\n" );
		printf( "-h to see this help\n" );
		printf( "-n <int> to set the number of particles\n" );
		printf( "-o <filename> to specify the output file name\n" );
		printf( "-s <filename> to specify a summary file name\n" );
		printf( "-no turns off all correctness checks and particle output\n");
		return 0;
	}

	int n = read_int( argc, argv, "-n", 1000 );
	char *savename = read_string( argc, argv, "-o", NULL );
	char *sumname = read_string( argc, argv, "-s", NULL );

	//	set up MPI
	int n_proc, rank;
	MPI_Init( &argc, &argv );
	MPI_Comm_size( MPI_COMM_WORLD, &n_proc );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	//	allocate generic resources
	FILE *fsave = savename && rank == 0 ? fopen( savename, "w" ) : NULL;
	FILE *fsum = sumname && rank == 0 ? fopen ( sumname, "a" ) : NULL;


	particle_t *particles = (particle_t*) malloc( n * sizeof(particle_t) );
	particle_t *local = (particle_t*) malloc( n * sizeof(particle_t) );
	int leftLength = 0;
	int rightLength = 0;
	int nlocal = 0;
	double size = sqrt(density*n);
	if (size <= 0)
		size = 1;


	// int *partition_sizes = (int*) malloc( n_proc * sizeof(int) );
	// int *partition_offsets = (int*) malloc( (n_proc) * sizeof(int) );
	int partition_offsets[16];
	int partition_sizes[16];

	int rightNeighbor = (rank + 1) % n_proc;
	int leftNeighbor = (rank + n_proc - 1) % n_proc;

	MPI_Datatype PARTICLE;
	MPI_Type_contiguous( 6, MPI_DOUBLE, &PARTICLE );
	MPI_Type_commit( &PARTICLE );

	set_size( n );
	if( rank == 0 )
		init_particles( n, particles );

	MPI_Bcast(particles, n, PARTICLE, 0, MPI_COMM_WORLD);

	// vector< vector< particle_t* > > binsOrig;
	// for (int i = 0; i < n_proc; ++i)
		// binsOrig.push_back(vector <particle_t*> ());
	// vector< vector< particle_t* > > bins;

	double simulation_time = read_timer( );
	for( int step = 0; step < NSTEPS; step++ )
	//for( int step = 0; step < 1; step++ )
	{
		// printf("step: %d rank: %d\n", step, rank);
		// MPI_Barrier(MPI_COMM_WORLD);
		navg = 0;
		dmin = 1.0;
		davg = 0.0;

		for (int i = 0; i < n_proc; ++i)
		{
			partition_sizes[i] = 0;
		}

		nlocal = 0;
		for (int p=0; p < n; ++p)
		{
			int col = particles[p].x * n_proc / size;
			++partition_sizes[col];
			if (rank == col)
			{
				local[nlocal] = particles[p];
				++nlocal;
			}
		}
		// printf("rank: %d mem local: %p\n", rank, local);
		// printf("rank: %d nlocal: %d\n", rank, nlocal);
		// printf("rank: %d &l + 1: %p\n", rank, &local + 1);
		// printf("rank: %d mem l+n: %p\n", rank, local + nlocal);
		// printf("rank: %d mem l+(n*s): %p\n", rank, local + (nlocal * sizeof(particle_t)));

		if (n_proc == 1) {
			// if there is only one processor then there is not any binning
			// the apply force and move are still relevant and do occur down below
		} else {
			// if not the first or last processor
			if (rank < n_proc-1 && rank > 0) {
				// printf("rank: %d begin mid\n", rank);

				double sendLeftIfLower = (size / n_proc) * rank + cutoff;
				double sendRightIfHigher = (size / n_proc) * (rank + 1) - cutoff;
				int nSendRight = 0;
				int nSendLeft = 0;
				vector< particle_t* > sendRightVector;
				vector< particle_t* > sendLeftVector;
				for (int p = 0; p < nlocal; ++p) {
					if (local[p].x > sendRightIfHigher) {
						sendRightVector.push_back(&local[p]);
						++nSendRight;
					}
				}
				for (int p = 0; p < nlocal; ++p) {
					if (local[p].x < sendLeftIfLower) {
						sendLeftVector.push_back(&local[p]);
						++nSendLeft;
					}
				}
				particle_t *sendRightArray = (particle_t*) malloc( nSendRight * sizeof(particle_t) );
				particle_t *sendLeftArray = (particle_t*) malloc( nSendLeft * sizeof(particle_t) );
				for (int c = 0; c < nSendRight; ++c) {
					sendRightArray[c] = *sendRightVector[c];
				}
				for (int c = 0; c < nSendLeft; ++c) {
					sendLeftArray[c] = *sendLeftVector[c];
				}

				// --- send right --->
				MPI_Send(&nSendRight, 1, MPI_INT, rightNeighbor, 0, MPI_COMM_WORLD);
				MPI_Send(sendRightArray, nSendRight, PARTICLE, rightNeighbor, 0, MPI_COMM_WORLD);
				// printf("rank: %d send right\n", rank);

				// ---> recieve left ---
				MPI_Recv(&leftLength, 1, MPI_INT, leftNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// particle_t* thing = local + nlocal;
				MPI_Recv(local + nlocal, leftLength, PARTICLE, leftNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// printf("rank: %d recv left\n", rank);

				// <--- send left ---
				MPI_Send(&nSendLeft, 1, MPI_INT, leftNeighbor, 0, MPI_COMM_WORLD);
				MPI_Send(sendLeftArray, nSendLeft, PARTICLE, leftNeighbor, 0, MPI_COMM_WORLD);
				// printf("rank: %d send left\n", rank);

				// --- recieve right <---
				MPI_Recv(&rightLength, 1, MPI_INT, rightNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(local + nlocal + leftLength, rightLength, PARTICLE, rightNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				free(sendRightArray);
				free(sendLeftArray);
				// printf("rank: %d recv right\n", rank);

			} else if (rank == 0) {
				// printf("rank: %d begin left\n", rank);
				double sendRightIfHigher = (size / n_proc) * (rank + 1) - cutoff;
				int nSendRight = 0;
				vector< particle_t* > sendRightVector;
				for (int p = 0; p < nlocal; ++p) {
					if (local[p].x > sendRightIfHigher) {
						sendRightVector.push_back(&local[p]);
						++nSendRight;
					}
				}
				particle_t *sendRightArray = (particle_t*) malloc( nSendRight * sizeof(particle_t) );
				for (int c = 0; c < nSendRight; ++c) {
					sendRightArray[c] = *sendRightVector[c];
				}

				// --- send right --->
				MPI_Send(&nSendRight, 1, MPI_INT, rightNeighbor, 0, MPI_COMM_WORLD);
				MPI_Send(sendRightArray, nSendRight, PARTICLE, rightNeighbor, 0, MPI_COMM_WORLD);
				// printf("rank: %d send right\n", rank);

				// --- recieve right <---
				MPI_Recv(&rightLength, 1, MPI_INT, rightNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(local + nlocal, rightLength, PARTICLE, rightNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				free(sendRightArray);
				// printf("rank: %d recv right\n", rank);

			} else {
				// printf("rank: %d begin right\n", rank);
				double sendLeftIfLower = (size / n_proc) * rank + cutoff;
				int nSendLeft = 0;
				vector< particle_t* > sendLeftVector;
				for (int p = 0; p < nlocal; ++p) {
					if (local[p].x < sendLeftIfLower) {
						sendLeftVector.push_back(&local[p]);
						++nSendLeft;
					}
				}
				particle_t *sendLeftArray = (particle_t*) malloc( nSendLeft * sizeof(particle_t) );
				for (int c = 0; c < nSendLeft; ++c) {
					sendLeftArray[c] = *sendLeftVector[c];
				}

				// ---> recieve left ---
				MPI_Recv(&leftLength, 1, MPI_INT, leftNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// particle_t* thing = local + nlocal;
				MPI_Recv(local + nlocal, leftLength, PARTICLE, leftNeighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// printf("rank: %d recv left\n", rank);

				// <--- send left ---
				MPI_Send(&nSendLeft, 1, MPI_INT, leftNeighbor, 0, MPI_COMM_WORLD);
				MPI_Send(sendLeftArray, nSendLeft, PARTICLE, leftNeighbor, 0, MPI_COMM_WORLD);
				free(sendLeftArray);
				// printf("rank: %d send left\n", rank);
			}
		}
		// printf("%d after logic\n", rank);

		if( find_option( argc, argv, "-no" ) == -1 )
		  if( fsave && (step%SAVEFREQ) == 0 )
			save( fsave, n, particles );

		// begin bining 
		int newLocal = nlocal + leftLength + rightLength;
		int DIMEN = 0.3*((size/0.011) - sqrt(n));
		if (DIMEN <= 0)
			DIMEN = 1;
		// width and height describe how many bins can fit if they are 2*cutoff square
		/*
		double width = size/(float)n_proc/(2.0*cutoff)
		double height = size/(2.0*cutoff)
		printf("rank: %d newLocal: %d size: %d dimen: %d \n", rank, newLocal, size, DIMEN);
		*/
		int width, height;
		width = height = DIMEN;
		// width = height = size*10;


		// printf("rank: %d before bins\n", rank);
		vector< vector< particle_t* > > bins;
		// printf("rank: %d before reserve\n", rank);
		bins.reserve(width*height);
		// printf("rank: %d before push back particle_t* vector\n", rank);
		for (int i = 0; i < width*height; ++i)
			bins.push_back(vector <particle_t*> ());
		// how many columns are in local
		// the left most and right most have two while th erest have three
		int sections = 3;
		if (rank == 0 || rank == n_proc-1)
			sections = 2;

		// double leftBound = (size / n_proc) * rank - cutoff;
		// double rightBound = (size / n_proc) * (rank + 1) + cutoff;
		// if (rank == 0)
			// leftBound = 0;
		// if (rank == n_proc-1)
			// rightBound = size;
			
		// printf("rank: %d before real binning\n", rank);
		for (int p=0; p < newLocal; ++p)
		{
			int binx = (local[p].x)/size*width;
			int biny = (local[p].y)/size*height;

			bins[binx+biny*height].push_back(&local[p]);
			/*
			if (binx+biny>= bins.size())
				printf("rank: %d, %d >= %d\n", rank, binx+biny*DIMEN, bins.size());
			 */
		}

		vector < particle_t* > neigborParticles;
		for (int i = 0; i < bins.size(); ++i)
		{
			neigborParticles.clear();

			int binX = i % width;
			int binY = i / height;

			for (int j = binY-1; j < binY+2; ++j)
			{
				for (int k = binX-1; k < binX+2; ++k)
				{
					if (k >= 0 && k < width && j >= 0 && j < height)
						neigborParticles.insert(neigborParticles.end(), bins[k+j*height].begin(), bins[k+j*height].end());
				}
			}

			// bins[i] is current bin being looked at
			for (int j = 0; j < bins[i].size(); ++j)
			{
				bins[i][j]->ax = bins[i][j]->ay = 0;
				for (int k = 0; k < neigborParticles.size(); ++k)
				{
					apply_force(*bins[i][j],*neigborParticles[k],&dmin,&davg,&navg);
				}
			}
		}
		// end binning 
		
		// printf("%d before apply\n", rank);
		/*
		for( int i = 0; i < nlocal; ++i )
		{
			local[i].ax = local[i].ay = 0;
			for (int j = 0; j < nlocal + leftLength + rightLength; ++j )
				apply_force( local[i], local[j], &dmin, &davg, &navg );
		}
		*/

		// printf("%d before reduce\n", rank);

		if( find_option( argc, argv, "-no" ) == -1 )
		{
			MPI_Reduce(&davg,&rdavg,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
			MPI_Reduce(&navg,&rnavg,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
			MPI_Reduce(&dmin,&rdmin,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
			if (rank == 0)
			{
				if (rnavg)
				{
					absavg +=  rdavg/rnavg;
					nabsavg++;
				}
				if (rdmin < absmin) absmin = rdmin;
			}
		}

		// printf("%d before move\n", rank);

		//	move particles
		for( int i = 0; i < nlocal; i++ )
		{
			move( local[i] );
		}

		int offset = 0;
		for (int i = 0; i < n_proc; ++i)
		{
			partition_offsets[i] = offset;
			offset += partition_sizes[i];
		}
		offset = partition_offsets[rank];
		// printf("%d offset %d\n", rank, offset);
		// printf("%d before other all gather\n", rank);
		// MPI_Barrier(MPI_COMM_WORLD);
		MPI_Allgatherv( local, nlocal, PARTICLE, particles, partition_sizes, partition_offsets, PARTICLE, MPI_COMM_WORLD );
		// MPI_Barrier(MPI_COMM_WORLD);
		// printf("rank: %d end\n", rank);
	}
	simulation_time = read_timer( ) - simulation_time;

	if (rank == 0) {
	  printf( "n = %d, simulation time = %g seconds", n, simulation_time);

	  if( find_option( argc, argv, "-no" ) == -1 )
	  {
		if (nabsavg) absavg /= nabsavg;
	  //
	  //  -the minimum distance absmin between 2 particles during the run of the simulation
	  //  -A Correct simulation will have particles stay at greater than 0.4 (of cutoff) with typical values between .7-.8
	  //  -A simulation were particles don't interact correctly will be less than 0.4 (of cutoff) with typical values between .01-.05
	  //
	  //  -The average distance absavg is ~.95 when most particles are interacting correctly and ~.66 when no particles are interacting
	  //
	  printf( ", absmin = %lf, absavg = %lf", absmin, absavg);
	  if (absmin < 0.4) printf ("\nThe minimum distance is below 0.4 meaning that some particle is not interacting");
	  if (absavg < 0.8) printf ("\nThe average distance is below 0.8 meaning that most particles are not interacting");
	  }
	  printf("\n");

	  //
	  // Printing summary data
	  //
	  if( fsum)
		fprintf(fsum,"%d %d %g\n",n,n_proc,simulation_time);
	}

	//
	//	release resources
	//
	if ( fsum )
		fclose( fsum );
   // at the moment 'offsets' and 'sizes' are not dynamically allocated
   // free( partition_offsets );
   // free( partition_sizes );
	free( local );
	free( particles );
	if( fsave )
		fclose( fsave );

	MPI_Finalize( );

	// printf("finish\n");

	return 0;
}
