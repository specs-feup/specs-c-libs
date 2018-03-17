#ifndef __RAPL_SYSFS_H
#define __RAPL_SYSFS_H

#ifdef __cplusplus
extern "C" {
#endif

double rapl_sysfs_monitor_report();

int rapl_sysfs_monitor_start();

#ifdef __cplusplus
}
#endif

#endif