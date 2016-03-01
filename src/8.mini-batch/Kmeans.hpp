#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cmath>
#include <omp.h>
#include "utils.hpp"

#define ALLOC alloc_if(1) free_if(0)
#define FREE alloc_if(0) free_if(1)
#define REUSE alloc_if(0) free_if(0)

#ifndef ALIGN_SIZE
    #define ALIGN_SIZE 64
#endif
#ifndef TASK_SIZE
    #define TASK_SIZE 1000
#endif
#ifndef TASK_TO_MIC
    #define TASK_TO_MIC 10
#endif

class Kmeans
{
public:
	int * membership, * counter, k, n, d;
	double * points, * centroids, * mini_batch;
	int * mic_signal, * mic_membership;
	Kmeans(double *points, double *centroids, int t, int b, int k, int n, int d);
private:
	int assign_step(double *points, double *centroids, int k, int n, int d);
    __attribute__((target(mic))) int asignar(double *point, double *centroids, int k, int d);
    __attribute__((target(mic))) double euclidian_distance(double *point, double *centroid, int d);
	void update_step(double *points, double *centroids, int k, int n, int d);
};
#endif
