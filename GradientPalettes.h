// ColorWavesWithPalettes
// Animated shifting color waves, with several cross-fading color palettes.
// by Mark Kriegsman, August 2015
//
// Color palettes courtesy of cpt-city and its contributors:
//   http://soliton.vm.bytemark.co.uk/pub/cpt-city/
//
// Color palettes converted for FastLED using "PaletteKnife" v1:
//   http://fastled.io/tools/paletteknife/
//

// Gradient Color Palette definitions for 33 different cpt-city color palettes.
//    956 bytes of PROGMEM for all of the palettes together,
//   +618 bytes of PROGMEM for gradient palette code (AVR).
//  1,494 bytes total for all 34 color palettes and associated code.

// Gradient palette "ib_jul01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/xmas/tn/ib_jul01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( ib_jul01_gp ) {
    0, 194,  1,  1,
   94,   1, 29, 18,
  132,  57,131, 28,
  255, 113,  1,  1};

// Gradient palette "es_vintage_57_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/vintage/tn/es_vintage_57.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

// Gradient palette "ramp_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/grass/tn/ramp.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( ramp_gp ) {
    0,   0,  0,  0,
   84,   0,  0,255,
   84,   0,  0,  0,
  170,   0,255,  0,
  170,   0,  0,  0,
  255, 255,  0,  0};

DEFINE_GRADIENT_PALETTE( es_vintage_57_gp ) {
    0,   2,  1,  1,
   53,  18,  1,  0,
  104,  69, 29,  1,
  153, 167,135, 10,
  255,  46, 56,  4};

// Gradient palette "es_vintage_01_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/vintage/tn/es_vintage_01.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_vintage_01_gp ) {
    0,   4,  1,  1,
   51,  16,  0,  1,
   76,  97,104,  3,
  101, 255,131, 19,
  127,  67,  9,  4,
  153,  16,  0,  1,
  229,   4,  1,  1,
  255,   4,  1,  1};

// Gradient palette "es_rivendell_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/rivendell/tn/es_rivendell_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_rivendell_15_gp ) {
    0,   1, 14,  5,
  101,  16, 36, 14,
  165,  56, 68, 30,
  242, 150,156, 99,
  255, 150,156, 99};

// Gradient palette "rgi_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/rgi/tn/rgi_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( rgi_15_gp ) {
    0,   4,  1, 31,
   31,  55,  1, 16,
   63, 197,  3,  7,
   95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38};

// Gradient palette "calbayo_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gacruxa/calbayo/tn/calbayo-15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( calbayo_15_gp ) {
    0, 210,131,  1,
   63, 210, 31,  6,
  125,  41,  2,  3,
  125,   7, 20, 12,
  125,   1, 58, 29,
  243,   1, 25, 14,
  255,   3,  6,  6};


// Gradient palette "retro2_16_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/retro2/tn/retro2_16.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE( retro2_16_gp ) {
    0, 188,135,  1,
  255,  46,  7,  1};

// Gradient palette "temperature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/arendal/tn/temperature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 144 bytes of program space.

DEFINE_GRADIENT_PALETTE( temperature_gp ) {
    0,   1, 27,105,
   14,   1, 27,105,
   14,   1, 40,127,
   28,   1, 40,127,
   28,   1, 70,168,
   42,   1, 70,168,
   42,   1, 92,197,
   56,   1, 92,197,
   56,   1,119,221,
   70,   1,119,221,
   70,   3,130,151,
   84,   3,130,151,
   84,  23,156,149,
   99,  23,156,149,
   99,  67,182,112,
  113,  67,182,112,
  113, 121,201, 52,
  127, 121,201, 52,
  127, 142,203, 11,
  141, 142,203, 11,
  141, 224,223,  1,
  155, 224,223,  1,
  155, 252,187,  2,
  170, 252,187,  2,
  170, 247,147,  1,
  184, 247,147,  1,
  184, 237, 87,  1,
  198, 237, 87,  1,
  198, 229, 43,  1,
  212, 229, 43,  1,
  212, 220, 15,  1,
  226, 220, 15,  1,
  226, 171,  2,  2,
  240, 171,  2,  2,
  240,  80,  3,  3,
  255,  80,  3,  3};

