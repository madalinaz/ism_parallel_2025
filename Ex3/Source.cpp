#include <string>
#include <vector>
#include <stdio.h>
#include <omp.h>
#include "Methods.h"
using namespace std;

int main() {

	const long N = 3e5;

	const int noCores = omp_get_num_procs();
	printf("\n Available cores = %d", noCores);

	benchmark("Sequential solution", N, sequentialSolution);
	benchmark("Sequential solution taking care of 0,1", N, sequentialSolutionModif);
	benchmark("Sequential solution with sqrt", N, sequentialSolutionModifOptimization);
	//benchmark("Parallel solution with race condition", N, parallelSolution);
	benchmark("Parallel solution without race condition (using mutex)", N, parallelSolutionMutex);
	benchmark("Parallel solution with load balancing", N, parallelSolutionLoadBalancing);
	benchmark("Parallel solution with better load balancing", N, parallelSolutionBetterLoadBalancing);
	benchmark("Parallel solution with better load balancing without mutex", N, parallelSolutionBetterLoadBalancingWithoutMutex);
	benchmark("Parallel solution using omp", N, ompParallelSolution);
}