/*
 * Torch: https://github.com/evilgeniuslabs/torch
 * Copyright (C) 2015 Jason Coon
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * ---------------------------------------------------------------------------------------------------*
 * 
 * Modified by RMProjectsUK https://www.youtube.com/c/RMProjectsUK
 * I have left all previous comments from the original code writers in place, i ask you do the same.
 * If you use this code for your own projects and upload it please link back to the original sources.
 * 
 */

#include <FastLED.h>
#include <IRremote.h>
#include <EEPROM.h>
#include <Bounce2.h>

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_PIN     11                          // Edit this to your required pin number (leave at 11 recommended)
#define IR_RECV_PIN 12                          // Pin for use with an IR remote control
#define COLOR_ORDER GRB                         // LED order Green, Red, Blue as default
#define CHIPSET     1, WS2812B                  // LED Chipset if using teensy 4.0 or above add 1, if below remove 1,
#define NUM_LEDS    174                         // How many leds total?
#define MAX_POWER_MILLIAMPS 5000                // Power Supply In m/A 1000=1amp 5000=5amp 10000=10amp etc.
#define BUTTON_1_PIN 16
#define BUTTON_2_PIN 17

const uint8_t MATRIX_WIDTH = 6;                 // Edit this to your matrix width
const uint8_t MATRIX_HEIGHT = 29;               // Edit this to your matrix height

uint16_t XY(uint8_t x, uint8_t y);
void dimAll(byte value);
uint16_t ColorWaves();
uint16_t Pride();
uint16_t CloudTwinkles();
uint16_t RainbowTwinkles();
uint16_t SnowTwinkles();
uint16_t IncandescentTwinkles();
uint16_t Fireflies();
uint16_t Rainbow();
uint16_t RainbowWithGlitter();
void AddGlitter(fract8 chanceOfGlitter);
uint16_t Confetti();
uint16_t BPM();
uint16_t Juggle();
uint16_t ShowSolidColor();
uint16_t HueCycle();
uint16_t Sinelon();

const int MATRIX_CENTER_X = MATRIX_WIDTH / 2;
const int MATRIX_CENTER_Y = MATRIX_HEIGHT / 2;

const byte MATRIX_CENTRE_X = MATRIX_CENTER_X - 1;
const byte MATRIX_CENTRE_Y = MATRIX_CENTER_Y - 1;

const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t brightness = brightnessMap[0];

CRGB leds[NUM_LEDS + 1];
IRrecv irReceiver(IR_RECV_PIN);

Bounce button1 = Bounce();
Bounce button2 = Bounce();

#include "Commands.h"
#include "GradientPalettes.h"

CRGB solidColor = CRGB::Purple;  //Solid Colour Effect

typedef uint16_t(*PatternFunctionPointer)();
typedef PatternFunctionPointer PatternList [];
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int autoPlayDurationSeconds = 40;                                     // Set automatic play time per effect adjust as required
unsigned int autoPlayTimeout = 10;
bool autoplayEnabled = true;                                          // Disable / Enable automatic play function, use false if you wish to use momentary switch to control effects.

InputCommand command;

int currentPatternIndex = 0;
PatternFunctionPointer currentPattern;

CRGB w(85, 85, 85), W(CRGB::White);
CRGBPalette16 snowColors = CRGBPalette16( W, W, W, W, w, w, w, w, w, w, w, w, w, w, w, w );

CRGB l(0xE1A024);
CRGBPalette16 incandescentColors = CRGBPalette16( l, l, l, l, l, l, l, l, l, l, l, l, l, l, l, l );

const CRGBPalette16 palettes[] = {
  RainbowColors_p,
  RainbowStripeColors_p,
  OceanColors_p,
  CloudColors_p,
  ForestColors_p,
  PartyColors_p,
  HeatColors_p,
  LavaColors_p,
  snowColors,
};

const int paletteCount = ARRAY_SIZE(palettes);

int currentPaletteIndex = 0;
CRGBPalette16 palette = palettes[0];

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

