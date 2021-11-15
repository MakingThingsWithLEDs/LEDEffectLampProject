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

uint16_t cycle_waitFireWhite = 50; // 0..255

byte flame_minFireWhite = 20; // 0..255
byte flame_maxFireWhite = 250; // 0..255

byte random_spark_probabilityFireWhite = 1; // 0..100
byte spark_minFireWhite = 200; // 0..255
byte spark_maxFireWhite = 255; // 0..255

byte spark_tfrFireWhite = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_capFireWhite = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_radFireWhite = 30; // up speed
uint16_t side_radFireWhite = 3; // sidewards radiation
uint16_t heat_capFireWhite = 30; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bgFireWhite = 0;
byte green_bgFireWhite = 0;
byte blue_bgFireWhite = 0;
byte red_biasFireWhite = 100;
byte green_biasFireWhite = 100;
byte blue_biasFireWhite = 100;
int red_energyFireWhite = 255;
int green_energyFireWhite = 255;
int blue_energyFireWhite = 255;

byte upside_downFireWhite = 0; // Invert effect. 0 disabled / 1 enabled

// torch mode

#define numLeds NUM_LEDS
#define ledsPerLevel MATRIX_WIDTH
#define levels MATRIX_HEIGHT

byte currentEnergyFireWhite[numLeds]; // current energy level
byte nextEnergyFireWhite[numLeds]; // next energy level
byte energyModeFireWhite[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passiveFireWhite = 1, // just environment, glow from nearby radiation
  torch_nopFireWhite = 1, // no processing
  torch_sparkFireWhite= 2, // slowly looses energy, moves up
  torch_sparkFireWhite_temp = 3, // a spark still getting energy from the level below
};

inline void reduceFireWhite(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}

inline void increaseFireWhite(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomFireWhite(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergyFireWhite()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergyFireWhite[i] = 0;
    nextEnergyFireWhite[i] = 0;
    energyModeFireWhite[i] = torch_passiveFireWhite;
  }
}

void calcnextEnergyFireWhite()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergyFireWhite[i];
      byte m = energyModeFireWhite[i];
      switch (m) {
        case torch_sparkFireWhite: {
          // loose transfer up energy as long as the is any
          reduceFireWhite(e, spark_tfrFireWhite);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyModeFireWhite[i+ledsPerLevel] = torch_sparkFireWhite_temp;
          }
          break;
        }
        case torch_sparkFireWhite_temp: {
          // just getting some energy from below
          byte e2 = currentEnergyFireWhite[i-ledsPerLevel];
          if (e2<spark_tfrFireWhite) {
            // cell below is exhausted, becomes passive
            energyModeFireWhite[i-ledsPerLevel] = torch_passiveFireWhite;
            // gobble up rest of energy
            increaseFireWhite(e, e2);
            // loose some overall energy
            e = ((int)e*spark_capFireWhite)>>8;
            // this cell becomes active spark
            energyModeFireWhite[i] = torch_sparkFireWhite;
          }
          else {
            increaseFireWhite(e, spark_tfrFireWhite);
          }
          break;
        }
        case torch_passiveFireWhite: {
          e = ((int)e*heat_capFireWhite)>>8;
          increaseFireWhite(e, ((((int)currentEnergyFireWhite[i-1]+(int)currentEnergyFireWhite[i+1])*side_radFireWhite)>>9) + (((int)currentEnergyFireWhite[i-ledsPerLevel]*up_radFireWhite)>>8));
        }
        default:
          break;
      }
      nextEnergyFireWhite[i++] = e;
    }
  }
}

const uint8_t energymapFireWhite[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColorsFireWhite()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_downFireWhite)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergyFireWhite[ei];
    currentEnergyFireWhite[ei] = e;
    if (e>250)
      leds[i] = CRGB(0, 0, 0); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymapFireWhite[e>>3];
        byte r = red_biasFireWhite;
        byte g = green_biasFireWhite;
        byte b = blue_biasFireWhite;
        increaseFireWhite(r, (eb*red_energyFireWhite)>>8);
        increaseFireWhite(g, (eb*green_energyFireWhite)>>8);
        increaseFireWhite(b, (eb*blue_energyFireWhite)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bgFireWhite, green_bgFireWhite, blue_bgFireWhite);
      }
    }
  }
}

void injectRandomFireWhite()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergyFireWhite[i] = random8(flame_minFireWhite, flame_maxFireWhite);
    energyModeFireWhite[i] = torch_nopFireWhite;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyModeFireWhite[i]!=torch_sparkFireWhite && random8(100)<random_spark_probabilityFireWhite) {
      currentEnergyFireWhite[i] = random8(spark_minFireWhite, spark_maxFireWhite);
      energyModeFireWhite[i] = torch_sparkFireWhite;
    }
  }
}

uint16_t FireWhite() {
  injectRandomFireWhite();
  calcnextEnergyFireWhite();
  calcNextColorsFireWhite();
  return 1;
}
