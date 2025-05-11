#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include<atomic>
#include <omp.h>
#include<iostream>
using namespace std;

void printHelloFromThreadWithId1(int id) {
	cout << endl << "Hello " << " from another thread" << " with id " << id;
}

void printHelloFromThreadWithId2(int id) {
	cout << "\n Hello from another thread with id " << id;
}

void printHelloFromThreadWithId3(int id) {
	printf("\n Hello from another thread with id %d", id);
}

void counter(int& counter, int limit) {
	for (int i = 0; i < limit; i++)
		counter += 1;
}

//define a global mutex
mutex counterMutex;

//define a global atomic
atomic<int> counterAtomic(0);

void counterWithMutex(int& counter, int limit) {
	for (int i = 0; i < limit; i++) {
		counterMutex.lock();
		counter += 1;
		counterMutex.unlock();
	}
}

void counterWithAtomic(int limit) {
	for (int i = 0; i < limit; i++) {
		counterAtomic += 1;
	}
}

int main() {
	const int noCores = omp_get_num_procs();
	printf("\nAvailable cores: %d", noCores);

	double currentTime = omp_get_wtime();

	printf("\nCurrent time %f", currentTime);

	currentTime = omp_get_wtime();

	printf("\nCurrent time %f", currentTime);


	int finalCounter = 0;
	vector<thread> threads;

	const int limit = 5e6;
	double tStart;
	double tFinal;

	//testing cout and printf
	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(printHelloFromThreadWithId1, i));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe time required for printHello1 is %f s", (tFinal - tStart));


	threads.clear();
	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(printHelloFromThreadWithId2, i));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe time required for printHello2 is %f s", (tFinal - tStart));


	threads.clear();
	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(printHelloFromThreadWithId3, i));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe time required for printHello3 is %f s", (tFinal - tStart));



	//testing the solution in sequential way
	tStart = omp_get_wtime();
	finalCounter = 0;
	for (int i = 0; i < noCores; i++) {
		counter(finalCounter, limit);
	}
	tFinal = omp_get_wtime();
	printf("\nThe counter result in sequntial way is %d and it required %f s",
		finalCounter, (tFinal - tStart));


	//testing the solution with race conditions
	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counter, ref(finalCounter), limit));
		//threads[i].join(); //it's a mistake - you wait on the thread to finish
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe counter result with race condition and multiple threads is %d and it required %f s",
		finalCounter, (tFinal - tStart));


	//testing the solution with a mutex and without race conditions
	threads.clear();
	tStart = omp_get_wtime();
	finalCounter = 0;
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counterWithMutex, ref(finalCounter), limit));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe counter result with mutex and multiple threads is %d and it required %f s",
		finalCounter, (tFinal - tStart));


	//testing the solution with an atomic
	threads.clear();
	tStart = omp_get_wtime();
	finalCounter = 0;
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counterWithAtomic, limit));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	tFinal = omp_get_wtime();
	printf("\nThe counter result with atomic and multiple threads is %d and it required %f s",
		counterAtomic.load(), (tFinal - tStart));


	//testing the solution in parallel version
	threads.clear();
	tStart = omp_get_wtime();
	finalCounter = 0;
	vector<int>results(noCores, 0);
	for (int i = 0; i < noCores; i++) {

		threads.push_back(thread(counter, ref(results[i]), limit));
	}
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}
	for (int i = 0; i < noCores; i++) {
		finalCounter += results[i];
	}
	tFinal = omp_get_wtime();
	printf("\nThe counter result with parallel programming and multiple threads is %d and it required %f s",
		finalCounter, (tFinal - tStart));
}