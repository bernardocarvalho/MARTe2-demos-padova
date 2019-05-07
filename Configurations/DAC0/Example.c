/*	
 * Simple FPGA Interface C API example application
 */

#include "NiFpga_DAC0.h"
#include <stdio.h>
//#include <stdint.h>
//#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
//#include <errno.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#define PING_MODE 0
#define FIFO1_MODE 1
#define FIFO2_MODE 2

NiFpga_Status status;
NiFpga_Session session;
int *pFIFOBuf = NULL;

void InterrHandler(int dummy) {
	status = NiFpga_Abort(session);

	/* Close the session now that we're done with it. */
	printf("Closing...\n");
	NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));

	if (pFIFOBuf != NULL) {
		free((void*) pFIFOBuf);
	}
}

int main(int argc, char ** argv) {

	signal(SIGINT, InterrHandler);
	//status=NiFpga_Initialize();
	printf("Status: %d\n", status);

	printf("Opening...\n");

	status = NiFpga_Open(NiFpga_DAC0_Bitfile, NiFpga_DAC0_Signature, "RIO0",
			NiFpga_OpenAttribute_NoRun, &session);
	if (NiFpga_IsNotError(status)) {
		printf("Opened...\n");
		if (NiFpga_IsNotError(status)) {
			printf("Status: %d\n", status);
			printf("Running...\n");
			NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));
			//sleep(1);
			printf("Status: %d\n", status);

		} else {
			printf("Error %d\n", status);
		}

		unsigned int FIFOBufSize = 65536 * 1024;
		pFIFOBuf = (int*) (malloc(FIFOBufSize * sizeof(int)));
		memset(pFIFOBuf, 0, FIFOBufSize * sizeof(int));
		size_t FIFORemaining;
		int store = 0;
		if (NiFpga_IsNotError(status)) {
			unsigned int numberOfRepetition = 1000;
			unsigned int cnt = 0;
			while ((cnt++) < numberOfRepetition) {
				//int fakeInt;
				float val = sin(2*M_PI*cnt/1000.);
				//memcpy(&fakeInt, &val, 4);

				//*(float*)(NiFpga_DAC0_ControlSgl_Mod1AO0/4)=2.5;
				status = NiFpga_WriteI32(session,
						NiFpga_DAC0_ControlSgl_Mod1AO0, *(int*)&val);
				usleep(1000);
			}
		}
		int fakeInt;
		float val = 0;
		memcpy(&fakeInt, &val, 4);
		//*(float*)(NiFpga_DAC0_ControlSgl_Mod1AO0/4)=0.0;
		status = NiFpga_WriteI32(session, NiFpga_DAC0_ControlSgl_Mod1AO0,
				fakeInt);
		status = NiFpga_Abort(session);

		/* Close the session now that we're done with it. */
		printf("Closing...\n");
		NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));

		if (pFIFOBuf != NULL) {
			free((void*) pFIFOBuf);
		}
	} else {
		printf("Error in open %d\n", status);
	}

	return status;
}
