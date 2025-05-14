#pragma once
#include <string>
#include <omp.h>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

//sa se realizeze modificari pe countPrimes astfel incat nr: 0,1 si celelalte sa fie CORECT evaluare ca nr prime

void countPrimes(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
}

//modificare ca nr 0 si 1 sa fie corect tratate
void countPrimesModif(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		if (i < 2) continue;
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
}

//modificare in scopuri de optimizare pentru ca verifica un posibil divizor pana la radicalul nr nostru
void countPrimesModifOptimizare(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j * j <= i; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
}

void countPrimesMutex(long lowerLimit, long upperLimit, long& result, mutex& mutex) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}
}

void countPrimesMutexStep(long lowerLimit, long upperLimit, long stepSize, long& result, mutex& mutex) {
	//double start = omp_get_wtime();
	for (long i = lowerLimit; i < upperLimit; i += stepSize) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}
	//double stop = omp_get_wtime();
	//printf("\n Thread duration %f for thread no %d", (stop - start), lowerLimit);
}

void countPrimesStep(long lowerLimit, long upperLimit, long stepSize, long& result) {
	//double start = omp_get_wtime();
	for (long i = lowerLimit; i < upperLimit; i += stepSize) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
	//double stop = omp_get_wtime();
	//printf("\n Thread duration %f for thread no %d", (stop - start), lowerLimit);
}


void ompCountPrimes(long lowerLimit, long upperLimit, long& result) {
	long localResult = 0;

#pragma omp parallel for reduction(+:localResult) schedule(dynamic,1000)
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			localResult += 1;
		}
	}

	result = localResult;
}


//
// Count number of primes in a given set of numbers
//
long sequentialSolution(long setSize) {
	long noPrimes = 0;
	countPrimes(0, setSize, noPrimes);
	return noPrimes;
}

//
// Count number of primes in a given set of numbers on modif count
//
long sequentialSolutionModif(long setSize) {
	long noPrimes = 0;
	countPrimesModif(0, setSize, noPrimes);
	return noPrimes;
}

//
// Count number of primes in a given set of numbers on modif count for optimization
//
long sequentialSolutionModifOptimization(long setSize) {
	long noPrimes = 0;
	countPrimesModifOptimizare(0, setSize, noPrimes);
	return noPrimes;
}

//
//Parallel solution I (race condition)
//
long parallelSolution(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	long intervalSize = setSize / noThreads;
	for (int i = 0; i < noThreads; i++) {
		long lowerLimit = i * intervalSize;
		long upperLimit = (i + 1) * intervalSize;
		threads.push_back(thread(countPrimes, lowerLimit, upperLimit, ref(noPrimes)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}

	return noPrimes;
}

//
//Parrallel solution II(no race condition -> using mutex)
//
long parallelSolutionMutex(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	long intervalSize = setSize / noThreads;
	mutex lockObject;
	for (int i = 0; i < noThreads; i++) {
		long lowerLimit = i * intervalSize;
		long upperLimit = (i + 1) * intervalSize;
		threads.push_back(thread(countPrimesMutex, lowerLimit, upperLimit, ref(noPrimes), ref(lockObject)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	return noPrimes;
}

//
//Parrallel solution III(using load balancing)
//
long parallelSolutionLoadBalancing(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	mutex lockObject;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesMutexStep, i, setSize, noThreads, ref(noPrimes), ref(lockObject)));
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	return noPrimes;
}

//
//Parrallel solution IV(using better load balancing)
//
long parallelSolutionBetterLoadBalancing(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	mutex lockObject;
	int startPoz = 1;
	//am sarit peste valorile pare(ptr ca ele nu sunt prime, exceptand valoarea 2)
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesMutexStep, startPoz, setSize, 2 * noThreads, ref(noPrimes), ref(lockObject)));
		startPoz += 2;
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	noPrimes += 1;//pentru valoare 2 care este prima
	return noPrimes;
}

//
//Parrallel solution V(using better load balancing and no mutex)
//
long parallelSolutionBetterLoadBalancingWithoutMutex(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	vector<long>results(noThreads, 0);
	int startPoz = 1;
	for (int i = 0; i < noThreads; i++) {
		threads.push_back(thread(countPrimesStep, startPoz, setSize, 2 * noThreads, ref(results[i])));
		startPoz += 2;
	}
	for (int i = 0; i < noThreads; i++) {
		threads[i].join();
	}
	for (int i = 0; i < noThreads; i++) {
		noPrimes += results[i];
	}
	noPrimes += 1;//pentru valoare 2 care este prima
	return noPrimes;
}

//
//Parrallel solution VI(using omp)
//
long ompParallelSolution(long setSize) {
	long noPrimes = 0;
	ompCountPrimes(0, setSize, noPrimes);
	return noPrimes;
}

void benchmark(string description,
	long setSize,
	long (*functionPointer)(long)) {

	printf("\n Test for %s", description.c_str());
	double tStart = omp_get_wtime();

	long result = functionPointer(setSize);

	double tFinal = omp_get_wtime();

	printf("\n Result is %d", result);
	printf("\n Required time = %f seconds", tFinal - tStart);

}