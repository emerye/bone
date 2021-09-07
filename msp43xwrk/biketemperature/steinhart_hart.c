/*
 * Calculate NTC temperature using Steinhart-Hart equation
 *
 */

#include <math.h>
#include "steinhart_hart.h"

/* 4700 ohm at 25 degree NTC
* 1/T = C1 + C2 * ln(R) + C3 * ln(R)3
*
*
*/
double calcTemperature_fromRes(double r)
{
    double a = 0.001307395;
    double b = 0.000234655;
    double c = 0.000000103;

    double bterm, logr;
    double cterm, temp, tkelvin;
    volatile double degc;

    logr = log(r);
    bterm = b * logr;
    temp = logr * logr * logr;
    cterm = c * temp;
    temp = a + bterm + cterm;
    tkelvin = 1 / temp;
    temp = kelvin_to_c(tkelvin);
    return celsius_to_f(temp);
}

double kelvin_to_c(double kelvin)
{
    return (kelvin - 273.15);
}

double celsius_to_f(double celsius)
{
    return (celsius * 1.8 + 32);
}

double calccurrentTemperature(double adcReading)
{
    double referenceVolts = 1.207;
    double thval;
    double tempdegC;

    thval = 5600 * ((referenceVolts/adcReading)-1);
    tempdegC = calcTemperature_fromRes(thval);
    return tempdegC;
}