// Gradient palette "Analogous_1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/red/tn/Analogous_1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Analogous_1_gp ) {
    0,   3,  0,255,
   63,  23,  0,255,
  127,  67,  0,255,
  191, 142,  0, 45,
  255, 255,  0,  0};

// Gradient palette "es_pinksplash_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_pinksplash_08_gp ) {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};

// Gradient palette "es_pinksplash_07_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_07.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_pinksplash_07_gp ) {
    0, 229,  1,  1,
   61, 242,  4, 63,
  101, 255, 12,255,
  127, 249, 81,252,
  153, 255, 11,235,
  193, 244,  5, 68,
  255, 232,  1,  5};

// Gradient palette "Coral_reef_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/other/tn/Coral_reef.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( Coral_reef_gp ) {
    0,  40,199,197,
   50,  10,152,155,
   96,   1,111,120,
   96,  43,127,162,
  139,  10, 73,111,
  255,   1, 34, 71};

// Gradient palette "es_ocean_breeze_068_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_068.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_068_gp ) {
    0, 100,156,153,
   51,   1, 99,137,
  101,   1, 68, 84,
  104,  35,142,168,
  178,   0, 63,117,
  255,   1, 10, 10};

// Gradient palette "es_ocean_breeze_036_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_036.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_036_gp ) {
    0,   1,  6,  7,
   89,   1, 99,111,
  153, 144,209,255,
  255,   0, 73, 82};

// Gradient palette "es_ocean_breeze_026_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_026.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_026_gp ) {
    0,  52, 78, 71,
   76,   0, 16, 29,
  127,   1,  1,  1,
  178,   0, 16, 29,
  255,  52, 78, 71};


// Gradient palette "colorcube_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/colorcube.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 256 bytes of program space.

DEFINE_GRADIENT_PALETTE( colorcube_gp ) {
    0,  14, 22,  0,
    4,  14,104,  0,
    8,  14,255,  0,
   12,  88, 22,  0,
   16,  88,104,  0,
   20,  88,255,  0,
   24, 255, 22,  0,
   28, 255,104,  0,
   32, 255,255,  0,
   36,   0, 22, 44,
   40,   0,104, 44,
   44,   0,255, 44,
   48,  14,  0, 44,
   52,  14, 22, 44,
   56,  14,104, 44,
   60,  14,255, 44,
   64,  88,  0, 44,
   68,  88, 22, 44,
   72,  88,104, 44,
   76,  88,255, 44,
   80, 255,  0, 44,
   84, 255, 22, 44,
   89, 255,104, 44,
   93, 255,255, 44,
   97,   0, 22,255,
  101,   0,104,255,
  105,   0,255,255,
  109,  14,  0,255,
  113,  14, 22,255,
  117,  14,104,255,
  121,  14,255,255,
  125,  88,  0,255,
  129,  88, 22,255,
  133,  88,104,255,
  137,  88,255,255,
  141, 255,  0,255,
  145, 255, 22,255,
  149, 255,104,255,
  153,   2,  0,  0,
  157,  14,  0,  0,
  161,  41,  0,  0,
  165,  88,  0,  0,
  170, 157,  0,  0,
  174, 255,  0,  0,
  178,   0,  4,  0,
  182,   0, 22,  0,
  186,   0, 55,  0,
  190,   0,104,  0,
  194,   0,169,  0,
  198,   0,255,  0,
  202,   0,  0,  2,
  206,   0,  0, 15,
  210,   0,  0, 44,
  214,   0,  0, 92,
  218,   0,  0,160,
  222,   0,  0,255,
  226,   0,  0,  0,
  230,   1,  3,  1,
  234,   9, 15, 10,
  238,  27, 39, 30,
  242,  58, 73, 62,
  246, 106,121,109,
  250, 169,180,172,
  255, 255,255,255};


