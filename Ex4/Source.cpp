#include<stdio.h>
#include<omp.h>
#include<vector>
using namespace std;

int main() {
	printf("\n no available cores %d", omp_get_num_procs());
	printf("\n no of threads %d", omp_get_num_threads());

	printf("\n Hello from thread %d", omp_get_thread_num());
	omp_set_num_threads(4);
	printf("\n no of threads %d", omp_get_num_threads());

	bool isParallel = false;
#pragma omp parallel if(isParallel)
	{
		if (omp_get_thread_num() == 0) {
			printf("\n no of threads %d", omp_get_num_threads());
		}
		printf("\n Hello from thread %d", omp_get_thread_num());
	}

#pragma omp parallel num_threads(7)
	{
		if (omp_get_thread_num() == 0) {
			printf("\n no of threads %d", omp_get_num_threads());
		}
		printf("\n Hello from thread %d", omp_get_thread_num());
	}


	int contor = 0;
#pragma omp parallel
	{
		for (int i = 0; i < 10000; i++) 
		{
			contor += 1;
		}
		printf("\n contor = %d", contor);
	}
	printf("\n contor final = %d", contor);


	contor = 0;
#pragma omp parallel private(contor)
	{
		contor = 0;
		for (int i = 0; i < 1000; i++)
		{
			contor += 1;
		}
		printf("\n contor = %d", contor);
	}
	printf("\n contor final = %d", contor);


	vector<int> results(omp_get_num_procs(), 0);
#pragma omp parallel private(contor)
	{
		contor = 0;
		for (int i = 0; i < 10000; i++)
		{
			contor += 1;
		}
		results[omp_get_thread_num()] = contor;
	}
	contor = 0;
	for (int i = 0; i < omp_get_num_procs(); i++) {
		contor += results[i];
	}
	printf("\n contor final = %d", contor);


	contor = 0;
#pragma omp parallel shared(contor)
	{
		for (int i = 0; i < 10000; i++)
		{
			contor += 1;
		}
	}
	printf("\n contor final = %d", contor);


	contor = 0;
#pragma omp parallel shared(contor)
	{
		for (int i = 0; i < 10000; i++)
		{
		#pragma omp critical
			contor += 1;
		}
	}
	printf("\n contor final = %d", contor);


	contor = 0;
#pragma omp parallel for
		for (int i = 0; i < 10000; i++)
		{
#pragma omp critical
			contor += 1;
		}

	printf("\n contor final = %d", contor);


	contor = 0;
#pragma omp parallel for
	for (int i = 0; i < 10000; i++)
	{
#pragma omp critical
		contor += 1;
	}

	printf("\n contor final = %d", contor);


	printf("\n end of main");
}

