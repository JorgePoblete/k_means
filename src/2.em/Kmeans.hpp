#ifndef KMEANS_H_
#define KMEANS_H_

#include <iostream>
#include <cmath>
#include "utils.hpp"

class Kmeans
{
public:
	int * membership, * counter, k, n, d;
	double ** points, ** centroids, **new_centroids;
	Kmeans(double **points, double **centroids, int t, int k, int n, int d);
private:
	int assign_step(double **points, double **centroids, int k, int n, int d);
	int asignar(double *point, double **centroids, int k, int d);
	double euclidian_distance(double *point, double *centroid, int d);
	void update_step(double **points, double **centroids, int k, int n, int d);
};
#endif
