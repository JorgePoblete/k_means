#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cmath>

class Kmeans
{
public:
	int * membership, * counter;
	Kmeans(double **points, double **centroids, int t, int k, int n, int d);
};
#endif
