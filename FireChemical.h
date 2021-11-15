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

uint16_t cycle_waitChemicalFire = 1; // 0..255

byte flame_minChemicalFire = 70; // 0..255
byte flame_maxChemicalFire = 250; // 0..255

byte random_spark_probabilityChemicalFire = 1; // 0..100
byte spark_minChemicalFire = 60; // 0..255
byte spark_maxChemicalFire = 255; // 0..255

byte spark_tfrChemicalFire = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_capChemicalFire = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_radChemicalFire = 50; // up radiation
uint16_t side_radChemicalFire = 40; // sidewards radiation
uint16_t heat_capChemicalFire = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bgChemicalFire = 0;
byte green_bgChemicalFire = 0;
byte blue_bgChemicalFire = 0;
byte red_biasChemicalFire = 35;
byte green_biasChemicalFire = 90;
byte blue_biasChemicalFire = 0;
int red_energyChemicalFire = 100;
int green_energyChemicalFire = 120;
int blue_energyChemicalFire = 0;

byte upside_downChemicalFire = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

byte currentEnergyChemicalFire[numLeds]; // current energy level
byte nextEnergyChemicalFire[numLeds]; // next energy level
byte energyModeChemicalFire[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passiveChemicalFire = 0, // just environment, glow from nearby radiation
  torch_nopChemicalFire = 1, // no processing
  torch_sparkChemicalFire= 2, // slowly looses energy, moves up
  torch_sparkChemicalFire_temp = 3, // a spark still getting energy from the level below
};

inline void reduceChemicalFire(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increaseChemicalFire(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomChemicalFire(uint16_t aMinOrMax, uint16_t aMax = 0)  // not really sure if this is needed at this stage
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
    currentEnergyChemicalFire[i] = 0;
    nextEnergyChemicalFire[i] = 0;
    energyModeChemicalFire[i] = torch_passiveChemicalFire;
  }
}

void calcnextEnergyChemicalFire()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergyChemicalFire[i];
      byte m = energyModeChemicalFire[i];
      switch (m) {
        case torch_sparkChemicalFire: {
          // loose transfer up energy as long as the is any
          reduceChemicalFire(e, spark_tfrChemicalFire);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyModeChemicalFire[i+ledsPerLevel] = torch_sparkChemicalFire_temp;
          }
          break;
        }
        case torch_sparkChemicalFire_temp: {
          // just getting some energy from below
          byte e2 = currentEnergyChemicalFire[i-ledsPerLevel];
          if (e2<spark_tfrChemicalFire) {
            // cell below is exhausted, becomes passive
            energyModeChemicalFire[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increaseChemicalFire(e, e2);
            // loose some overall energy
            e = ((int)e*spark_capChemicalFire)>>8;
            // this cell becomes active spark
            energyModeChemicalFire[i] = torch_sparkChemicalFire;
          }
          else {
            increaseChemicalFire(e, spark_tfrChemicalFire);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_capChemicalFire)>>8;
          increaseChemicalFire(e, ((((int)currentEnergyChemicalFire[i-1]+(int)currentEnergyChemicalFire[i+1])*side_radChemicalFire)>>9) + (((int)currentEnergyChemicalFire[i-ledsPerLevel]*up_radChemicalFire)>>8));
        }
        default:
          break;
      }
      nextEnergyChemicalFire[i++] = e;
    }
  }
}

const uint8_t energymapChemicalFire[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColorsChemicalFire()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_downChemicalFire)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergyChemicalFire[ei];
    currentEnergyChemicalFire[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_biasChemicalFire;
        byte g = green_biasChemicalFire;
        byte b = blue_biasChemicalFire;
        increaseChemicalFire(r, (eb*red_energyChemicalFire)>>8);
        increaseChemicalFire(g, (eb*green_energyChemicalFire)>>8);
        increaseChemicalFire(b, (eb*blue_energyChemicalFire)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bgChemicalFire, green_bgChemicalFire, blue_bgChemicalFire);
      }
    }
  }
}

void injectRandomChemicalFire()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergyChemicalFire[i] = random8(flame_minChemicalFire, flame_maxChemicalFire);
    energyModeChemicalFire[i] = torch_nopChemicalFire;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyModeChemicalFire[i]!=torch_sparkChemicalFire && random8(100)<random_spark_probabilityChemicalFire) {
      currentEnergyChemicalFire[i] = random8(spark_minChemicalFire, spark_maxChemicalFire);
      energyModeChemicalFire[i] = torch_sparkChemicalFire;
    }
  }
}

uint16_t FireChemical() {
  injectRandomChemicalFire();
  calcnextEnergyChemicalFire();
  calcNextColorsChemicalFire();
  return 1;
}