#include "Drawing.h"
#include "Effects.h"
#include "Noise.h"
#include "Pulse.h"
#include "Wave.h"
#include "Fire2012WithPalette.h"
#include "Fire2012Rainbow.h"
#include "FireWhite.h"
#include "FireRed.h"
#include "FireGreen.h"
#include "FireBlue.h"
#include "FireChemical.h"
#include "FireRainbow.h"
#include "FirePurple.h"
#include "FireOrange.h"
#include "AudioLogic.h"
#include "AudioPatterns.h"
//===================================================================================================================================================START EFFECT PLAYLIST
// EFFECTS LISTS                    // In automatic mode will display in order of the list.
const PatternList patterns = {      // remove or add // to enable/disable effects

  Aurora,
  BlackAndWhite,
  Calbayo,
  Cloud,   
  ColorCube,
  CoralReef,
  Curvature,
  DeepSea,
  Fire2012Rainbow1,
  FireBlue,
  FireChemical,
  FireGreen,
  FireOrange,
  FirePurple,
  FireRed,
  FireWhite,
  Forest,
  Lava,
  LavaLampRainbow,
  LavaLampRainbowStripe,
  Ocean,
  OceanBreeze,
  Party,
  RampRGB,
  Rstcurv,
  Shikon1,
  Shikon2,
  Spectrum,
  Temperature,

//--Sound-Reactive-Effects--
//  AudioAnalyzerColumns,
//  AudioAnalyzerColumnsSolid,
//  AudioAnalyzerPixels,
//  AudioBlackAndBlue,
//  AudioBlackAndWhite,
//  AudioCloud,
//  AudioFallingSpectrogram,
//  AudioFire1,
//  AudioFire2,
//  AudioFire3,
//  AudioForest,
//  AudioLava,
//  AudioLava2,
//  AudioLavaMagenta,
//  AudioLavaRainbow,
//  AudioOcean,
//  AudioParty,
//  AudioRainbowStripe,

//---Additional-Effects---
//  BPM,
//  CloudTwinkles,
//  ColorWaves,  
//  Confetti,
//  Fire2012WithPalette,
//  Fireflies,
//  FireNoise,
//  HueCycle,
//  IncandescentTwinkles,
//  Juggle,
//  Pride,
//  Pulse,
//  Rainbow,
//  RainbowTwinkles,
//  RainbowWithGlitter,
//  ShowSolidColor,
//  Sinelon,
//  SnowTwinkles,
//  Wave,

};
//===================================================================================================================================================END EFFECT PLAYLIST
const int patternCount = ARRAY_SIZE(patterns);

void setup() {
  delay(1000); // sanity delay
  // Serial.begin(9600);
  // Serial.println("setup start");

  loadSettings();

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);      // Voltage, MAX_POWER_MILLIAMPS
  // FastLED.setBrightness(brightness);
  // FastLED.setDither(brightness < 255);

  // Initialize the IR receiver
  irReceiver.enableIRIn();
  irReceiver.blink13(true);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  button1.attach(BUTTON_1_PIN);
  button2.attach(BUTTON_2_PIN);
  button1.interval(5);
  button2.interval(5);

  currentPattern = patterns[currentPatternIndex];
  autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);

  initializeAudio();

  // Serial.println("setup end");
}

void loop() {
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());

  EVERY_N_MILLISECONDS(30) {
    readAudio();
  }

  uint16_t requestedDelay = currentPattern();

  FastLED.show(); // display this frame

  handleInput(requestedDelay);

  if (autoplayEnabled && millis() > autoPlayTimeout) {
    move(1);
    autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);
  }

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}

void loadSettings() {
  // load settings from EEPROM

  // brightness
  brightness = EEPROM.read(0);
  if (brightness < 1)
    brightness = 1;
  else if (brightness > 255)
    brightness = 255;

  // currentPatternIndex
  currentPatternIndex = EEPROM.read(1);
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patternCount)
    currentPatternIndex = patternCount - 1;

  // solidColor
  solidColor.r = EEPROM.read(2);
  solidColor.g = EEPROM.read(3);
  solidColor.b = EEPROM.read(4);

  if (solidColor.r == 0 && solidColor.g == 0 && solidColor.b == 0)
    solidColor = CRGB::White;
}

