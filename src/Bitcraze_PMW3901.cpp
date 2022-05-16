/* PMW3901 Arduino driver
 * Copyright (c) 2017 Bitcraze AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Bitcraze_PMW3901.h"

#include <SPI.h>

Bitcraze_PMW3901::Bitcraze_PMW3901(uint8_t cspin) {
	_cs = cspin;
}

boolean Bitcraze_PMW3901::begin(void) {
	if (state) return true;
  // Setup SPI port
  SPI.begin();
  pinMode(_cs, OUTPUT);

   // Test the SPI communication, checking chipId and inverse chipId
  uint8_t chipId = registerRead(0x00);
  uint8_t dIpihc = registerRead(0x5F);

  if (chipId != 0x49 && dIpihc != 0xB8)
  {
	  //Serial.println("No Flow Sensor Found!");
	  return false;
  }

  initRegisters();

  state = 1;

  return state;
}

// Functional access
Burst* Bitcraze_PMW3901::readBurst()
{
	if (!state && !begin()) return nullptr;

	open();

	SPI.transfer(0x16);	//Burst register
	delayMicroseconds(35);

	for (int i = 0; i < sizeof(Burst); i++)
	{
		((uint8_t*)(&burst))[i] = SPI.transfer(0x00);
	}

	close(); 
	
	return &burst;
}

// Low level register access
void Bitcraze_PMW3901::open()
{
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

	digitalWrite(_cs, LOW);
}

void Bitcraze_PMW3901::close()
{
	digitalWrite(_cs, HIGH);

	SPI.endTransaction();
}

void Bitcraze_PMW3901::registerWrite(uint8_t reg, uint8_t value) 
{
  reg |= 0x80u;

  open();

  delayMicroseconds(50);
  SPI.transfer(reg);
  SPI.transfer(value);
  delayMicroseconds(50);

  close();

  delayMicroseconds(200);
}

uint8_t Bitcraze_PMW3901::registerRead(uint8_t reg) {

  reg &= ~0x80u;

  open();

  SPI.transfer(reg);
  delayMicroseconds(50);
  uint8_t value = SPI.transfer(0);

  close();

  return value;
}

bool Bitcraze_PMW3901::registerRead(uint8_t * buf, uint8_t len) 
{
	open();

	for (int i = 0; i < len; i++)
	{
		SPI.transfer(buf[i] & 0x7F);
		delayMicroseconds(35);
		buf[i] = SPI.transfer(0);
	}

	close();

	return true;
}

// Performance optimisation registers
void Bitcraze_PMW3901::initRegisters()
{
  registerWrite(0x7F, 0x00);
  registerWrite(0x61, 0xAD);
  registerWrite(0x7F, 0x03);
  registerWrite(0x40, 0x00);
  registerWrite(0x7F, 0x05);
  registerWrite(0x41, 0xB3);
  registerWrite(0x43, 0xF1);
  registerWrite(0x45, 0x14);
  registerWrite(0x5B, 0x32);
  registerWrite(0x5F, 0x34);
  registerWrite(0x7B, 0x08);
  registerWrite(0x7F, 0x06);
  registerWrite(0x44, 0x1B);
  registerWrite(0x40, 0xBF);
  registerWrite(0x4E, 0x3F);
  registerWrite(0x7F, 0x08);
  registerWrite(0x65, 0x20);
  registerWrite(0x6A, 0x18);
  registerWrite(0x7F, 0x09);
  registerWrite(0x4F, 0xAF);
  registerWrite(0x5F, 0x40);
  registerWrite(0x48, 0x80);
  registerWrite(0x49, 0x80);
  registerWrite(0x57, 0x77);
  registerWrite(0x60, 0x78);
  registerWrite(0x61, 0x78);
  registerWrite(0x62, 0x08);
  registerWrite(0x63, 0x50);
  registerWrite(0x7F, 0x0A);
  registerWrite(0x45, 0x60);
  registerWrite(0x7F, 0x00);
  registerWrite(0x4D, 0x11);
  registerWrite(0x55, 0x80);
  registerWrite(0x74, 0x1F);
  registerWrite(0x75, 0x1F);
  registerWrite(0x4A, 0x78);
  registerWrite(0x4B, 0x78);
  registerWrite(0x44, 0x08);
  registerWrite(0x45, 0x50);
  registerWrite(0x64, 0xFF);
  registerWrite(0x65, 0x1F);
  registerWrite(0x7F, 0x14);
  registerWrite(0x65, 0x60);
  registerWrite(0x66, 0x08);
  registerWrite(0x63, 0x78);
  registerWrite(0x7F, 0x15);
  registerWrite(0x48, 0x58);
  registerWrite(0x7F, 0x07);
  registerWrite(0x41, 0x0D);
  registerWrite(0x43, 0x14);
  registerWrite(0x4B, 0x0E);
  registerWrite(0x45, 0x0F);
  registerWrite(0x44, 0x42);
  registerWrite(0x4C, 0x80);
  registerWrite(0x7F, 0x10);
  registerWrite(0x5B, 0x02);
  registerWrite(0x7F, 0x07);
  registerWrite(0x40, 0x41);
  registerWrite(0x70, 0x00);

  delay(100);

  registerWrite(0x32, 0x44);
  registerWrite(0x7F, 0x07);
  registerWrite(0x40, 0x40);
  registerWrite(0x7F, 0x06);
  registerWrite(0x62, 0xf0);
  registerWrite(0x63, 0x00);
  registerWrite(0x7F, 0x0D);
  registerWrite(0x48, 0xC0);
  registerWrite(0x6F, 0xd5);
  registerWrite(0x7F, 0x00);
  registerWrite(0x5B, 0xa0);
  registerWrite(0x4E, 0xA8);
  registerWrite(0x5A, 0x50);
  registerWrite(0x40, 0x80);
}

void Bitcraze_PMW3901::setLed(bool ledOn)
{
  delay(200);
  registerWrite(0x7f, 0x14);
  registerWrite(0x6f, ledOn ? 0x1c : 0x00);
  registerWrite(0x7f, 0x00);
}

void Bitcraze_PMW3901::dump()
{
	Serial.print("\tM: ");
	Serial.print(burst.motion);
	Serial.print("\tO: ");
	Serial.print(burst.observation);
	Serial.print("\tX: ");
	Serial.print(burst.delta[0]);
	Serial.print("\tY: ");
	Serial.print(burst.delta[1]);
	Serial.print("\tQ: ");
	Serial.print(burst.squal);
	Serial.print("\tRS: ");
	Serial.print(burst.sum);
	Serial.print("\tRH: ");
	Serial.print(burst.rawMax);
	Serial.print("\tRL: ");
	Serial.print(burst.rawMin);
	Serial.print("\tSH: ");
	Serial.print(burst.shutterUpper);
	Serial.print("\tSL: ");
	Serial.print(burst.shutterLower);
	Serial.println();
}