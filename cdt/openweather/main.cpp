/*
 * main.c
 *
 *  Created on: 16 Jul 2017
 *      Author: andy
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *script = "/home/andy/bone/src/scripts/openWeather/getFre.pl";
const char *dataFile = "/home/andy/bone/src/scripts/openWeather/wdata.txt";

int parseData(const char *dataFile, const char *key, char *value) {
	FILE *fp;
	char fileLine[512];
	const char *pLine;
	const char *pstr = NULL;
	const char *pkey = NULL;
	int retValue = 0xFF;

	fp = fopen(dataFile, "r");
	if (fp != NULL) {
		do {
			fgets(fileLine, sizeof(fileLine), fp);
			pLine = fileLine;
			pstr = strstr((const char *) fileLine, key);
			if (pstr != NULL) {
				pkey = strstr((const char *) fileLine, "=");
				strcpy(value, pkey + 1);
				retValue = 0;
			}
		} while (feof(fp) == 0);

	}
	fclose(fp);
	return retValue;
}

int main(int argc, char **argv) {
	int status;
	char value[512];

	printf("Number of args %d\n", argc);
	printf("Argument %s\n", argv[0]);

	status = system(script);
	printf("Return code %d\n", status);
	status = parseData(dataFile, "temperature=", value);
	if (!status) {
		printf("Key value %s\n", value);
	} else {
		printf("Failed to read temperature");
	}
}