// Gradient palette "es_landscape_64_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/landscape/tn/es_landscape_64.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_landscape_64_gp ) {
    0,   0,  0,  0,
   37,   2, 25,  1,
   76,  15,115,  5,
  127,  79,213,  1,
  128, 126,211, 47,
  130, 188,209,247,
  153, 144,182,205,
  204,  59,117,250,
  255,   1, 37,192};

// Gradient palette "es_landscape_33_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/landscape/tn/es_landscape_33.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_landscape_33_gp ) {
    0,   1,  5,  0,
   19,  32, 23,  1,
   38, 161, 55,  1,
   63, 229,144,  1,
   66,  39,142, 74,
  255,   1,  4,  1};

// Gradient palette "rainbowsherbet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/icecream/tn/rainbowsherbet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( rainbowsherbet_gp ) {
    0, 255, 33,  4,
   43, 255, 68, 25,
   86, 255,  7, 25,
  127, 255, 82,103,
  170, 255,255,242,
  209,  42,255, 22,
  255,  87,255, 65};

// Gradient palette "gr65_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr65_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( gr65_hult_gp ) {
    0, 247,176,247,
   48, 255,136,255,
   89, 220, 29,226,
  160,   7, 82,178,
  216,   1,124,109,
  255,   1,124,109};

// Gradient palette "gr64_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr64_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( gr64_hult_gp ) {
    0,   1,124,109,
   66,   1, 93, 79,
  104,  52, 65,  1,
  130, 115,127,  1,
  150,  52, 65,  1,
  201,   1, 86, 72,
  239,   0, 55, 45,
  255,   0, 55, 45};

// Gradient palette "GMT_drywet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gmt/tn/GMT_drywet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( GMT_drywet_gp ) {
    0,  47, 30,  2,
   42, 213,147, 24,
   84, 103,219, 52,
  127,   3,219,207,
  170,   1, 48,214,
  212,   1,  1,111,
  255,   1,  7, 33};

// Gradient palette "ib15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( ib15_gp ) {
    0, 113, 91,147,
   72, 157, 88, 78,
   89, 208, 85, 33,
  107, 255, 29, 11,
  141, 137, 31, 39,
  255,  59, 33, 89};

// Gradient palette "Fuschia_7_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/fuschia/tn/Fuschia-7.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Fuschia_7_gp ) {
    0,  43,  3,153,
   63, 100,  4,103,
  127, 188,  5, 66,
  191, 161, 11,115,
  255, 135, 20,182};

// Gradient palette "es_emerald_dragon_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/emerald_dragon/tn/es_emerald_dragon_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_emerald_dragon_08_gp ) {
    0,  97,255,  1,
  101,  47,133,  1,
  178,  13, 43,  1,
  255,   2, 10,  1};

// Gradient palette "lava_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/lava.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( lava_gp ) {
    0,   0,  0,  0,
   46,  18,  0,  0,
   96, 113,  0,  0,
  108, 142,  3,  1,
  119, 175, 17,  1,
  146, 213, 44,  2,
  174, 255, 82,  4,
  188, 255,115,  4,
  202, 255,156,  4,
  218, 255,203,  4,
  234, 255,255,  4,
  244, 255,255, 71,
  255, 255,255,255};

// Gradient palette "fire_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/fire.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( fire_gp ) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255};

// Gradient palette "Colorfull_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Colorfull.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( Colorfull_gp ) {
    0,  10, 85,  5,
   25,  29,109, 18,
   60,  59,138, 42,
   93,  83, 99, 52,
  106, 110, 66, 64,
  109, 123, 49, 65,
  113, 139, 35, 66,
  116, 192,117, 98,
  124, 255,255,137,
  168, 100,180,155,
  255,  22,121,174};

// Gradient palette "Magenta_Evening_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Magenta_Evening.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Magenta_Evening_gp ) {
    0,  71, 27, 39,
   31, 130, 11, 51,
   63, 213,  2, 64,
   70, 232,  1, 66,
   76, 252,  1, 69,
  108, 123,  2, 51,
  255,  46,  9, 35};

