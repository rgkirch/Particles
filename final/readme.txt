mpi-1dSplitWithBins.cpp
this is the final submission
grade of 88 from the autograder
does a 1 dimensional split and divedis the data among the processors via columns
the particles in each column are then placed into bins and the apply force is called on all of the particles in all of the bins around the current bin plus the current bin for each of the particles in the current bin


mpi-ring.cpp
diveds the data among the processors and they all trade the data around in a ring like fassion
every particle sees every other particle
needs binning
