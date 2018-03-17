#include "energymonitor_c.h"
#include "energymonitor.h"

emonitor_t monitor_new(useconds_t wait_time) {
	return new EnergyMonitor(wait_time);
}
edata_t monitor_stop(emonitor_t monitor) {
	return new EnergyData(static_cast<EnergyMonitor*>(monitor)->stop());
}

size_t data_get_samples(edata_t data) {
	return static_cast<EnergyData*>(data)->samples();
}

float data_get_average_cpu(edata_t data) {
	return static_cast<EnergyData*>(data)->average_cpu();
}

float data_get_average_gpu(edata_t data) {
	return static_cast<EnergyData*>(data)->average_gpu();
}

float data_get_average_mem(edata_t data) {
	return static_cast<EnergyData*>(data)->average_mem();
}
