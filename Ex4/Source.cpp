#include<stdio.h>
#include<omp.h>
#include<vector>
#include<iostream>
using namespace std;

int main() {
	printf("\n No or available cores %d", omp_get_num_procs());
	printf("\n No or current threads %d", omp_get_num_threads());
	printf("\n Id of current thread %d", omp_get_thread_num());


	printf("\n No max thread %d", omp_get_max_threads());
	omp_set_num_threads(5);
	printf("\n No or current threads %d", omp_get_num_threads());

	printf("\n\n--------Sectiune---------");
#pragma omp parallel 
	{
		printf("\n Hello from thread %d", omp_get_thread_num());
	}

	printf("\n\n--------Sectiune---------");
	bool isParallel = false;
#pragma omp parallel if(isParallel)
	{
		printf("\n Hello from thread %d", omp_get_thread_num());
	}

	printf("\n\n--------Sectiune---------");
#pragma omp parallel
	{
		if (omp_get_thread_num() == 0) {
			//cod care se executa o singura data pe thread-ul cu id 0
			printf("\n No of threads %d", omp_get_num_threads());
		}
		printf("\n Hello from thread %d", omp_get_thread_num());
	}

	printf("\n\n--------Sectiune---------");
#pragma omp parallel num_threads(5)
	{
		printf("\n Hello from thread %d", omp_get_thread_num());
	}

	printf("\n\n--------Sectiune secventiala---------");
	double startTime = omp_get_wtime();
	const int N = 5e3;
	int contor = 0;
	for (int i = 0; i < N; i++) {
		contor += 1;
	}
	double stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));

	printf("\n\n--------Sectiune cu race condition---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel
	{
		for (int i = 0; i < N; i++) {
			contor += 1;
		}
		printf("\n Contor in thread %d is %d", omp_get_thread_num(), contor);
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));

	printf("\n\n--------Sectiune---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel private(contor)
	{
		for (int i = 0; i < N; i++) {
			contor += 1;
		}
		printf("\n Contor in thread %d is %d", omp_get_thread_num(), contor);
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));

	printf("\n\n--------Sectiune---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel private(contor)
	{
		contor = 0;
		for (int i = 0; i < N; i++) {
			contor += 1;
		}
		printf("\n Contor in thread %d is %d", omp_get_thread_num(), contor);
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune vector---------");
	startTime = omp_get_wtime();
	contor = 0;
	vector<int>results(omp_get_max_threads(), 0);
#pragma omp parallel private(contor)
	{
		contor = 0;
		for (int i = 0; i < N; i++) {
			contor += 1;
		}
		printf("\n Contor in thread %d is %d", omp_get_thread_num(), contor);
		results[omp_get_thread_num()] = contor;
	}
	for (int i = 0; i < omp_get_max_threads(); i++) {
		contor += results[i];
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune in race condition---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel shared(contor)
	{
		for (int i = 0; i < N; i++) {
			contor += 1;
		}
		printf("\n Contor in thread %d is %d", omp_get_thread_num(), contor);
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune without race condition critical---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel shared(contor)
	{
		for (int i = 0; i < N; i++) {
#pragma omp critical
			contor += 1;
		}
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune pragma for + critical---------");
	startTime = omp_get_wtime();
	contor = 0;
	isParallel = false;
#pragma omp parallel for if(isParallel)
	for (int i = 0; i < N; i++) {
#pragma omp critical
		contor += 1;
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune without race condition  reduction---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel reduction(+:contor)
	for (int i = 0; i < N; i++) {
		contor += 1;
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune pragma for + reduction---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel for reduction(+:contor)
	for (int i = 0; i < N; i++) {
		contor += 1;
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune pragma for + reduction + schedule static---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel for reduction(+:contor) schedule(static,100)
	for (int i = 0; i < N; i++) {
		contor += 1;
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));


	printf("\n\n--------Sectiune pragma for + reduction + schedule dynamic---------");
	startTime = omp_get_wtime();
	contor = 0;
#pragma omp parallel for reduction(+:contor) schedule(dynamic,100)
	for (int i = 0; i < N; i++) {
		contor += 1;
	}
	stopTime = omp_get_wtime();
	printf("\n Contor = %d in %lf s", contor, (stopTime - startTime));

	printf("\n End of story");
}