void setSolidColor(CRGB color) {
  solidColor = color;

  EEPROM.write(2, solidColor.r);
  EEPROM.write(3, solidColor.g);
  EEPROM.write(4, solidColor.b);

  moveTo(patternCount - 1);
}

void powerOff()
{
  // clear the display
  const uint8_t stepSize = 4;

  for (uint8_t i = 0; i < NUM_LEDS / 2 - stepSize; i += stepSize) {
    for (uint8_t j = 0; j < stepSize; j++) {
      leds[i + j] = CRGB::Black;
      leds[(NUM_LEDS - 1) - (i + j)] = CRGB::Black;
    }
    FastLED.show(); // display this frame
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  FastLED.show(); // display this frame

  while (true) {
    // check for physical button input
    button1.update();
    button2.update();

    if (button1.rose() || button2.rose()) {
      Serial.println("Button released");
      return;
    }

    // check for ir remote input
    InputCommand command = readCommand();
    if (command != InputCommand::None)
      return;
  }
}

void move(int delta) {
  moveTo(currentPatternIndex + delta);
}

void moveTo(int index) {
  currentPatternIndex = index;

  if (currentPatternIndex >= patternCount)
    currentPatternIndex = 0;
  else if (currentPatternIndex < 0)
    currentPatternIndex = patternCount - 1;

  currentPattern = patterns[currentPatternIndex];

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  EEPROM.write(1, currentPatternIndex);
}

int getBrightnessLevel() {
  int level = 0;
  for (int i = 0; i < brightnessCount; i++) {
    if (brightnessMap[i] >= brightness) {
      level = i;
      break;
    }
  }
  return level;
}

uint8_t cycleBrightness() {
  adjustBrightness(1);

  if (brightness == brightnessMap[0])
    return 0;

  return brightness;
}

void adjustBrightness(int delta) {
  int level = getBrightnessLevel();

  level += delta;

  // don't wrap
  if (level < 0)
    level = 0;
  if (level >= brightnessCount)
    level = brightnessCount - 1;

  brightness = brightnessMap[level];
  FastLED.setBrightness(brightness);
  FastLED.setDither(brightness < 255);

  EEPROM.write(0, brightness);
}

void cyclePalette(int delta = 1) {
  if (currentPaletteIndex == 0 && delta < 0)
    currentPaletteIndex = paletteCount - 1;
  else if (currentPaletteIndex >= paletteCount - 1 && delta > 0)
    currentPaletteIndex = 0;
  else
    currentPaletteIndex += delta;

  if (currentPaletteIndex >= paletteCount)
    currentPaletteIndex = 0;

  palette = palettes[currentPaletteIndex];
}

unsigned long button1PressTimeStamp;
unsigned long button2PressTimeStamp;

void handleInput(unsigned int requestedDelay) {
  unsigned int requestedDelayTimeout = millis() + requestedDelay;

  while (true) {
    // check for physical button input
    button1.update();
    button2.update();

    if (button1.fell()) {
      Serial.println("Button 1 depressed");
      button1PressTimeStamp = millis();
    }

    if (button2.fell()) {
      Serial.println("Button 2 depressed");
      button2PressTimeStamp = millis();
    }

    if (button1.rose()) {
      Serial.println("Button 1 released");
      move(1);
    }

    if (button2.rose()) {
      Serial.println("Button 2 released");
      powerOff();
      break;
    }

    command = readCommand(defaultHoldDelay);

    if (command != InputCommand::None) {
      // Serial.print("command: ");
      // Serial.println((int) command);
    }

    if (command == InputCommand::Up) {
      move(1);
      break;
    }
    else if (command == InputCommand::Down) {
      move(-1);
      break;
    }
    else if (command == InputCommand::Brightness) {
      if (isHolding || cycleBrightness() == 0) {
        heldButtonHasBeenHandled();
        powerOff();
        break;
      }
    }
    else if (command == InputCommand::Power) {
      powerOff();
      break;
    }
    else if (command == InputCommand::BrightnessUp) {
      adjustBrightness(1);
    }
    else if (command == InputCommand::BrightnessDown) {
      adjustBrightness(-1);
    }
    else if (command == InputCommand::PlayMode) { // toggle pause/play
      autoplayEnabled = !autoplayEnabled;
    }
    else if (command == InputCommand::NextPalette) { // cycle color palette
      cyclePalette(1);
    }
    else if (command == InputCommand::PreviousPalette) { // cycle color palette
      cyclePalette(-1);
    }

    // pattern buttons

    else if (command == InputCommand::Pattern1) {
      moveTo(0);
      break;
    }
    else if (command == InputCommand::Pattern2) {
      moveTo(1);
      break;
    }
    else if (command == InputCommand::Pattern3) {
      moveTo(2);
      break;
    }
    else if (command == InputCommand::Pattern4) {
      moveTo(3);
      break;
    }
    else if (command == InputCommand::Pattern5) {
      moveTo(4);
      break;
    }
    else if (command == InputCommand::Pattern6) {
      moveTo(5);
      break;
    }
    else if (command == InputCommand::Pattern7) {
      moveTo(6);
      break;
    }
    else if (command == InputCommand::Pattern8) {
      moveTo(7);
      break;
    }
    else if (command == InputCommand::Pattern9) {
      moveTo(8);
      break;
    }
    else if (command == InputCommand::Pattern10) {
      moveTo(9);
      break;
    }
    else if (command == InputCommand::Pattern11) {
      moveTo(10);
      break;
    }
    else if (command == InputCommand::Pattern12) {
      moveTo(11);
      break;
    }

    // custom color adjustment buttons

    else if (command == InputCommand::RedUp) {
      solidColor.red += 1;
      setSolidColor(solidColor);
      break;
    }
    else if (command == InputCommand::RedDown) {
      solidColor.red -= 1;
      setSolidColor(solidColor);
      break;
    }
    else if (command == InputCommand::GreenUp) {
      solidColor.green += 1;
      setSolidColor(solidColor); \
      break;
    }
    else if (command == InputCommand::GreenDown) {
      solidColor.green -= 1;
      setSolidColor(solidColor);
      break;
    }
    else if (command == InputCommand::BlueUp) {
      solidColor.blue += 1;
      setSolidColor(solidColor);
      break;
    }
    else if (command == InputCommand::BlueDown) {
      solidColor.blue -= 1;
      setSolidColor(solidColor);
      break;
    }

    // color buttons

    else if (command == InputCommand::Red && currentPatternIndex != patternCount - 2 && currentPatternIndex != patternCount - 3) { // Red, Green, and Blue buttons can be used by ColorInvaders game, which is the next to last pattern
      setSolidColor(CRGB::Red);
      break;
    }
    else if (command == InputCommand::RedOrange) {
      setSolidColor(CRGB::OrangeRed);
      break;
    }
    else if (command == InputCommand::Orange) {
      setSolidColor(CRGB::Orange);
      break;
    }
    else if (command == InputCommand::YellowOrange) {
      setSolidColor(CRGB::Goldenrod);
      break;
    }
    else if (command == InputCommand::Yellow) {
      setSolidColor(CRGB::Yellow);
      break;
    }

    else if (command == InputCommand::Green && currentPatternIndex != patternCount - 2 && currentPatternIndex != patternCount - 3) { // Red, Green, and Blue buttons can be used by ColorInvaders game, which is the next to last pattern
      setSolidColor(CRGB::Green);
      break;
    }
    else if (command == InputCommand::Lime) {
      setSolidColor(CRGB::Lime);
      break;
    }
    else if (command == InputCommand::Aqua) {
      setSolidColor(CRGB::Aqua);
      break;
    }
    else if (command == InputCommand::Teal) {
      setSolidColor(CRGB::Teal);
      break;
    }
    else if (command == InputCommand::Navy) {
      setSolidColor(CRGB::Navy);
      break;
    }

    else if (command == InputCommand::Blue && currentPatternIndex != patternCount - 2 && currentPatternIndex != patternCount - 3) { // Red, Green, and Blue buttons can be used by ColorInvaders game, which is the next to last pattern
      setSolidColor(CRGB::Blue);
      break;
    }
    else if (command == InputCommand::RoyalBlue) {
      setSolidColor(CRGB::RoyalBlue);
      break;
    }
    else if (command == InputCommand::Purple) {
      setSolidColor(CRGB::Purple);
      break;
    }
    else if (command == InputCommand::Indigo) {
      setSolidColor(CRGB::Indigo);
      break;
    }
    else if (command == InputCommand::Magenta) {
      setSolidColor(CRGB::Magenta);
      break;
    }

    else if (command == InputCommand::White && currentPatternIndex != patternCount - 2 && currentPatternIndex != patternCount - 3) {
      setSolidColor(CRGB::White);
      break;
    }
    else if (command == InputCommand::Pink) {
      setSolidColor(CRGB::Pink);
      break;
    }
    else if (command == InputCommand::LightPink) {
      setSolidColor(CRGB::LightPink);
      break;
    }
    else if (command == InputCommand::BabyBlue) {
      setSolidColor(CRGB::CornflowerBlue);
      break;
    }
    else if (command == InputCommand::LightBlue) {
      setSolidColor(CRGB::LightBlue);
      break;
    }

    if (millis() >= requestedDelayTimeout)
      break;
  }
}

uint16_t XY( uint8_t x, uint8_t y) // maps the matrix to the strip
{
  uint16_t i;
  i = (y * MATRIX_WIDTH) + (MATRIX_WIDTH - x);

  i = (NUM_LEDS - 1) - i;

  if (i > NUM_LEDS)
    i = NUM_LEDS;

  return i;
}

// scale the brightness of the screenbuffer down
void dimAll(byte value)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(value);
  }
}
//===============================================
uint16_t ShowSolidColor() {
  fill_solid(leds, NUM_LEDS, solidColor);

  return 60;
}
//===============================================
uint16_t Rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, gHue, 1);

  return 8;
}
//===============================================
uint16_t RainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  Rainbow();
  AddGlitter(80);
  return 8;
}
//===============================================
void AddGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}
//===============================================
uint16_t Confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += ColorFromPalette(palette, gHue + random8(64), 255); // CHSV(gHue + random8(64), 200, 255);
  return 8;
}
//===============================================
uint16_t BPM()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  return 8;
}
//===============================================
uint16_t Juggle() {
  // N colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 1;
  byte dotCount = 3;
  for (int i = 0; i < dotCount; i++) {
    leds[beatsin16(i + dotCount - 1, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 256 / dotCount;
  }
  return 0;
}
//===============================================
// An animation to play while the crowd goes wild after the big performance
uint16_t Applause()
{
  static uint16_t lastPixel = 0;
  fadeToBlackBy(leds, NUM_LEDS, 32);
  leds[lastPixel] = CHSV(random8(HUE_BLUE, HUE_PURPLE), 255, 255);
  lastPixel = random16(NUM_LEDS);
  leds[lastPixel] = CRGB::White;
  return 8;
}
//===============================================
// An "animation" to just fade to black.  Useful as the last track
// in a non-looping performance-oriented playlist.
uint16_t fadeToBlack()
{
  fadeToBlackBy(leds, NUM_LEDS, 10);
  return 8;
}
//===============================================
uint16_t Sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint16_t pos = beatsin16(13, 0, NUM_LEDS);
  static uint16_t prevpos = 0;
  if ( pos < prevpos ) {
    fill_solid( leds + pos, (prevpos - pos) + 1, CHSV(gHue, 220, 255));
  } else {
    fill_solid( leds + prevpos, (pos - prevpos) + 1, CHSV( gHue, 220, 255));
  }
  prevpos = pos;

  return 8;
}
//===============================================
uint16_t HueCycle() {
  fill_solid(leds, NUM_LEDS, CHSV(gHue, 255, 255));
  return 60;
}
//===============================================
// Pride2015 by Mark Kriegsman
// https://gist.github.com/kriegsman/964de772d64c502760e5

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
uint16_t Pride()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (int i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t) ((uint32_t) b16 * (uint32_t) b16) / 65536;
    uint8_t bri8 = (uint32_t) (((uint32_t) bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint8_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
#define SECONDS_PER_PALETTE 10

uint16_t ColorWaves()
{
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }

  Colorwaves( leds, NUM_LEDS, gCurrentPalette);

  return 20;
}


// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void Colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  // uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds - 1) - pixelnumber;

    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}

// Alternate rendering function just scrolls the current palette
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette)
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_LEDS) + 1, gCurrentPalette, 255, LINEARBLEND);
}

