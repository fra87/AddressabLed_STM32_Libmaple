/*--------------------------------------------------------------------
  The ADDRESSABLED library is free software: you can redistribute
  it and/or modify it under the terms of the GNU Lesser General
  Public License as published by the Free Software Foundation,
  either version 3 of the License, or (at your option) any later
  version.

  It is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  See <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#ifndef ADDRESSABLED_H
#define ADDRESSABLED_H

#include <Arduino.h>

// Set the SPI clock divisor to as close to 400ns as we can;
// the WS2812 spec allows for +/- 150ns
// Selecting a frequency here means we can have a frequency
// between the specified one and half of it.
// In fact, the selected divider is the smallest n so that
// f_SPI > F_CPU / 2^n
// A target of 400ns corresponds to 2.5MHz; let's choose 3.5 MHz,
// which corresponds to:
// F_CPU  | n (divider) | Bit time
//-----------------------------------
// 72 MHz |   5 (32)    | 444 ns
// 64 MHz |   5 (32)    | 500 ns
// 48 MHz |   4 (16)    | 333 ns
// 40 MHz |   4 (16)    | 400 ns
// 36 MHz |   4 (16)    | 444 ns
// 24 MHz |   3 ( 8)    | 333 ns
// 16 MHz |   3 ( 8)    | 500 ns
//  8 MHz |   2 ( 4)    | 500 ns

#define ADDRESSABLED_SPI_TARGET_FREQUENCY 3500000

/**
  Generic base class for the LEDs
*/
class AddressabLed_Base
{
 public:

  // Constructor
  // number_of_leds: number of LEDs in the strip
  AddressabLed_Base(uint16_t number_of_leds);
  
  // Destructor
  ~AddressabLed_Base();
  
  // Initialize the class item
  void begin(void);
  
  // Show the current buffer
  // cloneBuffer: true if you want to clone the buffer
  // NOTE: if false, the status of the buffer is not
  // the current one, but you may assume it is unknown
  void show(boolean cloneBuffer);
  
  // Writes the encoded 0 to all pixels
  void clear();
  
  // Updates the string length, writing 0 to the whole string
  void updateLength(uint16_t n);
  
  // Current number of LEDs present on the strip
  uint16_t numPixels(void) const;
  
 protected:
  // Holds the current LED color values, which the external API calls
  // interact with 9 or bytes per pixel + start + end empty bytes
  uint8_t *pixels;
  
  // Method to be overridden
  // returns the number of colors per pixel
  virtual uint8_t getColorsPerPixel(void) const = 0;
  
 private:

  // true if begin() previously called
  boolean begun;
  
  // Number of LEDs in strip
  uint16_t numLEDs;
  
  // Size of 'pixels' buffer
  uint16_t numBytes;
  
  // Holds the start of the double buffer (1 buffer for async DMA transfer
  // and one for the API interaction.
  uint8_t *doubleBuffer;
};

class AddressabLed_RGB : public AddressabLed_Base
{
 public:

  // Constructor
  // number_of_leds: number of LEDs in the strip
  AddressabLed_RGB(uint16_t number_of_leds) : AddressabLed_Base(number_of_leds) {}
  
  // Sets pixel with ID n with the specified color
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  
  // Sets pixel with ID n with the specified color
  // c format is the same specified in static function Color
  void setPixelColor(uint16_t n, uint32_t c);
  
  // Create a single variable representing the color, in format 00RRGGBB
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
  
 protected:
  // Number of colors per pixel
  static const uint8_t colorsPerPixel = 3;
  
  // Overridden method to get static value
  uint8_t getColorsPerPixel(void) const { return colorsPerPixel; }
};

class AddressabLed_RGBW : public AddressabLed_Base
{
 public:

  // Constructor
  // number_of_leds: number of LEDs in the strip
  AddressabLed_RGBW(uint16_t number_of_leds) : AddressabLed_Base(number_of_leds) {}
  
  // Sets pixel with ID n with the specified color (W = 0)
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) { setPixelColor(n,r,g,b,0); }
  
  // Sets pixel with ID n with the specified color
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  
  // Sets pixel with ID n with the specified color
  // c format is the same specified in static function Color
  void setPixelColor(uint16_t n, uint32_t c);
    
  // Create a single variable representing the color, in format WWRRGGBB
  // In this case, W = 0
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) { return Color(r,g,b,0); }
    
  // Create a single variable representing the color, in format WWRRGGBB
  static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  
 protected:
  // Number of colors per pixel
  static const uint8_t colorsPerPixel = 4;
  
  // Overridden method to get static value
  uint8_t getColorsPerPixel(void) const { return colorsPerPixel; }
};

#endif // ADDRESSABLED_H
