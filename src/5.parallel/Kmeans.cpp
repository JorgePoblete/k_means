#include "Kmeans.hpp"

Kmeans::Kmeans(double *points, double *centroids, int t, int k, int n, int d)
{
	Utils utils;
	membership = (int *) _mm_malloc (sizeof(int) * n, ALIGN_SIZE);
	counter = (int *) _mm_malloc (sizeof(int) * k, ALIGN_SIZE);
	new_centroids = (double *) _mm_malloc (sizeof(double) * k * d, ALIGN_SIZE);

	int changed;
	do
	{
		utils.start_timer("iteration");

		utils.start_timer("estep");
		changed = assign_step(points, centroids, k, n, d);
		utils.stop_timer("estep");

		utils.start_timer("mstep");
		update_step(points, centroids, k, n, d);
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

int Kmeans::assign_step(double *points, double *centroids, int k, int n, int d)
{
	int changed = 0, kmin;
	#pragma omp parallel for schedule(static)
	for (int x=0; x<n; x++)
	{
		kmin = asignar(points + (x * d), centroids, k, d);
		if (membership[x] != kmin)
			changed = changed + 1;
		membership[x] = kmin;
	}
	return changed;
}
int Kmeans::asignar(double *point, double *centroids, int k, int d)
{
	int kmin;
  	double dmin,dist;
	dmin = 99e99;
	kmin = 0;
	for (int c=0; c<k; c++)
	{
		dist = euclidian_distance(point, centroids + (c * d), d);
		if (dmin > dist)
		{
			dmin = dist;
			kmin = c;
		}
	}
	return kmin;
}
double Kmeans::euclidian_distance(double *point, double *centroid, int d)
{
	__assume_aligned(point, ALIGN_SIZE);
	__assume_aligned(centroid, ALIGN_SIZE);
	double dist = 0.0;
	#pragma ivdep
	#pragma vector aligned
	for (int m=0; m<d; m++)
	{
		dist += (point[m]-centroid[m]) * (point[m]-centroid[m]);
	}
	dist = sqrt(dist);
	return dist;
}
void Kmeans::update_step(double *points, double *centroids, int k, int n, int d)
{
	__assume_aligned(points, ALIGN_SIZE);
	__assume_aligned(centroids, ALIGN_SIZE);
	__assume_aligned(new_centroids, ALIGN_SIZE);
	__assume_aligned(counter, ALIGN_SIZE);
	//reinitialization of variables
	for (int i=0; i<k; i++)
	{
		for (int j=0; j<d; j++)
			new_centroids[i * d + j] = 0.0;
		counter[i] = 0;
	}
	for (int x=0; x<n; x++)
	{
		int c = membership[x];
		counter[c]++;
		for (int m=0; m<d; m++)
			new_centroids[c * d + m] += points[x * d + m];
	}
	for (int c=0; c<k; c++)
	{
		if (counter[c] > 0)
			for (int m=0; m<d; m++)
				centroids[c * d + m] = new_centroids[c * d + m] / counter[c];
	}
}