#define STARTING_BRIGHTNESS 64
#define FADE_IN_SPEED       32
#define FADE_OUT_SPEED      20
uint8_t DENSITY          = 255;
//===============================================
uint16_t CloudTwinkles()
{
  DENSITY = 255;
  colortwinkles(CloudColors_p);
  return 20;
}
//===============================================
uint16_t RainbowTwinkles()
{
  DENSITY = 255;
  colortwinkles(RainbowColors_p);
  return 20;
}
//===============================================
uint16_t SnowTwinkles()
{
  DENSITY = 255;
  colortwinkles(snowColors);
  return 20;
}
//===============================================
uint16_t IncandescentTwinkles()
{
  DENSITY = 255;
  colortwinkles(incandescentColors);
  return 20;
}
//===============================================
uint16_t Fireflies()
{
  DENSITY = 16;
  colortwinkles(incandescentColors);
  return 20;
}
//===============================================
enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };

void colortwinkles(CRGBPalette16 palette)
{
  // Make each pixel brighter or darker, depending on
  // its 'direction' flag.
  brightenOrDarkenEachPixel( FADE_IN_SPEED, FADE_OUT_SPEED);

  // Now consider adding a new random twinkle
  if ( random8() < DENSITY ) {
    int pos = random16(NUM_LEDS);
    if ( !leds[pos]) {
      leds[pos] = ColorFromPalette( palette, random8(), STARTING_BRIGHTNESS, NOBLEND);
      setPixelDirection(pos, GETTING_BRIGHTER);
    }
  }
}

