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
uint16_t cycle_wait8 = 1; // 0..255

byte flame_min8 = 100; // 0..255
byte flame_max8 = 220; // 0..255

byte random_spark_probability8 = 2; // 0..100  // Spark amount keep low
byte spark_min8 = 60; // 0..255
byte spark_max8 = 255; // 0..255

byte spark_tfr8 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_cap8 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_rad8 = 50; // up radiation
uint16_t side_rad8 = 40; // sidewards radiation
uint16_t heat_cap8 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bg8 = 0;
byte green_bg8 = 0;
byte blue_bg8 = 0;
byte red_bias8 = 0;
byte green_bias8 = 0;
byte blue_bias8 = 0;
int red_energy8 = 0;
int green_energy8 = 0;
int blue_energy8 = 0;

byte upside_down8 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode

byte currentEnergy8[numLeds]; // current energy level
byte nextEnergy8[numLeds]; // next energy level
byte energyMode8[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passiveTorchRainbow = 0, // just environment, glow from nearby radiation
  torch_nop8 = 1, // no processing
  torch_spark8= 2, // slowly looses energy, moves up
  torch_spark8_temp = 3, // a spark still getting energy from the level below
};

inline void reduce8(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}

inline void increase8(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomRainbow(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergy8()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergy8[i] = 0;
    nextEnergy8[i] = 0;
    energyMode8[i] = torch_passiveTorchRainbow;
  }
}

void calcnextEnergy8()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergy8[i];
      byte m = energyMode8[i];
      switch (m) {
        case torch_spark8: {
          // loose transfer up energy as long as the is any
          reduce8(e, spark_tfr8);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyMode8[i+ledsPerLevel] = torch_spark8_temp;
          }
          break;
        }
        case torch_spark8_temp: {
          // just getting some energy from below
          byte e2 = currentEnergy8[i-ledsPerLevel];
          if (e2<spark_tfr8) {
            // cell below is exhausted, becomes passive
            energyMode8[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increase8(e, e2);
            // loose some overall energy
            e = ((int)e*spark_cap8)>>8;
            // this cell becomes active spark
            energyMode8[i] = torch_spark8;
          }
          else {
            increase8(e, spark_tfr8);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_cap8)>>8;
          increase8(e, ((((int)currentEnergy8[i-1]+(int)currentEnergy8[i+1])*side_rad8)>>9) + (((int)currentEnergy8[i-ledsPerLevel]*up_rad8)>>8));
        }
        default:
          break;
      }
      nextEnergy8[i++] = e;
    }
  }
}

const uint8_t energymap8[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColors8()
{ 
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    leds[i] = ColorFromPalette( RainbowColors_p, 60);
    if (upside_down8)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergy8[ei];
    currentEnergy8[ei] = e;
    if (e>250)
      leds[i] = ColorFromPalette( RainbowColors_p, 60); //CRGB(0, 15, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_bias8;
        byte g = green_bias8;
        byte b = blue_bias8;
        increase8(r, (eb*red_energy8)>>8);
        increase8(g, (eb*green_energy8)>>8);
        increase8(b, (eb*blue_energy8)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bg8, green_bg8, blue_bg8);
      }
    }
  }
}

void injectrandom8()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergy8[i] = random8(flame_min8, flame_max8);
    energyMode8[i] = torch_nop8;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyMode8[i]!=torch_spark8 && random8(100)<random_spark_probability8) {
      currentEnergy8[i] = random8(spark_min8, spark_max8);
      energyMode8[i] = torch_spark8;
    }
  }
}

uint16_t FireRainbow() {
  injectrandom8();
  calcnextEnergy8();
  calcNextColors8();
  return 1;
}
