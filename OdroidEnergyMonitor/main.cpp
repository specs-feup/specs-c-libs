#include "energymonitor.h"
#include <iostream>
#include <cstdlib>

#define N 10000
#define M N

float compute_sum() {
	float* x = (float*) malloc(sizeof(float) * N);
	float* y = (float*) malloc(sizeof(float) * M);

	for (int i = 0; i < N; ++i) {
		x[i] = rand() % 10;
	}
	for (int j = 0; j < M; ++j) {
		y[j] = rand() % 10;
	}

	float acc = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			acc += x[i] * y[j];
		}
	}

	return acc;
}

int main() {
	EnergyMonitor monitor(263808);

	float vecSum = compute_sum();

	EnergyData data = monitor.stop();

	std::cout << "Samples: " << data.samples() << std::endl;

	std::cout << "Average CPU: " << data.average_cpu() << std::endl;
	std::cout << "Average GPU: " << data.average_gpu() << std::endl;
	std::cout << "Average MEM: " << data.average_mem() << std::endl;

	std::cout << "Result: " << vecSum << std::endl;

	return 0;
}