// Gradient palette "Pink_Purple_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Pink_Purple.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( Pink_Purple_gp ) {
    0,  19,  2, 39,
   25,  26,  4, 45,
   51,  33,  6, 52,
   76,  68, 62,125,
  102, 118,187,240,
  109, 163,215,247,
  114, 217,244,255,
  122, 159,149,221,
  149, 113, 78,188,
  183, 128, 57,155,
  255, 146, 40,123};

// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};

// Gradient palette "es_autumn_19_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/autumn/tn/es_autumn_19.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_autumn_19_gp ) {
    0,  26,  1,  1,
   51,  67,  4,  1,
   84, 118, 14,  1,
  104, 137,152, 52,
  112, 113, 65,  1,
  122, 133,149, 59,
  124, 137,152, 52,
  135, 113, 65,  1,
  142, 139,154, 46,
  163, 113, 13,  1,
  204,  55,  3,  1,
  249,  17,  1,  1,
  255,  17,  1,  1};

// Gradient palette "BlacK_Blue_Magenta_White_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Blue_Magenta_White.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( BlacK_Blue_Magenta_White_gp ) {
    0,   0,  0,  0,
   42,   0,  0, 45,
   84,   0,  0,255,
  127,  42,  0,255,
  170, 255,  0,255,
  212, 255, 55,255,
  255, 255,255,255};

// Gradient palette "BlacK_Magenta_Red_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Magenta_Red.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( BlacK_Magenta_Red_gp ) {
    0,   0,  0,  0,
   63,  42,  0, 45,
  127, 255,  0,255,
  191, 255,  0, 45,
  255, 255,  0,  0};

// Gradient palette "BlacK_Red_Magenta_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Red_Magenta_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( BlacK_Red_Magenta_Yellow_gp ) {
    0,   0,  0,  0,
   42,  42,  0,  0,
   84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0,255,
  212, 255, 55, 45,
  255, 255,255,  0};

// Gradient palette "Blue_Cyan_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Cyan_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Blue_Cyan_Yellow_gp ) {
    0,   0,  0,255,
   63,   0, 55,255,
  127,   0,255,255,
  191,  42,255, 45,
  255, 255,255,  0};

// Gradient palette "BlacK_Blue_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Blue.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( BlacK_Blue_gp ) {
    0,   0,  0,  0,
  127,   0,  0, 45,
  255,   0,  0,255};

// Gradient palette "Stripped_Spectrum_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/strips/tn/Stripped_Spectrum.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 512 bytes of program space.

DEFINE_GRADIENT_PALETTE( Stripped_Spectrum_gp ) {
    0, 255,  0,  0,
    2, 255,  1,  0,
    4, 255,  1,  0,
    6, 255,  4,  0,
    9, 255,  8,  0,
   11, 255, 13,  0,
   13, 255, 21,  0,
   15, 255, 29,  0,
   15,   0,  0,  0,
   18,   0,  0,  0,
   20,   0,  0,  0,
   22,   0,  0,  0,
   25,   0,  0,  0,
   27,   0,  0,  0,
   29,   0,  0,  0,
   31,   0,  0,  0,
   31, 255,135,  0,
   34, 255,157,  0,
   36, 255,182,  0,
   38, 255,207,  0,
   40, 255,235,  0,
   43, 242,255,  0,
   45, 210,255,  0,
   47, 179,255,  0,
   47,   0,  0,  0,
   50,   0,  0,  0,
   52,   0,  0,  0,
   54,   0,  0,  0,
   56,   0,  0,  0,
   59,   0,  0,  0,
   61,   0,  0,  0,
   63,   0,  0,  0,
   63,  42,255,  0,
   66,  31,255,  0,
   68,  22,255,  0,
   57,   0,  0,  0,
  159,   0,  0,  0,
  159,   0, 12,255,
  161,   0,  7,255,
  163,   0,  3,255,
  166,   0,  1,255,
  168,   0,  1,255,
  170,   1,  0,255,
  173,   1,  0,255,
  175,   1,  0,255,
  175,   0,  0,  0,
  177,   0,  0,  0,
  179,   0,  0,  0,
  182,   0,  0,  0,
  184,   0,  0,  0,
  186,   0,  0,  0,
  188,   0,  0,  0,
  191,   0,  0,  0,
  191,  42,  0,255,
  193,  54,  0,255,
  195,  69,  0,255,
  198,  86,  0,255,
  200, 106,  0,255,
  202, 128,  0,255,
  204, 152,  0,255,
  207, 179,  0,255,
  207,   0,  0,  0,
  209,   0,  0,  0,
  211,   0,  0,  0,
  214,   0,  0,  0,
  216,   0,  0,  0,
  218,   0,  0,  0,
  220,   0,  0,  0,
  223,   0,  0,  0,
  223, 255,  0,123,
  225, 255,  0,112,
  227, 255,  0,103,
  229, 255,  0, 93,
  232, 255,  0, 84,
  234, 255,  0, 75,
  236, 255,  0, 67,
  239, 255,  0, 60,
  239,   0,  0,  0,
  241,   0,  0,  0,
  243,   0,  0,  0,
  245,   0,  0,  0,
  248,   0,  0,  0,
  250,   0,  0,  0,
  252,   0,  0,  0,
  255,   0,  0,  0};

