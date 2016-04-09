#include <iostream>
#include <cstdlib>
#include "Kmeans.hpp"
#include "utils.hpp"

#ifndef ALIGN_SIZE
    #define ALIGN_SIZE 64
#endif
#ifndef TASK_SIZE
    #define TASK_SIZE 1000
#endif


int main(int argc, char** argv)
{
    std::cout.precision(10);
    if (argc < 6){
    	std::cout << "Insuficient args " << std::endl;
        exit (0);
    }
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int d = atoi(argv[3]);
    int t = atoi(argv[4]);
    int b = atoi(argv[5]);

    if (b < TASK_TO_MIC)
    {
    	std::cout << "Mini-Batch size must be >= than " << TASK_TO_MIC << std::endl;
    	exit(0);
    }

    std::cout << "ALIGN_SIZE: " << ALIGN_SIZE << std::endl;
    std::cout << "TASK_SIZE: " << TASK_SIZE << std::endl;

    //asignacion de memoria
    double * data = (double *) _mm_malloc (sizeof(double) * n * d, ALIGN_SIZE);
    double * means = (double *) _mm_malloc (sizeof(double) * k * d, ALIGN_SIZE);
    //lectura de datos
    for (int i=0; i<n;i++)
    {
        for (int j=0; j<d; j++)
            std::cin >> data[i*d+j];
    }
    //se asignan los primeros k datos
    for (int i=0; i<k; i++)
    {
        for (int j=0; j<d;j++)
        {
            means[i*d+j] = data[i*d+j];
        }
    }
    Utils utils;
    utils.start_timer("kmeans");
    Kmeans kmeans(data, means, t, b, k, n, d);
    utils.stop_timer("kmeans");
    std::cout << "Tiempo kmeans total: " << utils.get_total_time("kmeans") << std::endl;

    /*
    for (int i=0; i<k;i++)
    {
        for (int j=0; j<d; j++)
            std::cout << means[i][j] << " ";
        std::cout << std::endl;
    }
    */
    return 0;
}
