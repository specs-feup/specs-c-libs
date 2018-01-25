#ifndef ENERGY_MONITOR_H_
#define ENERGY_MONITOR_H_

#include "getnode.h"
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <vector>

class EnergyData {
	std::vector<float> m_armW;
	std::vector<float> m_memW;
	std::vector<float> m_kfcW;
	std::vector<float> m_g3dW;
	size_t m_samples;
	float m_total_cpu;
	float m_total_mem;
	float m_total_gpu;
public:
	EnergyData();

	void addSample(float armW, float memW, float kfcW, float g3dW);

	const float* arm() const { return m_armW.data(); }
	const float* mem() const { return m_memW.data(); }
	const float* kfc() const { return m_kfcW.data(); }
	const float* g3d() const { return m_g3dW.data(); }

	float total_cpu() const { return m_total_cpu; }
	float total_gpu() const { return m_total_gpu; }
	float total_mem() const { return m_total_mem; }
	size_t samples() const { return m_samples; }

	float average_cpu() const { return total_cpu() / (samples() - 1); }
	float average_gpu() const { return total_gpu() / (samples() - 1); }
	float average_mem() const { return total_mem() / (samples() - 1); }
};

static void* start_thread(void* data);

class EnergyMonitor {
	pthread_t m_thread;
	useconds_t m_waittime;
	std::atomic_bool m_finished;

	EnergyData m_data;
	GetNode m_getter;

	void run();

	friend void* start_thread(void* data);
public:
	EnergyMonitor(useconds_t uwait);

	EnergyData stop();
};

#endif
