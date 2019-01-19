#include "rapl.h"
//#include "rapl_sysfs.h"
#include "rapl_fs.h"

double rapl_monitor_report() {
	// Using rapl_sysfs for now
	//return rapl_sysfs_monitor_report();
	return rapl_fs_monitor_report();
}

int rapl_monitor_start() {
	// Using rapl_sysfs for now	
	//return rapl_sysfs_monitor_start();
	return rapl_fs_monitor_start();
}


long long rapl_energy() {
	return rapl_fs_energy();
}

int hasLooped() {
	return hasLooped_fs();
}