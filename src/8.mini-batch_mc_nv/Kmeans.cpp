#include "Kmeans.hpp"

Kmeans::Kmeans(double *points, double *centroids, int t, int b, int k, int n, int d)
{
	Utils utils;
	membership = (int *) _mm_malloc (sizeof(int) * b * TASK_SIZE, ALIGN_SIZE);
	counter = (int *) _mm_malloc (sizeof(int) * k, ALIGN_SIZE);

	mini_batch = (double *) _mm_malloc (sizeof(double) * b * TASK_SIZE * d, ALIGN_SIZE);

	//initialization of variables
	for (int i=0; i<k; i++) counter[i] = 0;

	utils.seed_random();

	int changed;
	do
	{
		utils.start_timer("iteration");

		utils.start_timer("mbatch");
		//mini-batch creation loop
		for (int i=0; i<b * TASK_SIZE; i++)
		{
			int row = utils.get_random() * n;
			for (int j=0; j<d; j++)
			{
				mini_batch[i*d+j] = points[row*d+j];
			}
		}
		utils.stop_timer("mbatch");

		utils.start_timer("estep");
		changed = assign_step(mini_batch, centroids, k, b * TASK_SIZE, d);
		utils.stop_timer("estep");

		utils.start_timer("mstep");
		update_step(mini_batch, centroids, k, b * TASK_SIZE, d);
		utils.stop_timer("mstep");

		utils.stop_timer("iteration");
		t--;
	} while (t > 0 && changed > 0);

	std::cout << "Tiempo mini-batch promedio: " << utils.get_prom_time("mbatch") << std::endl;
	std::cout << "Tiempo mini-batch total: " << utils.get_total_time("mbatch") << std::endl;
	std::cout << "Tiempo estep promedio: " << utils.get_prom_time("estep") << std::endl;
	std::cout << "Tiempo estep total: " << utils.get_total_time("estep") << std::endl;
	std::cout << "Tiempo mstep promedio: " << utils.get_prom_time("mstep") << std::endl;
	std::cout << "Tiempo mstep total: " << utils.get_total_time("mstep") << std::endl;
	std::cout << "Tiempo iteracion promedio: " << utils.get_prom_time("iteration") << std::endl;
	std::cout << "Tiempo iteracion total: " << utils.get_total_time("iteration") << std::endl;
}

int Kmeans::assign_step(double *mini_batch, double *centroids, int k, int n, int d)
{
	int host_changed = 0, kmin;

	//host computation
	#pragma omp parallel for schedule(static) private(kmin)
	for (int i=0; i<n; i++)
	{
		kmin = asignar(mini_batch + (i * d), centroids, k, d);
		if (membership[i] != kmin)
			#pragma omp atomic
			host_changed = host_changed + 1;
		membership[i] = kmin;
	}

	return host_changed;
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
	__assume_aligned(counter, ALIGN_SIZE);

	double learning_rate;
    int c;
    for (int i=0; i<n; i++)
    {
        c = membership[i];
        counter[c]++;
        learning_rate = 1 / counter[c];

        for (int j=0; j<d; j++)
        {
        	centroids[c*d+j] = ((1 - learning_rate)*centroids[c*d+j]) + (learning_rate * points[i*d+j]);
        }
    }
}
