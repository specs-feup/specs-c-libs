#ifndef __RAPL_FS_H
#define __RAPL_FS_H

// This value will be used to estimate the update frequency of energy measurements
#ifndef SPECS_RAPL_POWER_W
#define SPECS_RAPL_POWER_W 200
#endif

#define DEBUG 0

#ifdef __cplusplus
extern "C" {
#endif

// 
struct RaplFsEnergy{
	long long energy;
	long long lap;
};

long long rapl_fs_energy();

int rapl_fs_hasLooped();

#ifdef __cplusplus
}
#endif

#endif
