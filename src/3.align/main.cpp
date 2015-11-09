#include <iostream>
#include <cstdlib>
#include "Kmeans.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
    std::cout.precision(10);
    if (argc < 5){
        exit (0);
    }
    int k = atoi(argv[1]);
    int n = atoi(argv[2]);
    int d = atoi(argv[3]);
    int t = atoi(argv[4]);
    //asignacion de memoria
    double * data = (double *) _mm_malloc (sizeof(double)*n*d,64);
    double * means = (double *) _mm_malloc (sizeof(double)*k*d,64);
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
    Kmeans kmeans(data,means,t,k,n,d);
    utils.stop_timer("kmeans");
    std::cout << "Tiempo kmeans total: " << utils.get_total_time("kmeans") << std::endl;

    for (int i=0; i<k;i++)
    {
        for (int j=0; j<d; j++)
            std::cout << means[i*d+j] << " ";
        std::cout << std::endl;
    }
    return 0;
}
