#include <string>
#include <vector>
#include <stdio.h>
#include <omp.h>
#include "Methods.h"
using namespace std;

int main() {

	const long N = 5e5;

	const int noCores = omp_get_num_procs();
	printf("\n Available cores = %d", noCores);

	benchmark("Sequential solution", N, sequentialSolution);
}