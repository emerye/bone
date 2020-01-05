
#ifndef SENSORS_H_
#define SENSORS_H_


class Sensors {
	int i2cHandle;
		int i2cAddress;

public:

	Sensors(int i2cHandle, int i2cAddress);
	float getLM75Temperature();
	virtual ~Sensors();
};


#endif /* SENSORS_H_ */
