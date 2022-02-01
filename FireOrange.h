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

uint16_t cycle_wait5 = 1; // 0..255

byte flame_min5 = 100; // 0..255
byte flame_max5 = 220; // 0..255

byte random_spark_probability5 = 2; // 0..100
byte spark_min5 = 200; // 0..255
byte spark_max5 = 255; // 0..255

byte spark_tfr5 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap5 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad5 = 40; // up radiation
uint16_t side_rad5 = 35; // sideward radiation
uint16_t heat_cap5 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg5 = 0;
byte green_bg5 = 0;
byte blue_bg5 = 0;
byte red_bias5 = 70;
byte green_bias5 = 35;
byte blue_bias5 = 0;
int red_energy5 = 220;
int green_energy5 = 20; // 145;
int blue_energy5 = 0;

byte upside_down5 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

byte currentEnergy5[numLeds]; // current energy level
byte nextEnergy5[numLeds]; // next energy level
byte energyMode5[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive5 = 0, // just environment, glow from nearby radiation
  torch_nop5 = 1, // no processing
  torch_spark5= 2, // slowly looses energy, moves up
  torch_spark5_temp = 3, // a spark still getting energy from the level below
};

inline void reduce5(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increase5(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t random6(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy5()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy5[i] = 0;
    nextEnergy5[i] = 0;
    energyMode5[i] = torch_passive5;
  }
}

void calcnextEnergy5()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy5[i];
      byte m = energyMode5[i];
      switch (m) {
        case torch_spark5: {
          // loose transfer up energy as long as the is any
          reduce5(e, spark_tfr5);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode5[i+ledsPerLevel] = torch_spark5_temp;
          }
          break;
        }
        case torch_spark5_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy5[i-ledsPerLevel];
          if (e2<spark_tfr5) {
            // cell below is exhausted, becomes passive
            energyMode5[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase5(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap5)>>8;
            // this cell becomes active spark
            energyMode5[i] = torch_spark5;
          }
          else {
            increase5(e, spark_tfr5);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap5)>>8;
          increase5(e, ((((int)currentEnergy5[i-1]+(int)currentEnergy5[i+1])*side_rad5)>>9) + (((int)currentEnergy5[i-ledsPerLevel]*up_rad5)>>8));
        }
        default:
          break;
      }
      nextEnergy5[i++] = e;
    }
  }
}

const uint8_t energymap5[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors5()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_down5)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy5[ei];
    currentEnergy5[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias5;
        byte g = green_bias5;
        byte b = blue_bias5;
        increase5(r, (eb*red_energy5)>>8);
        increase5(g, (eb*green_energy5)>>8);
        increase5(b, (eb*blue_energy5)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg5, green_bg5, blue_bg5);
      }
    }
  }
}

void injectRandom5()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy5[i] = random8(flame_min5, flame_max5);
    energyMode5[i] = torch_nop5;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode5[i]!=torch_spark5 && random8(100)<random_spark_probability5) {
      currentEnergy5[i] = random8(spark_min5, spark_max5);
      energyMode5[i] = torch_spark5;
    }
  }
}

uint16_t FireOrange() {
  injectRandom5();
  calcnextEnergy5();
  calcNextColors5();
  return 1;
}
