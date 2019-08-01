/*-----------------------------------------------------------------------------
  Arduino library to control addressabled RGB(W) Led strips using the Arduino
  STM32 LibMaple core
  -----------------------------------------------------------------------------
 
  Note. 
  This library has only been tested on the WS2812B LED. It may not work with
  the older WS2812 or other types of addressable RGB LED, becuase it relies on
  a division multiple of the 72Mhz clock frequence on the STM32 SPI to
  generate the correct width T0H pulse, of 400ns +/- 150nS SPI DIV32 gives a
  pulse width of 444nS which is well within spec for the WS2812B but is
  probably too long for the WS2812 which needs a 350ns pulse for T0H
 
  This library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  See   <http://www.gnu.org/licenses/>.
  -----------------------------------------------------------------------------------------------*/

#include "AddressabLed.h"
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

/*
 * old version used 3 separate tables, one per byte of the 24 bit encoded data
 *
static const uint8_t byte0Lookup[256]={0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x93,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9A,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0x9B,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD2,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xD3,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDA,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB,0xDB};
static const uint8_t byte1Lookup[256]={0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x49,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x4D,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x69,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D,0x6D};
static const uint8_t byte2Lookup[256]={0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6,0x24,0x26,0x34,0x36,0xA4,0xA6,0xB4,0xB6};
*/

// How many bytes are present per each color
static const uint8_t bytesPerColor = 3;