// Gradient palette "bhw1_14_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_14.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw1_14_gp ) {
    0,   0,  0,  0,
   12,   1,  1,  3,
   53,   8,  1, 22,
   80,   4,  6, 89,
  119,   2, 25,216,
  145,   7, 10, 99,
  186,  15,  2, 31,
  233,   2,  1,  5,
  255,   0,  0,  0};

// Gradient palette "bhw2_22_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_22.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_22_gp ) {
    0,   0,  0,  0,
   99, 227,  1,  1,
  130, 249,199, 95,
  155, 227,  1,  1,
  255,   0,  0,  0};

// Gradient palette "bhw3_52_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw3/tn/bhw3_52.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw3_52_gp ) {
    0,  31,  1, 27,
   45,  34,  1, 16,
   99, 137,  5,  9,
  132, 213,128, 10,
  175, 199, 22,  1,
  201, 199,  9,  6,
  255,   1,  0,  1};

// Gradient palette "Deep_Sea_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ggr/tn/Deep_Sea.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Deep_Sea_gp ) {
    0,   0,  1,  2,
  148,   1,  7, 24,
  194,   2, 31, 77,
  226,   1,108,144,
  255,   0,237,235};

// Gradient palette "ofaurora_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/ofaurora.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 124 bytes of program space.

DEFINE_GRADIENT_PALETTE( ofaurora_gp ) {
    0, 255,241,242,
   10, 237,  1,  9,
   15,  42,  1,  2,
   22,   1,  1,  1,
   35, 237,  1,  9,
   48, 255,241,242,
   58, 239, 57,  1,
   66,   1,  1,  1,
   76, 239, 57,  1,
   84, 255,241,242,
   94, 242,217,  1,
  101,   1,  1,  1,
  109, 242,217,  1,
  119, 255,241,242,
  127,   9,144, 36,
  132,   1,  1,  1,
  140,   9,144, 36,
  147, 255,241,242,
  158,   1,156,186,
  168,   1,  1,  1,
  178,   1,156,186,
  186, 255,241,242,
  198,  16, 13,255,
  206,   1,  1,  1,
  216,  16, 13,255,
  224, 255,241,242,
  234,  78,  1,156,
  237,  14,  1, 30,
  239,   1,  1,  1,
  244,  78,  1,156,
  255, 255,241,242};

// Gradient palette "shikon_22_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gacruxa/shikon/tn/shikon-22.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 84 bytes of program space.

