#include <stdio.h>
#include "rapl.h"

int main() {
	
        long long e0, e1;

        //rapl_monitor_start();	// CALLED ONCE

        e0 = rapl_energy();

		while(!hasLooped()) {
			int iter = 2000;
			long long acc = 0LL;
			for(int i=0; i<iter; i++) {
				for(int j=0; j<iter; j++) {
					for(int k=0; k<iter; k++) {
						acc += 1;
					}
				}
			}			
			
			e1 = rapl_energy();
			printf("Energy consumed: %llduJ\n", e1 - e0);
		}

		e1 = rapl_energy();
		printf("Finally, energy consumed: %llduJ\n", e1 - e0);

        return 0;
}
