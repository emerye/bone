/*
 * Hardware.h
 *
 *  Created on: 21 Feb 2017
 *      Author: root
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#define RESETLINE 29
#define DCLINE 28

enum cmd { COMMAND, DATA };

class Hardware {
public:
	Hardware();
	int SendSPIBlock(enum cmd cmdType, unsigned char *spiData,
			int numBytes);
    int sendByte(enum cmd cmdType, int data);

	virtual ~Hardware();
};

#endif /* HARDWARE_H_ */
