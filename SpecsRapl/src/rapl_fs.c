#include "rapl_fs.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>


/// Definitions

// Amount needed to multiply current values to obtain joules
// Current unit is set to uJ
//double TO_JOULE = 0.000001;

// Amount needed to divide with the current values to obtain Joules
// Current unit is set to uJ
long long JOULE_SCALE = 1000000;

/// Needs initialization

// Number of packages in the system to measure
unsigned int numberPackages;

// Max energy range of the machine of each package
long long *maxEnergyRanges;

// Update frequency, based on smaller energy range
unsigned int sleepAmount;

// Global state that manages energy measurement
struct RaplFsEnergy* lastMeasures;


/// Miscellaneous state
bool firstTime = true;
pthread_t updateThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool looped = false;

int hasLooped_fs() {
	return looped;
}

double rapl_fs_monitor_report() {
	printf("'rapl_fs_monitor_report() NOT IMPLEMENTED!\n'");
}

int rapl_fs_monitor_start() {
	printf("'rapl_fs_monitor_start() NOT IMPLEMENTED!\n'");
}

/**
 * Writes the field 'energy' with the energy value of the corresponding package.
 */
void readEnergy(struct RaplFsEnergy* energyMeasure) {
	for(int i=0; i<numberPackages; i++) {
		char energyFilename[100];
		snprintf(energyFilename, 100, "/sys/class/powercap/intel-rapl/intel-rapl:%d/energy_uj",i);		

		// Read maximum energy range
		FILE *energyFile = fopen(energyFilename,"r");

		if(energyFile == NULL){
			printf("[SPECS_RAPL] Could not read energy value for package %d\n", i);
			exit(1);
		}

		fscanf(energyFile,"%lld", &energyMeasure[i].energy);
		fclose(energyFile);
		
		if(DEBUG) {
			printf("[SPECS_RAPL] Current energy for package %d: %lld\n", i, energyMeasure[i].energy);
		}		
	}
}


/**
 * Converts the given measure into a single value.
 */
long long toEnergy(struct RaplFsEnergy* energyMeasure) {
	long long totalEnergy = 0LL;
	
	for(int i=0; i<numberPackages; i++) {
		if(DEBUG) {
			printf("[SPECS_RAPL] Package %d contribution: %lld of current energy + %lld laps of %lld max energy range\n", i, energyMeasure[i].energy, energyMeasure[i].lap, maxEnergyRanges[i]);
		}
		
		totalEnergy += energyMeasure[i].energy + (energyMeasure[i].lap * maxEnergyRanges[i]);
	}
	
	return totalEnergy;
}


void update() {
	// Read current energy
	struct RaplFsEnergy* currentEnergies;
	currentEnergies = malloc(numberPackages*sizeof(struct RaplFsEnergy));
	
	
	readEnergy(currentEnergies);


	pthread_mutex_lock(&mutex);
	
	// Calculate current laps 
	for(int i=0; i<numberPackages; i++) {
		// If last energy is less or equal than current energy, use the same lap
		if(lastMeasures[i].energy <= currentEnergies[i].energy) {
			currentEnergies[i].lap = lastMeasures[i].lap;
		} 
		// Otherwise, increase lap number
		else {
			currentEnergies[i].lap = lastMeasures[i].lap + 1;
			printf("[SPECS_RAPL] Increase lap number for package %d, currently is %lld\n", i, currentEnergies[i].lap);
			looped = true;
		}
	}
	
	// Update last measure
	struct RaplFsEnergy* tempEnergies = lastMeasures;
	lastMeasures = currentEnergies;
	currentEnergies = tempEnergies;
	
	pthread_mutex_unlock(&mutex);
	
	free(currentEnergies);
}

void* updateThreadFunction(void* arg) {
	while(1) {
		sleep(sleepAmount);
		if(DEBUG) {
			printf("[SPECS_RAPL] Update thread is executing\n");
		}
		
		update();
	}
}

/**
 * Initializes needed values for the library 
 */
void init() {
	//printf("INIT!\n");
	
	// Get number of packages
	int currentPackages = 0;

	while(1) {
		char packageFoldername[100];
		snprintf(packageFoldername, 100, "/sys/class/powercap/intel-rapl/intel-rapl:%d",currentPackages);
		
		if(DEBUG) {
			printf("[SPECS_RAPL] Testing package folder %s\n", packageFoldername);
		}
		
		// Test if folder exists
		FILE *packageFolder = fopen(packageFoldername,"r");
				
		if(packageFolder == NULL) {
			break;
		}
	
		currentPackages++;
		fclose(packageFolder);
	}

	// Init number of packages
	numberPackages = currentPackages;
	
	if(DEBUG) {
		printf("[SPECS_RAPL] Found packages: %d\n", numberPackages);
	}
	
	if(numberPackages == 0) {
		printf("[SPECS_RAPL] WARNING: No RAPL packages found, energy cannot be measured\n");
		return;
	}
	
	
	// Obtain maximum energy ranges for each package

	maxEnergyRanges = malloc(numberPackages*sizeof(long long));
	for(int i=0; i<numberPackages; i++) {
		char maxRangeFilename[100];
		snprintf(maxRangeFilename, 100, "/sys/class/powercap/intel-rapl/intel-rapl:%d/max_energy_range_uj",i);		

		// Read maximum energy range
		FILE *maxRangeFile = fopen(maxRangeFilename,"r");

		if(maxRangeFile == NULL){
			printf("[SPECS_RAPL] Could not read maximum energy range for package %d\n", i);
			exit(1);
		}

		fscanf(maxRangeFile,"%lld", &maxEnergyRanges[i]);
		fclose(maxRangeFile);
		
		if(DEBUG) {
			printf("[SPECS_RAPL] Max range for package %d: %lld\n", i, maxEnergyRanges[i]);
		}
	}	

		
	// Calculate sleep amount based on the minimum maximum range and defined maximum power
	long long minMaxRange = maxEnergyRanges[0];
	for(int i=1; i<numberPackages; i++) {
		if(maxEnergyRanges[i] < minMaxRange) {
			minMaxRange = maxEnergyRanges[i];
		}
	}
	
	sleepAmount = (minMaxRange / JOULE_SCALE) / SPECS_RAPL_POWER_W;
	
	// Divide by 2, just to be on the safe side
	sleepAmount = sleepAmount / 2;
	
	if(DEBUG) {
		printf("[SPECS_RAPL] Update thread sleep amount is %d seconds\n", sleepAmount);
	}
	
	
	// Initialize last measures
	lastMeasures = malloc(numberPackages*sizeof(struct RaplFsEnergy));
	// 'readEnergy' only updates the energy field, manually initialize the 'lap' field
	readEnergy(lastMeasures);	
	for(int i=0; i<numberPackages; i++) {
		lastMeasures[i].lap = 0;
	}
	
	// Finally, launch update thread
	//printf("PThread: %ld\n", updateThread);	
	if(updateThread != 0) {
		printf("[SPECS_RAPL] WARNING: Update thread already initialized, init() has been called multiple times\n");
		return;
	}
	
	pthread_create(&updateThread, NULL, updateThreadFunction ,NULL);	
}

long long rapl_fs_energy() {
	//printf("First time: %d\n", firstTime);

	if(firstTime) {
		firstTime = false;
		init();
	}
	
	// Call update
	update();
	
	// Convert last measure to a single value and return
	return toEnergy(lastMeasures);
}