void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount)
{
  for ( uint16_t i = 0; i < NUM_LEDS; i++) {
    if ( getPixelDirection(i) == GETTING_DARKER) {
      // This pixel is getting darker
      leds[i] = makeDarker( leds[i], fadeDownAmount);
    } else {
      // This pixel is getting brighter
      leds[i] = makeBrighter( leds[i], fadeUpAmount);
      // now check to see if we've maxxed out the brightness
      if ( leds[i].r == 255 || leds[i].g == 255 || leds[i].b == 255) {
        // if so, turn around and start getting darker
        setPixelDirection(i, GETTING_DARKER);
      }
    }
  }
}

CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

CRGB makeDarker( const CRGB& color, fract8 howMuchDarker)
{
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

// Compact implementation of
// the directionFlags array, using just one BIT of RAM
// per pixel.  This requires a bunch of bit wrangling,
// but conserves precious RAM.  The cost is a few
// cycles and about 100 bytes of flash program memory.
uint8_t  directionFlags[ (NUM_LEDS + 7) / 8];

bool getPixelDirection( uint16_t i) {
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  andMask = 1 << bitNum;
  return (directionFlags[index] & andMask) != 0;
}

void setPixelDirection( uint16_t i, bool dir) {
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  orMask = 1 << bitNum;
  uint8_t andMask = 255 - orMask;
  uint8_t value = directionFlags[index] & andMask;
  if ( dir ) {
    value += orMask;
  }
  directionFlags[index] = value;
}
