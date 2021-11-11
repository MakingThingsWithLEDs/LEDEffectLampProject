// Slightly modified version of the fire pattern from MessageTorch by Lukas Zeller:
// https://github.com/plan44/messagetorch

// The MIT License (MIT)

// Copyright (c) 2014 Lukas Zeller

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// torch parameters

uint16_t cycle_wait3 = 1; // 0..255

byte flame_min3 = 100; // 0..255
byte flame_max3 = 220; // 0..255

byte random_spark_probability3 = 2; // 0..100
byte spark_min3 = 200; // 0..255
byte spark_max3 = 255; // 0..255

byte spark_tfr3 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap3 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad3 = 40; // up radiation
uint16_t side_rad3 = 35; // sidewards radiation
uint16_t heat_cap3 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg3 = 0;
byte green_bg3 = 0;
byte blue_bg3 = 0;
byte red_bias3 = 0;
byte green_bias3 = 10;
byte blue_bias3 = 0;
int red_energy3 = 0;
int green_energy3 = 180; // 145;
int blue_energy3 = 20;

byte upside_down3 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT

byte currentEnergy3[numLeds]; // current energy level
byte nextEnergy3[numLeds]; // next energy level
byte energyMode3[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive3 = 0, // just environment, glow from nearby radiation
  torch_nop3 = 1, // no processing
  torch_spark3= 2, // slowly looses energy, moves up
  torch_spark3_temp = 3, // a spark still getting energy from the level below
};

inline void reduce3(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increase3(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomGreen(uint16_t aMinOrMax, uint16_t aMax = 0)
{
  if (aMax==0) {
    aMax = aMinOrMax;
    aMinOrMax = 0;
  }
  uint32_t r = aMinOrMax;
  aMax = aMax - aMinOrMax + 1;
  r += rand() % aMax;
  return r;
}

void resetEnergy3()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy3[i] = 0;
    nextEnergy3[i] = 0;
    energyMode3[i] = torch_passive3;
  }
}

void calcNextEnergy3()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy3[i];
      byte m = energyMode3[i];
      switch (m) {
        case torch_spark3: {
          // loose transfer up energy as long as the is any
          reduce3(e, spark_tfr3);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode3[i+ledsPerLevel] = torch_spark3_temp;
          }
          break;
        }
        case torch_spark3_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy3[i-ledsPerLevel];
          if (e2<spark_tfr3) {
            // cell below is exhausted, becomes passive
            energyMode3[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase3(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap3)>>8;
            // this cell becomes active spark
            energyMode3[i] = torch_spark3;
          }
          else {
            increase3(e, spark_tfr3);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap3)>>8;
          increase3(e, ((((int)currentEnergy3[i-1]+(int)currentEnergy3[i+1])*side_rad3)>>9) + (((int)currentEnergy3[i-ledsPerLevel]*up_rad3)>>8));
        }
        default:
          break;
      }
      nextEnergy3[i++] = e;
    }
  }
}

const uint8_t energymap3[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors3()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_down3)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy3[ei];
    currentEnergy3[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias3;
        byte g = green_bias3;
        byte b = blue_bias3;
        increase3(r, (eb*red_energy3)>>8);
        increase3(g, (eb*green_energy3)>>8);
        increase3(b, (eb*blue_energy3)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg3, green_bg3, blue_bg3);
      }
    }
  }
}

void injectRandom3()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy3[i] = random8(flame_min3, flame_max3);
    energyMode3[i] = torch_nop3;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode3[i]!=torch_spark3 && random8(100)<random_spark_probability3) {
      currentEnergy3[i] = random8(spark_min3, spark_max3);
      energyMode3[i] = torch_spark3;
    }
  }
}

uint16_t TorchGreen() {
  injectRandom3();
  calcNextEnergy3();
  calcNextColors3();
  return 1;
}
