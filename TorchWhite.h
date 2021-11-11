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

uint16_t cycle_waitRMPWhite = 50; // 0..255

byte flame_minRMPWhite = 100; // 0..255
byte flame_maxRMPWhite = 220; // 0..255

byte random_spark_probabilityRMPWhite = 5; // 0..100
byte spark_minRMPWhite = 200; // 0..255
byte spark_maxRMPWhite = 255; // 0..255

byte spark_tfr7 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap7 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad7 = 30; // up speed
uint16_t side_rad7 = 0; // sidewards radiation
uint16_t heat_cap7 = 30; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg7 = 0;
byte green_bg7 = 0;
byte blue_bg7 = 0;
byte red_bias7 = 100;
byte green_bias7 = 100;
byte blue_bias7 = 100;
int red_energy7 = 255;
int green_energy7 = 255;
int blue_energy7 = 255;

byte upside_downRMPWhite = 0; // Invert effect. 0 disabled / 1 enabled

// torch mode

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT

byte currentEnergy7[numLeds]; // current energy level
byte nextEnergy7[numLeds]; // next energy level
byte energyMode7[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passive7 = 1, // just environment, glow from nearby radiation
  torch_nop7 = 1, // no processing
  torch_spark7= 2, // slowly looses energy, moves up
  torch_spark7_temp = 3, // a spark still getting energy from the level below
};

inline void reduce7(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}

inline void increase7(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t random9(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy7()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy7[i] = 0;
    nextEnergy7[i] = 0;
    energyMode7[i] = torch_passive7;
  }
}

void calcnextEnergy7()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy7[i];
      byte m = energyMode7[i];
      switch (m) {
        case torch_spark7: {
          // loose transfer up energy as long as the is any
          reduce7(e, spark_tfr7);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode7[i+ledsPerLevel] = torch_spark7_temp;
          }
          break;
        }
        case torch_spark7_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy7[i-ledsPerLevel];
          if (e2<spark_tfr7) {
            // cell below is exhausted, becomes passive
            energyMode7[i-ledsPerLevel] = torch_passive7;
            // gobble up rest of energy
            increase7(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap7)>>8;
            // this cell becomes active spark
            energyMode7[i] = torch_spark7;
          }
          else {
            increase7(e, spark_tfr7);
          }
          break;
        }
        case torch_passive7: {
          e = ((int)e*heat_cap7)>>8;
          increase7(e, ((((int)currentEnergy7[i-1]+(int)currentEnergy7[i+1])*side_rad7)>>9) + (((int)currentEnergy7[i-ledsPerLevel]*up_rad7)>>8));
        }
        default:
          break;
      }
      nextEnergy7[i++] = e;
    }
  }
}

const uint8_t energymap7[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors7()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_downRMPWhite)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy7[ei];
    currentEnergy7[ei] = e;
    if (e>250)
      leds[i] = CRGB(0, 0, 0); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap7[e>>3];
        byte r = red_bias7;
        byte g = green_bias7;
        byte b = blue_bias7;
        increase7(r, (eb*red_energy7)>>8);
        increase7(g, (eb*green_energy7)>>8);
        increase7(b, (eb*blue_energy7)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg7, green_bg7, blue_bg7);
      }
    }
  }
}

void injectRandom8()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy7[i] = random8(flame_minRMPWhite, flame_maxRMPWhite);
    energyMode7[i] = torch_nop7;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode7[i]!=torch_spark7 && random8(100)<random_spark_probabilityRMPWhite) {
      currentEnergy7[i] = random8(spark_minRMPWhite, spark_maxRMPWhite);
      energyMode7[i] = torch_spark7;
    }
  }
}

uint16_t TorchWhite() {
  injectRandom8();
  calcnextEnergy7();
  calcNextColors7();
  return 1;
}
