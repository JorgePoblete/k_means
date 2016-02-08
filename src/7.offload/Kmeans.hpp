#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cmath>
#include <omp.h>
#include "utils.hpp"

#include <offload.h>
#define ALLOC alloc_if(1) free_if(0)
#define FREE alloc_if(0) free_if(1)
#define REUSE alloc_if(0) free_if(0)
#define MIC_THREAD 0

#ifndef ALIGN_SIZE
    #define ALIGN_SIZE 64
#endif
#ifndef TASK_SIZE
    #define TASK_SIZE 1000
#endif

class Kmeans
{
public:
    int mic_number_devices;
	int * membership, * counter, k, n, d;
	double * points, * centroids, *new_centroids;
	Kmeans(double *points, double *centroids, int t, int k, int n, int d);
private:
	int assign_step(double *points, double *centroids, int k, int n, int d);
	int asignar(double *point, double *centroids, int k, int d);
	double euclidian_distance(double *point, double *centroid, int d);
	void update_step(double *points, double *centroids, int k, int n, int d);
};
#endif
