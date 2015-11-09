#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cmath>
#include "utils.hpp"

class Kmeans
{
public:
	int * membership, * counter, k, n, d;
	double ** points, ** centroids;
	Kmeans(double **points, double **centroids, int t, int k, int n, int d);
};
#endif
