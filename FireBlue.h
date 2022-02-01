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

uint16_t cycle_wait2 = 1; // 0..255

byte flame_min2 = 100; // 0..255
byte flame_max2 = 220; // 0..255

byte random_spark_probability2 = 2; // 0..100
byte spark_min2 = 200; // 0..255
byte spark_max2 = 255; // 0..255

byte spark_tfr2 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap2 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad2 = 40; // up radiation
uint16_t side_rad2 = 35; // sidewards radiation
uint16_t heat_cap2 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg2 = 0;
byte green_bg2 = 0; //0
byte blue_bg2 = 0;
byte red_bias2 = 0;  //10
byte green_bias2 = 0;
byte blue_bias2 = 10;
int red_energy2 = 0;  //180
int green_energy2 = 0; // 80;
int blue_energy2 = 220;

byte upside_down2 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT

byte currentEnergy2[numLeds]; // current energy level
byte nextEnergy2[numLeds]; // next energy level
byte energyMode2[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive2 = 0, // just environment, glow from nearby radiation
  torch_nop2 = 1, // no processing
  torch_spark2= 2, // slowly looses energy, moves up
  torch_spark2_temp = 3, // a spark still getting energy from the level below
};

inline void reduce2(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increase2(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomBlue(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy2()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy2[i] = 0;
    nextEnergy2[i] = 0;
    energyMode2[i] = torch_passive2;
  }
}

void calcNextEnergy2()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy2[i];
      byte m = energyMode2[i];
      switch (m) {
        case torch_spark2: {
          // loose transfer up energy as long as the is any
          reduce2(e, spark_tfr2);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode2[i+ledsPerLevel] = torch_spark2_temp;
          }
          break;
        }
        case torch_spark2_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy2[i-ledsPerLevel];
          if (e2<spark_tfr2) {
            // cell below is exhausted, becomes passive
            energyMode2[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase2(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap2)>>8;
            // this cell becomes active spark
            energyMode2[i] = torch_spark2;
          }
          else {
            increase2(e, spark_tfr2);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap2)>>8;
          increase2(e, ((((int)currentEnergy2[i-1]+(int)currentEnergy2[i+1])*side_rad2)>>9) + (((int)currentEnergy2[i-ledsPerLevel]*up_rad2)>>8));
        }
        default:
          break;
      }
      nextEnergy2[i++] = e;
    }
  }
}

const uint8_t energymap2[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors2()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_down2)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy2[ei];
    currentEnergy2[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias2;
        byte g = green_bias2;
        byte b = blue_bias2;
        increase2(r, (eb*red_energy2)>>8);
        increase2(g, (eb*green_energy2)>>8);
        increase2(b, (eb*blue_energy2)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg2, green_bg2, blue_bg2);
      }
    }
  }
}

void injectRandom2()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy2[i] = random8(flame_min2, flame_max2);
    energyMode2[i] = torch_nop2;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode2[i]!=torch_spark2 && random8(100)<random_spark_probability2) {
      currentEnergy2[i] = random8(spark_min2, spark_max2);
      energyMode2[i] = torch_spark2;
    }
  }
}

uint16_t FireBlue() {
  injectRandom2();
  calcNextEnergy2();
  calcNextColors2();
  return 1;
}
