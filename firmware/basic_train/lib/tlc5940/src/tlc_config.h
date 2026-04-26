#ifndef TLC_CONFIG_H
#define TLC_CONFIG_H

// --- Wiring on ESP32-S3-MINI-1 ---------------------------------------------
//
//  ESP32-S3 GPIO   ->  TLC5940 pin
//  ---------------     -----------
//      11           ->  2  (GSCLK)
//      12           ->  3  (BLANK)
//      13           ->  6  (SIN  / SPI MOSI)
//      14           ->  5  (SCLK / SPI SCK)
//      15           ->  4  (XLAT / SPI CS)
//      16           ->  7  (VPRG)   -- left disabled below
//
// IMPORTANT: on ESP32-S3-MINI-1, GPIO26..32 are bonded to the internal SPI
// flash and MUST NOT be used. The upstream library defaulted GSCLK=27 and
// BLANK=26, which would crash the chip. We override them here.
// ---------------------------------------------------------------------------

// SPI pins
#define HSPI_SCLK 14    // -> TLC pin 5  (SCLK)
#define HSPI_MOSI 13    // -> TLC pin 6  (SIN)
#define HSPI_CS   15    // -> TLC pin 4  (XLAT)
#define HSPI_MISO -1    // unused: TLC5940 chain is write-only

// Greyscale clock (PWM via LEDC)
#define GSCLK_PIN              11
#define GSCLK_FREQ             10000000   // 10 MHz
#define GSCLK_TIMER            0
#define GSCLK_TIMER_RESULUTION 1

// BLANK pin (toggled in the timer ISR)
#define BLANK_PIN        12
#define BLANK_FREQ       2441
#define BLANK_HIGH       1
#define BLANK_RESULUTION 15

// Optional features -- left OFF
#ifndef VPRG_ENABLED
#define VPRG_ENABLED 0
#endif
#ifndef XERR_ENABLED
#define XERR_ENABLED 0
#endif

#ifndef NUM_TLCS
#define NUM_TLCS 1
#endif

#endif
