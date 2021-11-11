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

uint16_t cycle_wait6 = 1; // 0..255

byte flame_min6 = 70; // 0..255
byte flame_max6 = 250; // 0..255

byte random_spark_probability6 = 1; // 0..100
byte spark_min6 = 60; // 0..255
byte spark_max6 = 255; // 0..255

byte spark_tfr6 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap6 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad6 = 50; // up radiation
uint16_t side_rad6 = 40; // sidewards radiation
uint16_t heat_cap6 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg6 = 0;
byte green_bg6 = 0;
byte blue_bg6 = 0;
byte red_bias6 = 35;
byte green_bias6 = 90;
byte blue_bias6 = 0;
int red_energy6 = 100;
int green_energy6 = 120;
int blue_energy6 = 0;

byte upside_down6 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

byte currentEnergy6[numLeds]; // current energy level
byte nextEnergy6[numLeds]; // next energy level
byte energyMode6[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive6 = 0, // just environment, glow from nearby radiation
  torch_nop6 = 1, // no processing
  torch_spark6= 2, // slowly looses energy, moves up
  torch_spark6_temp = 3, // a spark still getting energy from the level below
};

inline void reduce6(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increase6(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t random7(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy6()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy6[i] = 0;
    nextEnergy6[i] = 0;
    energyMode6[i] = torch_passive6;
  }
}

void calcnextEnergy6()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy6[i];
      byte m = energyMode6[i];
      switch (m) {
        case torch_spark6: {
          // loose transfer up energy as long as the is any
          reduce6(e, spark_tfr6);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode6[i+ledsPerLevel] = torch_spark6_temp;
          }
          break;
        }
        case torch_spark6_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy6[i-ledsPerLevel];
          if (e2<spark_tfr6) {
            // cell below is exhausted, becomes passive
            energyMode6[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase6(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap6)>>8;
            // this cell becomes active spark
            energyMode6[i] = torch_spark6;
          }
          else {
            increase6(e, spark_tfr6);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap6)>>8;
          increase6(e, ((((int)currentEnergy6[i-1]+(int)currentEnergy6[i+1])*side_rad6)>>9) + (((int)currentEnergy6[i-ledsPerLevel]*up_rad6)>>8));
        }
        default:
          break;
      }
      nextEnergy6[i++] = e;
    }
  }
}

const uint8_t energymap6[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors6()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_down6)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy6[ei];
    currentEnergy6[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias6;
        byte g = green_bias6;
        byte b = blue_bias6;
        increase6(r, (eb*red_energy6)>>8);
        increase6(g, (eb*green_energy6)>>8);
        increase6(b, (eb*blue_energy6)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg6, green_bg6, blue_bg6);
      }
    }
  }
}

void injectRandom6()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy6[i] = random8(flame_min6, flame_max6);
    energyMode6[i] = torch_nop6;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode6[i]!=torch_spark6 && random8(100)<random_spark_probability6) {
      currentEnergy6[i] = random8(spark_min6, spark_max6);
      energyMode6[i] = torch_spark6;
    }
  }
}

uint16_t TorchChemical() {
  injectRandom6();
  calcnextEnergy6();
  calcNextColors6();
  return 1;
}
