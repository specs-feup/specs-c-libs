#include "energymonitor.h"
#include "getnode.h"
#include <cassert>
#include <iostream>

EnergyData::EnergyData()
	: m_armW(0), m_memW(0), m_kfcW(0), m_g3dW(0), m_samples(0), m_total_cpu(0), m_total_gpu(0), m_total_mem(0)
{
}

void EnergyData::addSample(float armW, float memW, float kfcW, float g3dW) {
	m_armW.push_back(armW);
	m_memW.push_back(memW);
	m_kfcW.push_back(kfcW);
	m_g3dW.push_back(g3dW);

	if (m_samples > 0) {
		// Ignore first sample.
		m_total_cpu += armW + kfcW;
		m_total_gpu += g3dW;
		m_total_mem += memW;
	}

	++m_samples;
}

static void* start_thread(void* data) {
	EnergyMonitor* monitor = (EnergyMonitor*) data;

	monitor->run();

	return nullptr;
}

EnergyMonitor::EnergyMonitor(useconds_t uwait) : m_waittime(uwait), m_finished(false) {
	int result = pthread_create(&m_thread, NULL, start_thread, (void*) this);

	assert(result == 0);
}

EnergyData EnergyMonitor::stop() {
	assert(!m_finished.load());

	m_finished.store(true);
	pthread_join(m_thread, NULL);

	return m_data;
}

void EnergyMonitor::run() {
	if (m_getter.OpenINA231() < 0) {
		std::cerr << "Could not open sensors." << std::endl;

		return;
	}

	while (!m_finished.load()) {
		usleep(m_waittime);

		m_getter.GetINA231();

		m_data.addSample(m_getter.armuW, m_getter.memuW, m_getter.kfcuW, m_getter.g3duW);
	}

	m_getter.CloseINA231();
}
