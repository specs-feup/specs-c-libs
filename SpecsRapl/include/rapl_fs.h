#ifndef __RAPL_FS_H
#define __RAPL_FS_H

// This value will be used to estimate the update frequency of energy measurements
#ifndef SPECS_RAPL_POWER_W
#define SPECS_RAPL_POWER_W 200
#endif

#define DEBUG 1

#ifdef __cplusplus
extern "C" {
#endif

// 
struct RaplFsEnergy{
	long long energy;
	long long lap;
};

double rapl_fs_monitor_report();

int rapl_fs_monitor_start();

long long rapl_fs_energy();

int hasLooped_fs();

#ifdef __cplusplus
}
#endif

#endif