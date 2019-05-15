#ifndef __RAPL_H
#define __RAPL_H

#ifdef __cplusplus
extern "C" {
#endif

double rapl_monitor_report();

int rapl_monitor_start();

long long rapl_energy();

int rapl_hasLooped();

#ifdef __cplusplus
}
#endif

#endif
