/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * StopWatchMode.h
 *
 * Simple stopwatch application that supports counting up and split time.
 *
 * September 2014
 * E. Chen
 *
 ******************************************************************************/
#include <stdint.h>
#include <msp430fr4133.h>

#ifndef STOPWATCHMODE_H_
#define STOPWATCHMODE_H_

extern volatile unsigned char * Centiseconds;      // Store seconds in the backup RAM module
extern volatile unsigned char * Seconds;      // Store minutes in the backup RAM module
extern volatile unsigned char * Minutes;        // Store hours in the backup RAM module
extern volatile unsigned char * count;        // Store hours in the backup RAM module
extern volatile unsigned char * init;        // Store hours in the backup RAM module

#define STOPWATCH_MODE        1

void stopWatch(void);
void stopWatchModeInit(void);
void Inc_RTC(void);
void resetStopWatch(void);
void displayTime(void);
void PMM_External_1_2VRef(uint8_t mode);

#endif /* STOPWATCHMODE_H_ */
