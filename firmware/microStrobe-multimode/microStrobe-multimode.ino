/*
 *
 * Police Strobe v2
 * by keoshi
 *
 * inspiration:
 * https://www.youtube.com/watch?v=YZSK80aEbRE
 *
 */

#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Define on-board LED pin
#define ledPin     13
#define neoPixel   2

int pattern = 0;
int numPixels  = 8;
int brightness = 220; // 1 to 255

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip(numPixels, neoPixel, NEO_GRB + NEO_KHZ800);

void setup() {
	pattern = EEPROM.read(0);
  pattern = (pattern + 1) % 3; // Cycle 0→1→2→0
  EEPROM.write(0, pattern);
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {
	switch(pattern) {
		case 0:
			bikeRearLight();
			break;
		case 1:
			pulseTrailsSeq( 3 );
			break;
		case 2:
			flashAll();
			fastSwitch( 40, 80 );
			flashAll();
			policeStrobe( 40 );
			flashAll();
			policeWave( 10 );
			flashAll();
			converge( 20 );
			flashAll();
			break;
	}
}

void bikeRearLight() {
	int bikeDelay = 100;
	// Quick double flash
	clearStrip();
	for ( int pixel = 0; pixel < strip.numPixels(); pixel++ ) {
		strip.setPixelColor( pixel, strip.Color(brightness, 0, 0) );
	}
	strip.show();
	delay(bikeDelay);
	clearStrip();
	delay(bikeDelay);
	for ( int pixel = 0; pixel < strip.numPixels(); pixel++ ) {
		strip.setPixelColor( pixel, strip.Color(brightness, 0, 0) );
	}
	strip.show();
	delay(bikeDelay);
	clearStrip();
	delay(bikeDelay);
}

void pulseTrailsSeq( int iterations ) {
	for ( int i = 0; i < iterations; i++ ) {
		pulseTrails( 2, 40, 40 );
	}
	for ( int i = 0; i < iterations; i++ ) {
		pulseTrails( 1, 20, 30 );
	}
}

void pulseTrails ( int eyeSize, int speedDelay, int returnDelay ) {
	for ( int pixel = 0; pixel < strip.numPixels() - eyeSize - 2; pixel++ ) {
		clearStrip();
		strip.setPixelColor( pixel, strip.Color( brightness / 10 , 0, 0) );
		for ( int i = 1; i <= eyeSize; i++ ) {
			strip.setPixelColor( pixel + i, strip.Color( brightness, 0, 0) );
		}
		strip.setPixelColor( pixel + eyeSize + 1, strip.Color( brightness / 10, 0, 0) );
		strip.show();
		delay( speedDelay );
	}
	
	delay( returnDelay );

	for ( int pixel = strip.numPixels() - eyeSize - 2; pixel > 0; pixel-- ) {
		clearStrip();
		strip.setPixelColor( pixel, strip.Color( 0, 0, brightness / 10 ) );
		for ( int i = 1; i <= eyeSize; i++ ) {
			strip.setPixelColor( pixel + i, strip.Color( 0, 0, brightness) );
		}
		strip.setPixelColor( pixel + eyeSize + 1, strip.Color( 0, 0, brightness / 10) );
		strip.show();
		delay( speedDelay );
	}
}

void fastSwitch( int delayValue1, int delayValue2 ) {
	for ( int i = 0; i < 5; i++ ) {
		switchLight( 0, 0, 0, brightness, delayValue1 );
		switchLight( 1, 0, 0, brightness, delayValue1 );
		switchLight( 2, 0, 0, brightness, delayValue1 );
		switchLight( 3, 0, 0, brightness, delayValue1 );
		switchLight( 4, brightness, 0, 0, delayValue1 );
		switchLight( 5, brightness, 0, 0, delayValue1 );
		switchLight( 6, brightness, 0, 0, delayValue1 );
		switchLight( 7, brightness, 0, 0, delayValue1 );

		switchLight( 0, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 1, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 2, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 3, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 4, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 5, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 6, 0, 0, 0, delayValue2 - i * 20 );
		switchLight( 7, 0, 0, 0, delayValue2 - i * 20 );
	}
}

void flashAll() {
	for ( int i = 0; i < 4; i++ ) {
		for ( int pixel = 0; pixel < strip.numPixels(); pixel++ ) {
			strip.setPixelColor( pixel, strip.Color(brightness, brightness, brightness) );
			strip.show();
		}
		delay(40);
		clearStrip();
		delay(80);
	}
}

void policeStrobe( int delayValue ) {
	for ( int i = 0; i < 3; i++ ) {
		for ( int i = 0; i < 3; i++ ) {
			for ( int i = 0; i < 5; i++ ) {
				for ( int pixel = 0; pixel < 4; pixel++ ) {
					switchLight( pixel, 0, 0, brightness, 0 );
				}
				delay( delayValue );
				clearStrip();
				delay( delayValue );
			}
			for ( int i = 0; i < 5; i++ ) {
				for ( int pixel = 4; pixel < 8; pixel++ ) {
					switchLight( pixel, brightness, 0, 0, 0 );
				}
				delay( delayValue );
				clearStrip();
				delay( delayValue );
			}
		}
		for ( int i = 0; i < 6; i++ ) {
			for ( int pixel = 2; pixel < 6; pixel++ ) {
				switchLight( pixel, brightness, brightness, brightness, 0 );
			}
			delay( delayValue );
			clearStrip();
			delay( delayValue );
		}
	}
}

void policeWave( int delayValue ) {
	for ( int i = 0; i < 5; i++ ) {
		for ( int pixel = 0; pixel < strip.numPixels(); pixel++ ) {
			if ( pixel < 4 )  {
				switchLight( pixel, brightness, 0, 0, delayValue );
				switchLight( pixel + 1, brightness, 0, 0, delayValue );
				switchLight( pixel + 2, brightness, 0, 0, delayValue );
			} else {
				switchLight( pixel, 0, 0, brightness, delayValue );
				switchLight( pixel + 1, 0, 0, brightness, delayValue );
				switchLight( pixel + 2, 0, 0, brightness, delayValue );
			}
			switchLight( pixel, 0, 0, 0, delayValue );
		}
		for ( int pixel = 8; pixel > -1; pixel-- ) {
			if ( pixel < 4 )  {
				switchLight( pixel, brightness, 0, 0, delayValue );
				switchLight( pixel - 1, brightness, 0, 0, delayValue );
				switchLight( pixel - 2, brightness, 0, 0, delayValue );
			} else {
				switchLight( pixel, 0, 0, brightness, delayValue );
				switchLight( pixel - 1, 0, 0, brightness, delayValue );
				switchLight( pixel - 2, 0, 0, brightness, delayValue );
			}
			switchLight( pixel, 0, 0, 0, delayValue );
		}
	}
}

void converge( int delayValue ) {
	for ( int i = 0; i < 5; i++ ) {
		switchLight( 0, 0, 0, brightness, delayValue );
		switchLight( 7, brightness, 0, 0, delayValue );
		switchLight( 1, 0, 0, brightness, delayValue );
		switchLight( 6, brightness, 0, 0, delayValue );
		switchLight( 2, 0, 0, brightness, delayValue );
		switchLight( 5, brightness, 0, 0, delayValue );
		switchLight( 3, 0, 0, brightness, delayValue );
		switchLight( 4, brightness, 0, 0, delayValue );
		switchLight( 1, 0, 0, 0, delayValue );
		switchLight( 6, 0, 0, 0, delayValue );
		switchLight( 0, 0, 0, 0, delayValue );
		switchLight( 7, 0, 0, 0, delayValue );
		switchLight( 1, 0, 0, 0, delayValue );
		switchLight( 6, 0, 0, 0, delayValue );
		switchLight( 2, 0, 0, 0, delayValue );
		switchLight( 5, 0, 0, 0, delayValue );
		switchLight( 3, 0, 0, 0, delayValue );
		switchLight( 4, 0, 0, 0, delayValue );
	}
}

void switchLight( int pixelNum, int colorR, int ColorG, int colorB, int delayValue ) {
	strip.setPixelColor( pixelNum, strip.Color(colorR, ColorG, colorB) );
	strip.show();
	delay( delayValue );
}

void clearStrip() {
	strip.clear();
	strip.show();
}