DEFINE_GRADIENT_PALETTE( shikon_22_gp ) {
    0,   2,  2,  2,
   24, 217, 47,  0,
   26,   2,  2,  2,
   49, 126,  0,  1,
   51,   2,  2,  2,
   73,   0,223, 64,
   76,   2,  2,  2,
  100, 126,  0, 59,
  102,   2,  2,  2,
  125,   0,219,219,
  127,   2,  2,  2,
  151,   0, 25,219,
  153,   2,  2,  2,
  175,  44,223,  0,
  179,   2,  2,  2,
  201,  55,  2, 32,
  203,   2,  2,  2,
  226, 199,223,  0,
  229,   2,  2,  2,
  252,  26,  0,219,
  255,   2,  2,  2};

// Gradient palette "shikon_23_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gacruxa/shikon/tn/shikon-23.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 164 bytes of program space.

DEFINE_GRADIENT_PALETTE( shikon_23_gp ) {
    0,   2,  2,  2,
    8,   2,  2,  2,
   10, 126,  0,  1,
   24, 126,  0,  1,
   25,   2,  2,  2,
   37,   2,  2,  2,
   38, 199,223,  0,
   49, 199,223,  0,
   50,   2,  2,  2,
   63,   2,  2,  2,
   65,  44,223,  0,
   77,  44,223,  0,
   77,   2,  2,  2,
   88,   2,  2,  2,
   89,   0,223, 64,
  101,   0,223, 64,
  102,   2,  2,  2,
  113,   2,  2,  2,
  114,   0,219,219,
  128,   0,219,219,
  129,   2,  2,  2,
  140,   2,  2,  2,
  141,   0, 25,219,
  153,   0, 25,219,
  153,   2,  2,  2,
  165,   2,  2,  2,
  166, 126,  0, 59,
  178, 126,  0, 59,
  179,   2,  2,  2,
  191,   2,  2,  2,
  192,  55,  2, 32,
  204,  55,  2, 32,
  205,   2,  2,  2,
  216,   2,  2,  2,
  217, 217, 47,  0,
  228, 217, 47,  0,
  228,   2,  2,  2,
  242,   2,  2,  2,
  243,  26,  0,219,
  250,  26,  0,219,
  255,   2,  2,  2};

// Gradient palette "curvature_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/grass/tn/curvature.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( curvature_gp ) {
    0,   0,  0,  0,
  114,   0,  0,255,
  126,  22, 55,255,
  127,   0,255,255,
  127, 255,255,255,
  127, 255,255,  0,
  128, 255, 55,  0,
  140, 255,  0,  0,
  255, 255,  0,255};

// Gradient palette "rstcurv_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/grass/tn/rstcurv.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( rstcurv_gp ) {
    0,  42,  0,255,
  114,   0,  0,255,
  126,   0, 55,255,
  127,   0,255,255,
  127, 135,255,138,
  127, 255,255,  0,
  128, 255, 55,  0,
  140, 255,  0,  0,
  255, 255,  0,138};

// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly
// by name every time.
// Since it is const, this array could also be moved
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Sunset_Real_gp,
  es_rivendell_15_gp,
  es_ocean_breeze_036_gp,
  rgi_15_gp,
  retro2_16_gp,
  Analogous_1_gp,
  es_pinksplash_08_gp,
  Coral_reef_gp,
  es_ocean_breeze_068_gp,
  es_pinksplash_07_gp,
  es_vintage_01_gp,
  es_landscape_64_gp,
  es_landscape_33_gp,
  rainbowsherbet_gp,
  gr65_hult_gp,
  gr64_hult_gp,
  GMT_drywet_gp,
  ib_jul01_gp,
  es_vintage_57_gp,
  ib15_gp,
  Fuschia_7_gp,
  es_emerald_dragon_08_gp,
  lava_gp,
  fire_gp,
  Colorfull_gp,
  Magenta_Evening_gp,
  Pink_Purple_gp,
  es_autumn_19_gp,
  BlacK_Blue_Magenta_White_gp,
  BlacK_Magenta_Red_gp,
  BlacK_Red_Magenta_Yellow_gp,
  Blue_Cyan_Yellow_gp };


// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount =
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
