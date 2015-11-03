#include "Kmeans.hpp"


Kmeans::Kmeans(double **points, double **centroids, int t, int k, int n, int d)
{
	membership = (int *) malloc (sizeof(int)*n);
	counter = (int *) malloc (sizeof(int)*k);
	double ** newCentroids = (double **) malloc (sizeof(double)*k);
	for (int i=0; i<k; i++)
		newCentroids[i] = (double *) malloc (sizeof(double)*d);
	int changed, kmin;
	double dmin,dist;
	do
	{
		//reinitialization of variables
		for (int i=0; i<k; i++)
		{
			for (int j=0; j<d; j++)
				newCentroids[i][j] = 0.0;
			counter[i] = 0;
		}

		changed = 0;
		//estep
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
		//mstep
		for (int x=0; x<n; x++)
		{
			int c = membership[x];
			counter[c]++;
			for (int m=0; m<d; m++)
				newCentroids[c][m] += points[x][m];
		}
		for (int c=0; c<k; c++)
		{
			if (counter[c] > 0)
				for (int m=0; m<d; m++)
					centroids[c][m] = newCentroids[c][m] / counter[c];
		}
		t--;
	} while (t > 0 && changed > 0);
}
