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

uint16_t cycle_wait4 = 1; // 0..255

byte flame_min4 = 100; // 0..255
byte flame_max4 = 220; // 0..255

byte random_spark_probability4 = 2; // 0..100
byte spark_min4 = 60; // 0..255
byte spark_max4 = 255; // 0..255

byte spark_tfr4 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap4 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad4 = 40; // up radiation
uint16_t side_rad4 = 35; // sidewards radiation
uint16_t heat_cap4 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg4 = 0;
byte green_bg4 = 0;
byte blue_bg4 = 0;
byte red_bias4 = 20;
byte green_bias4 = 0;
byte blue_bias4 = 20;
int red_energy4 = 75;
int green_energy4 = 0; // 145;
int blue_energy4 = 130;

byte upside_down4 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT

byte currentEnergy4[numLeds]; // current energy level
byte nextEnergy4[numLeds]; // next energy level
byte energyMode4[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive4 = 0, // just environment, glow from nearby radiation
  torch_nop4 = 1, // no processing
  torch_spark4= 2, // slowly looses energy, moves up
  torch_spark4_temp = 3, // a spark still getting energy from the level below
};

inline void reduce4(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increase4(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t random5(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy4()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy4[i] = 0;
    nextEnergy4[i] = 0;
    energyMode4[i] = torch_passive4;
  }
}

void calcnextEnergy4()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy4[i];
      byte m = energyMode4[i];
      switch (m) {
        case torch_spark4: {
          // loose transfer up energy as long as the is any
          reduce4(e, spark_tfr4);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode4[i+ledsPerLevel] = torch_spark4_temp;
          }
          break;
        }
        case torch_spark4_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy4[i-ledsPerLevel];
          if (e2<spark_tfr4) {
            // cell below is exhausted, becomes passive
            energyMode4[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase4(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap4)>>8;
            // this cell becomes active spark
            energyMode4[i] = torch_spark4;
          }
          else {
            increase4(e, spark_tfr4);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap4)>>8;
          increase4(e, ((((int)currentEnergy4[i-1]+(int)currentEnergy4[i+1])*side_rad4)>>9) + (((int)currentEnergy4[i-ledsPerLevel]*up_rad4)>>8));
        }
        default:
          break;
      }
      nextEnergy4[i++] = e;
    }
  }
}

const uint8_t energymap4[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors4()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_down4)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy4[ei];
    currentEnergy4[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias4;
        byte g = green_bias4;
        byte b = blue_bias4;
        increase4(r, (eb*red_energy4)>>8);
        increase4(g, (eb*green_energy4)>>8);
        increase4(b, (eb*blue_energy4)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg4, green_bg4, blue_bg4);
      }
    }
  }
}

void injectRandom4()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy4[i] = random8(flame_min4, flame_max4);
    energyMode4[i] = torch_nop4;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode4[i]!=torch_spark4 && random8(100)<random_spark_probability4) {
      currentEnergy4[i] = random8(spark_min4, spark_max4);
      energyMode4[i] = torch_spark4;
    }
  }
}

uint16_t FirePurple() {
  injectRandom4();
  calcnextEnergy4();
  calcNextColors4();
  return 1;
}
