#include "rapl.h"
#include "rapl_sysfs.h"

double rapl_monitor_report() {
	// Using rapl_sysfs for now
	return rapl_sysfs_monitor_report();
}

int rapl_monitor_start() {
	// Using rapl_sysfs for now	
	return rapl_sysfs_monitor_start();
}