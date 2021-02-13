#ifndef ENERGYMONITOR_C_H_
#define ENERGYMONITOR_C_H_

#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* emonitor_t;
typedef void* edata_t;

emonitor_t monitor_new(useconds_t wait_time);
edata_t monitor_stop(emonitor_t monitor);

size_t data_get_samples(edata_t data);
float data_get_average_cpu(edata_t data);
float data_get_average_gpu(edata_t data);
float data_get_average_mem(edata_t data);

#ifdef __cplusplus
}
#endif

#endif
