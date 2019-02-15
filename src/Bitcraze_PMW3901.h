/* PMW3901 Arduino driver
 * Copyright (c) 2017 Bitcraze AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __BITCRAZE_PMW3901_H__
#define __BITCRAZE_PMW3901_H__

#include "Arduino.h"

#include <stdint.h>

typedef struct __attribute__((__packed__))
{
	uint8_t motion;
	uint8_t observation;
	int16_t delta[2];
	uint8_t squal;
	uint8_t sum;
	uint8_t rawMax;
	uint8_t rawMin;
	uint8_t shutterUpper;
	uint8_t shutterLower;
} Burst;

class Bitcraze_PMW3901 {
public:
  Bitcraze_PMW3901(uint8_t cspin);

  bool begin(void);
  bool connected() { return state; };
  
  Burst* readBurst();
  Burst* getBurst() { return &burst; };

  void dump();

private:
  uint8_t _cs;
  uint8_t state = 0;

  Burst burst;

  void open();
  void close();
  void registerWrite(uint8_t reg, uint8_t value);
  uint8_t registerRead(uint8_t reg);
  bool registerRead(uint8_t *buf, uint8_t len);
  void initRegisters(void);
};

#endif //__BITCRAZE_PMW3901_H__