// New version uses one large LUT as its faster index into sequential bytes for the GRB pattern
// Moved to cpp file to avoid multiple inclusions in different units
static const uint8_t encoderLookup[256*bytesPerColor]={
                                            0x92,0x49,0x24,0x92,0x49,0x26,0x92,0x49,0x34,0x92,0x49,0x36,0x92,0x49,0xA4,0x92,0x49,0xA6,0x92,0x49,0xB4,0x92,0x49,0xB6,0x92,0x4D,0x24,0x92,0x4D,0x26,0x92,0x4D,0x34,0x92,0x4D,0x36,0x92,0x4D,0xA4,0x92,0x4D,0xA6,0x92,0x4D,0xB4,0x92,0x4D,0xB6,0x92,0x69,0x24,0x92,0x69,0x26,0x92,0x69,0x34,0x92,0x69,0x36,0x92,0x69,0xA4,0x92,0x69,0xA6,0x92,0x69,0xB4,0x92,0x69,0xB6,0x92,0x6D,0x24,0x92,0x6D,0x26,0x92,0x6D,0x34,0x92,0x6D,0x36,0x92,0x6D,0xA4,0x92,0x6D,0xA6,0x92,0x6D,0xB4,0x92,0x6D,0xB6,0x93,0x49,0x24,0x93,0x49,0x26,0x93,0x49,0x34,0x93,0x49,0x36,0x93,0x49,0xA4,0x93,0x49,0xA6,0x93,0x49,0xB4,0x93,0x49,0xB6,0x93,0x4D,0x24,0x93,0x4D,0x26,0x93,0x4D,0x34,0x93,0x4D,0x36,0x93,0x4D,0xA4,0x93,0x4D,0xA6,0x93,0x4D,0xB4,0x93,0x4D,0xB6,0x93,0x69,0x24,0x93,0x69,0x26,0x93,0x69,0x34,0x93,0x69,0x36,0x93,0x69,0xA4,0x93,0x69,0xA6,0x93,0x69,0xB4,0x93,0x69,0xB6,0x93,0x6D,0x24,0x93,0x6D,0x26,0x93,0x6D,0x34,0x93,0x6D,0x36,0x93,0x6D,0xA4,0x93,0x6D,0xA6,0x93,0x6D,0xB4,0x93,0x6D,0xB6,0x9A,0x49,0x24,0x9A,0x49,0x26,0x9A,0x49,0x34,0x9A,0x49,0x36,0x9A,0x49,0xA4,0x9A,0x49,0xA6,0x9A,0x49,0xB4,0x9A,0x49,0xB6,0x9A,0x4D,0x24,0x9A,0x4D,0x26,0x9A,0x4D,0x34,0x9A,0x4D,0x36,0x9A,0x4D,0xA4,0x9A,0x4D,0xA6,0x9A,0x4D,0xB4,0x9A,0x4D,0xB6,0x9A,0x69,0x24,0x9A,0x69,0x26,0x9A,0x69,0x34,0x9A,0x69,0x36,0x9A,0x69,0xA4,0x9A,0x69,\
                                            0xA6,0x9A,0x69,0xB4,0x9A,0x69,0xB6,0x9A,0x6D,0x24,0x9A,0x6D,0x26,0x9A,0x6D,0x34,0x9A,0x6D,0x36,0x9A,0x6D,0xA4,0x9A,0x6D,0xA6,0x9A,0x6D,0xB4,0x9A,0x6D,0xB6,0x9B,0x49,0x24,0x9B,0x49,0x26,0x9B,0x49,0x34,0x9B,0x49,0x36,0x9B,0x49,0xA4,0x9B,0x49,0xA6,0x9B,0x49,0xB4,0x9B,0x49,0xB6,0x9B,0x4D,0x24,0x9B,0x4D,0x26,0x9B,0x4D,0x34,0x9B,0x4D,0x36,0x9B,0x4D,0xA4,0x9B,0x4D,0xA6,0x9B,0x4D,0xB4,0x9B,0x4D,0xB6,0x9B,0x69,0x24,0x9B,0x69,0x26,0x9B,0x69,0x34,0x9B,0x69,0x36,0x9B,0x69,0xA4,0x9B,0x69,0xA6,0x9B,0x69,0xB4,0x9B,0x69,0xB6,0x9B,0x6D,0x24,0x9B,0x6D,0x26,0x9B,0x6D,0x34,0x9B,0x6D,0x36,0x9B,0x6D,0xA4,0x9B,0x6D,0xA6,0x9B,0x6D,0xB4,0x9B,0x6D,0xB6,0xD2,0x49,0x24,0xD2,0x49,0x26,0xD2,0x49,0x34,0xD2,0x49,0x36,0xD2,0x49,0xA4,0xD2,0x49,0xA6,0xD2,0x49,0xB4,0xD2,0x49,0xB6,0xD2,0x4D,0x24,0xD2,0x4D,0x26,0xD2,0x4D,0x34,0xD2,0x4D,0x36,0xD2,0x4D,0xA4,0xD2,0x4D,0xA6,0xD2,0x4D,0xB4,0xD2,0x4D,0xB6,0xD2,0x69,0x24,0xD2,0x69,0x26,0xD2,0x69,0x34,0xD2,0x69,0x36,0xD2,0x69,0xA4,0xD2,0x69,0xA6,0xD2,0x69,0xB4,0xD2,0x69,0xB6,0xD2,0x6D,0x24,0xD2,0x6D,0x26,0xD2,0x6D,0x34,0xD2,0x6D,0x36,0xD2,0x6D,0xA4,0xD2,0x6D,0xA6,0xD2,0x6D,0xB4,0xD2,0x6D,0xB6,0xD3,0x49,0x24,0xD3,0x49,0x26,0xD3,0x49,0x34,0xD3,0x49,0x36,0xD3,0x49,0xA4,0xD3,0x49,0xA6,0xD3,0x49,0xB4,0xD3,0x49,0xB6,0xD3,0x4D,0x24,0xD3,0x4D,0x26,0xD3,0x4D,0x34,0xD3,\
                                            0x4D,0x36,0xD3,0x4D,0xA4,0xD3,0x4D,0xA6,0xD3,0x4D,0xB4,0xD3,0x4D,0xB6,0xD3,0x69,0x24,0xD3,0x69,0x26,0xD3,0x69,0x34,0xD3,0x69,0x36,0xD3,0x69,0xA4,0xD3,0x69,0xA6,0xD3,0x69,0xB4,0xD3,0x69,0xB6,0xD3,0x6D,0x24,0xD3,0x6D,0x26,0xD3,0x6D,0x34,0xD3,0x6D,0x36,0xD3,0x6D,0xA4,0xD3,0x6D,0xA6,0xD3,0x6D,0xB4,0xD3,0x6D,0xB6,0xDA,0x49,0x24,0xDA,0x49,0x26,0xDA,0x49,0x34,0xDA,0x49,0x36,0xDA,0x49,0xA4,0xDA,0x49,0xA6,0xDA,0x49,0xB4,0xDA,0x49,0xB6,0xDA,0x4D,0x24,0xDA,0x4D,0x26,0xDA,0x4D,0x34,0xDA,0x4D,0x36,0xDA,0x4D,0xA4,0xDA,0x4D,0xA6,0xDA,0x4D,0xB4,0xDA,0x4D,0xB6,0xDA,0x69,0x24,0xDA,0x69,0x26,0xDA,0x69,0x34,0xDA,0x69,0x36,0xDA,0x69,0xA4,0xDA,0x69,0xA6,0xDA,0x69,0xB4,0xDA,0x69,0xB6,0xDA,0x6D,0x24,0xDA,0x6D,0x26,0xDA,0x6D,0x34,0xDA,0x6D,0x36,0xDA,0x6D,0xA4,0xDA,0x6D,0xA6,0xDA,0x6D,0xB4,0xDA,0x6D,0xB6,0xDB,0x49,0x24,0xDB,0x49,0x26,0xDB,0x49,0x34,0xDB,0x49,0x36,0xDB,0x49,0xA4,0xDB,0x49,0xA6,0xDB,0x49,0xB4,0xDB,0x49,0xB6,0xDB,0x4D,0x24,0xDB,0x4D,0x26,0xDB,0x4D,0x34,0xDB,0x4D,0x36,0xDB,0x4D,0xA4,0xDB,0x4D,0xA6,0xDB,0x4D,0xB4,0xDB,0x4D,0xB6,0xDB,0x69,0x24,0xDB,0x69,0x26,0xDB,0x69,0x34,0xDB,0x69,0x36,0xDB,0x69,0xA4,0xDB,0x69,0xA6,0xDB,0x69,0xB4,0xDB,0x69,0xB6,0xDB,0x6D,0x24,0xDB,0x6D,0x26,0xDB,0x6D,0x34,0xDB,0x6D,0x36,0xDB,0x6D,0xA4,0xDB,0x6D,0xA6,0xDB,0x6D,0xB4,0xDB,0x6D,0xB6};

