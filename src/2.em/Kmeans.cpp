#include "Kmeans.hpp"

Kmeans::Kmeans(double **points, double **centroids, int t, int k, int n, int d)
{
	Utils utils;
	membership = (int *) malloc (sizeof(int)*n);
	counter = (int *) malloc (sizeof(int)*k);
	new_centroids = (double **) malloc (sizeof(double)*k);
	for (int i=0; i<k; i++)
		new_centroids[i] = (double *) malloc (sizeof(double)*d);
	int changed;
	do
	{
		utils.start_timer("iteration");
		//reinitialization of variables
		for (int i=0; i<k; i++)
		{
			for (int j=0; j<d; j++)
				new_centroids[i][j] = 0.0;
			counter[i] = 0;
		}

		utils.start_timer("estep");
		changed = estep(points, centroids, k, n, d);
		utils.stop_timer("estep");

		utils.start_timer("mstep");
		mstep(points, centroids, k, n, d);
		utils.stop_timer("mstep");

		utils.stop_timer("iteration");
		t--;
	} while (t > 0 && changed > 0);
	std::cout << "Tiempo estep promedio: " << utils.get_prom_time("estep") << std::endl;
	std::cout << "Tiempo estep total: " << utils.get_total_time("estep") << std::endl;
	std::cout << "Tiempo mstep promedio: " << utils.get_prom_time("mstep") << std::endl;
	std::cout << "Tiempo mstep total: " << utils.get_total_time("mstep") << std::endl;
	std::cout << "Tiempo iteracion promedio: " << utils.get_prom_time("iteration") << std::endl;
	std::cout << "Tiempo iteracion total: " << utils.get_total_time("iteration") << std::endl;
}

int Kmeans::estep(double **points, double **centroids, int k, int n, int d)
{
	int changed = 0, kmin;
	double dmin,dist;
	for (int x=0; x<n; x++)
	{
		dmin = 99e99;
		kmin = 0;
		for (int c=0; c<k; c++)
		{
			//euclid distance on a d-dimensional space
			dist = 0.0;
			for (int m=0; m<d; m++)
			{
				dist += (points[x][m]-centroids[c][m]) * (points[x][m]-centroids[c][m]);
			}
			dist = sqrt(dist);
			if (dmin > dist)
			{
				dmin = dist;
				kmin = c;
			}
		}
		if (membership[x] != kmin)
			changed = changed + 1;
		membership[x] = kmin;
	}
	return changed;
}
void Kmeans::mstep(double **points, double **centroids, int k, int n, int d)
{
	for (int x=0; x<n; x++)
	{
		int c = membership[x];
		counter[c]++;
		for (int m=0; m<d; m++)
			new_centroids[c][m] += points[x][m];
	}
	for (int c=0; c<k; c++)
	{
		if (counter[c] > 0)
			for (int m=0; m<d; m++)
				centroids[c][m] = new_centroids[c][m] / counter[c];
	}
}