// parameters

uint16_t cycle_waitCustom6 = 1; // 0..255

byte flame_minCustom6 = 100; // 0..255
byte flame_maxCustom6 = 220; // 0..255

byte random_spark_probabilityCustom6 = 2; // 0..100
byte spark_minCustom6 = 200; // 0..255
byte spark_maxCustom6 = 255; // 0..255

byte spark_tfrCustom6 = 40; // 0..256 how much energy is transferred up for a spark per cycle
uint16_t spark_capCustom6 = 200; // 0..255: spark cells: how much energy is retained from previous cycle

uint16_t up_radCustom6 = 40; // up radiation
uint16_t side_radCustom6 = 35; // sideward radiation
uint16_t heat_capCustom6 = 0; // 0..255: passive cells: how much energy is retained from previous cycle

byte red_bgCustom6 = 0;
byte green_bgCustom6 = 0;
byte blue_bgCustom6 = 0;
byte red_biasCustom6 = 3;
byte green_biasCustom6 = 50;
byte blue_biasCustom6 = 75;
int red_energyCustom6 = 89;
int green_energyCustom6 = 200; // 145;
int blue_energyCustom6 = 255;

byte upside_downCustom6 = 0; // if set, flame (or rather: drop) animation is upside down. Text remains as-is

// torch mode
// ==========

byte currentEnergyCustom6[numLeds]; // current energy level
byte nextEnergyCustom6[numLeds]; // next energy level
byte energyModeCustom6[numLeds]; // mode how energy is calculated for this point

enum {
  torch_passiveCustom6 = 0, // just environment, glow from nearby radiation
  torch_nopCustom6 = 1, // no processing
  torch_sparkCustom6 = 2, // slowly looses energy, moves up
  torch_sparkCustom6_temp = 3, // a spark still getting energy from the level below
};

inline void reduceCustom6(byte &aByte, byte aAmount, byte aMin = 0)
{
  int r = aByte-aAmount;
  if (r<aMin)
    aByte = aMin;
  else
    aByte = (byte)r;
}


inline void increaseCustom6(byte &aByte, byte aAmount, byte aMax = 255)
{
  int r = aByte+aAmount;
  if (r>aMax)
    aByte = aMax;
  else
    aByte = (byte)r;
}

uint16_t randomCustom6(uint16_t aMinOrMax, uint16_t aMax = 0)
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

void resetEnergyCustom6()
{
  for (int i=0; i<numLeds; i++) {
    currentEnergyCustom6[i] = 0;
    nextEnergyCustom6[i] = 0;
    energyModeCustom6[i] = torch_passiveCustom6;
  }
}

void calcnextEnergyCustom6()
{
  int i = 0;
  for (int y=0; y<levels; y++) {
    for (int x=0; x<ledsPerLevel; x++) {
      byte e = currentEnergyCustom6[i];
      byte m = energyModeCustom6[i];
      switch (m) {
        case torch_sparkCustom6: {
          // loose transfer up energy as long as the is any
          reduceCustom6(e, spark_tfrCustom6);
          // cell above is temp spark, sucking up energy from this cell until empty
          if (y<levels-1) {
            energyModeCustom6[i+ledsPerLevel] = torch_sparkCustom6_temp;
          }
          break;
        }
        case torch_sparkCustom6_temp: {
          // just getting some energy from below
          byte e2 = currentEnergyCustom6[i-ledsPerLevel];
          if (e2<spark_tfrCustom6) {
            // cell below is exhausted, becomes passive
            energyModeCustom6[i-ledsPerLevel] = torch_passive;
            // gobble up rest of energy
            increaseCustom6(e, e2);
            // loose some overall energy
            e = ((int)e*spark_capCustom6)>>8;
            // this cell becomes active spark
            energyModeCustom6[i] = torch_sparkCustom6;
          }
          else {
            increaseCustom6(e, spark_tfrCustom6);
          }
          break;
        }
        case torch_passive: {
          e = ((int)e*heat_capCustom6)>>8;
          increaseCustom6(e, ((((int)currentEnergyCustom6[i-1]+(int)currentEnergyCustom6[i+1])*side_radCustom6)>>9) + (((int)currentEnergyCustom6[i-ledsPerLevel]*up_radCustom6)>>8));
        }
        default:
          break;
      }
      nextEnergyCustom6[i++] = e;
    }
  }
}

const uint8_t energymapCustom6[32] = {0, 64, 96, 112, 128, 144, 152, 160, 168, 176, 184, 184, 192, 200, 200, 208, 208, 216, 216, 224, 224, 224, 232, 232, 232, 240, 240, 240, 240, 248, 248, 248};

void calcNextColorsCustom6()
{
  for (int i=0; i<numLeds; i++) {
    int ei; // index into energy calculation buffer
    if (upside_downCustom6)
      ei = numLeds-i;
    else
      ei = i;
    uint16_t e = nextEnergyCustom6[ei];
    currentEnergyCustom6[ei] = e;
    if (e>250)
      leds[i] = CRGB(170, 170, e); // blueish extra-bright spark
    else {
      if (e>0) {
        // energy to brightness is non-linear
        byte eb = energymap[e>>3];
        byte r = red_biasCustom6;
        byte g = green_biasCustom6;
        byte b = blue_biasCustom6;
        increaseCustom6(r, (eb*red_energyCustom6)>>8);
        increaseCustom6(g, (eb*green_energyCustom6)>>8);
        increaseCustom6(b, (eb*blue_energyCustom6)>>8);
        leds[i] = CRGB(r, g, b);
      }
      else {
        // background, no energy
        leds[i] = CRGB(red_bgCustom6, green_bgCustom6, blue_bgCustom6);
      }
    }
  }
}

void injectRandomCustom6()
{
  // random flame energy at bottom row
  for (int i=0; i<ledsPerLevel; i++) {
    currentEnergyCustom6[i] = randomCustom6(flame_minCustom6, flame_maxCustom6);
    energyModeCustom6[i] = torch_nopCustom6;
  }
  // random sparks at second row
  for (int i=ledsPerLevel; i<2*ledsPerLevel; i++) {
    if (energyModeCustom6[i]!=torch_sparkCustom6 && randomCustom6(100)<random_spark_probabilityCustom6) {
      currentEnergyCustom6[i] = randomCustom6(spark_minCustom6, spark_maxCustom6);
      energyModeCustom6[i] = torch_sparkCustom6;
    }
  }
}

uint16_t FireElectricBlue() {
  injectRandomCustom6();
  calcnextEnergyCustom6();
  calcNextColorsCustom6();
  return 1;
}