AddressabLed_Base::AddressabLed_Base(uint16_t number_of_leds) : pixels(NULL)
{
  updateLength(number_of_leds);
}

AddressabLed_Base::~AddressabLed_Base() 
{
  if(pixels)
  {
    free(pixels);
  }
  SPI.end();
}

void AddressabLed_Base::begin(void)
{
  if (!begun)
  {
    SPI.setClockDivider(ADDRESSABLED_SPI_DIVISOR);
    SPI.begin();
    begun = true;
  }
}

void AddressabLed_Base::show(boolean cloneBuffer) 
{
  // Start the DMA transfer of the current pixel buffer to the LEDs and return immediately.
  SPI.dmaSendAsync(pixels,numBytes);

  // Sometimes there is no need to copy the last / current buffer to the other half of the
  // double buffer (e.g. when the program writes the whole buffer every time).
  // If this feature is needed, set cloneBuffer to true.
  
  if (pixels==doubleBuffer)
  {
    // pixels was using the first buffer
    pixels = doubleBuffer+numBytes; // set pixels to second buffer

    if (cloneBuffer)
    {
      memcpy(pixels,doubleBuffer,numBytes);// copy first buffer to second buffer
    }
  }
  else
  {
    // pixels was using the second buffer      
    pixels = doubleBuffer; // set pixels to first buffer
    
    if (cloneBuffer)
    {
      memcpy(pixels,doubleBuffer+numBytes,numBytes); // copy second buffer to first buffer
    }
  }    
}

void AddressabLed_Base::clear() 
{
  uint8_t * bptr= pixels+1;// Note first byte in the buffer is a preable and is always zero. hence the +1
  uint8_t *tPtr;

  for(int i=0;i< (numLEDs * getColorsPerPixel());i++)
  {
    tPtr = (uint8_t *)encoderLookup;
    *bptr++ = *tPtr++;
    *bptr++ = *tPtr++;
    *bptr++ = *tPtr++;
  }
}

void AddressabLed_Base::updateLength(uint16_t n)
{
  if(doubleBuffer)
  {
    free(doubleBuffer);
  }

  numBytes = n * getColorsPerPixel() * bytesPerColor + 2; // 9 encoded bytes per pixel when RGB, 12 when RGBW.
  // 1 byte empty peamble to fix issue with SPI MOSI and on byte at the end to clear down MOSI
  
  if((doubleBuffer = (uint8_t *)malloc(numBytes*2)))
  {
    numLEDs = n;
    pixels = doubleBuffer;
    // Only need to init the part of the double buffer which will be interacted with by the API e.g. setPixelColor
    *pixels=0;//clear the preamble byte
    *(pixels+numBytes-1)=0;// clear the post send cleardown byte.
    clear();// Set the encoded data to all encoded zeros
    
    // Since cloning the buffer is optional, the second buffer must be cloned now
    memcpy(pixels,doubleBuffer+numBytes,numBytes);
  } 
  else
  {
    numLEDs = numBytes = 0;
  }
}

uint16_t AddressabLed_Base::numPixels(void) const {
  return numLEDs;
}

void AddressabLed_RGB::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t *bptr = pixels + n * colorsPerPixel * bytesPerColor +1;
  uint8_t *tPtr = (uint8_t *)encoderLookup + g * bytesPerColor;
   
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;

  tPtr = (uint8_t *)encoderLookup + r * bytesPerColor;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;

  tPtr = (uint8_t *)encoderLookup + b * bytesPerColor;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
}

void AddressabLed_RGB::setPixelColor(uint16_t n, uint32_t c)
{
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >> 8);
  uint8_t b = (uint8_t)(c);

    setPixelColor(n, r, g, b);
}

uint32_t AddressabLed_RGB::Color(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t result = r;
  result = (result << 8) | g;
  result = (result << 8) | b;
  return result;
}

void AddressabLed_RGBW::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  uint8_t *bptr = pixels + n * colorsPerPixel * bytesPerColor +1;
  uint8_t *tPtr = (uint8_t *)encoderLookup + g * bytesPerColor;

  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;

  tPtr = (uint8_t *)encoderLookup + r * bytesPerColor;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;   

  tPtr = (uint8_t *)encoderLookup + b * bytesPerColor;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;

  tPtr = (uint8_t *)encoderLookup + w * bytesPerColor;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
  *bptr++ = *tPtr++;
}

void AddressabLed_RGBW::setPixelColor(uint16_t n, uint32_t c)
{
  uint8_t w = (uint8_t)(c >> 24);
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >> 8);
  uint8_t b = (uint8_t)(c);

  setPixelColor(n, r, g, b, w);
}

uint32_t AddressabLed_RGBW::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  uint32_t result = w;
  result = (result << 8) | r;
  result = (result << 8) | g;
  result = (result << 8) | b;
  return result;
}
