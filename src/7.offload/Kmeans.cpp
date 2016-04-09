#include "Kmeans.hpp"

Kmeans::Kmeans(double *points, double *centroids, int t, int k, int n, int d)
{
	Utils utils;
	membership = (int *) _mm_malloc (sizeof(int) * n, ALIGN_SIZE);
	counter = (int *) _mm_malloc (sizeof(int) * k, ALIGN_SIZE);
	new_centroids = (double *) _mm_malloc (sizeof(double) * k * d, ALIGN_SIZE);

	//mic memory management
	#pragma offload_transfer target(mic) \
	nocopy(centroids : ALLOC length(k*d)) \
	in(points[0:TASK_TO_MIC*TASK_SIZE*d] : ALLOC into(points[0:TASK_TO_MIC*TASK_SIZE*d]))

	#pragma offload_transfer target(mic) \
	nocopy(membership : ALLOC length(TASK_SIZE*TASK_TO_MIC))

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

	//release memory on mic
	#pragma offload_transfer target(mic) \
	nocopy(centroids: FREE length(0)) \
	nocopy(points: FREE length(0)) \
	nocopy(membership: FREE length(0))

	std::cout << "Tiempo estep promedio: " << utils.get_prom_time("estep") << std::endl;
	std::cout << "Tiempo estep total: " << utils.get_total_time("estep") << std::endl;
	std::cout << "Tiempo mstep promedio: " << utils.get_prom_time("mstep") << std::endl;
	std::cout << "Tiempo mstep total: " << utils.get_total_time("mstep") << std::endl;
	std::cout << "Tiempo iteracion promedio: " << utils.get_prom_time("iteration") << std::endl;
	std::cout << "Tiempo iteracion total: " << utils.get_total_time("iteration") << std::endl;
}

int Kmeans::assign_step(double *points, double *centroids, int k, int n, int d)
{
	int mic_changed = 0, host_changed = 0, kmin;
	mic_membership = membership;

	//mic computation
	#pragma offload target(mic:0) \
	in(centroids[0:k*d] : REUSE into(centroids[0:k*d])) \
	in(points : REUSE length(0)) \
	in(mic_membership : REUSE length(0)) \
	signal(mic_signal)
	{
		#pragma omp parallel for schedule(static) private(kmin)
		for (int i=0; i<TASK_SIZE*TASK_TO_MIC; i++)
		{
			kmin = asignar(points + (i * d), centroids, k, d);

			if (mic_membership[i] != kmin)
				#pragma omp atomic
				mic_changed = mic_changed + 1;
			mic_membership[i] = kmin;
		}
	}

	//host computation
	#pragma omp parallel for schedule(static) private(kmin)
	for (int i=TASK_SIZE*TASK_TO_MIC; i<n; i++)
	{
		kmin = asignar(points + (i * d), centroids, k, d);
		if (membership[i] != kmin)
			#pragma omp atomic
			host_changed = host_changed + 1;
		membership[i] = kmin;
	}

	#pragma offload_transfer target(mic:0) \
	wait(mic_signal) \
	out(membership : REUSE length(TASK_SIZE*TASK_TO_MIC))

	return mic_changed + host_changed;
}

__attribute__((target(mic)))
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
__attribute__((target(mic)))
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
