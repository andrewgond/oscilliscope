


#include <_Teensy.h>

#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"

//Encoder stuff
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#include <ADC.h>
#include <ADC_util.h>

//********
// Stuff for Display
//********
// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

#include <math.h>  // Needed for sin()

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

//******
// Stuff for the UI 
//******

static const int SLX = 320;
static const int SLY = 240;



// Mode 1 Time Axis Scaling
// Mode 2 Voltage Axis Scaling
// Mode 3 Mode?
int mode;
int mode_values[3] = {1,1,0};

const int SAMPLE_CNT = 4000;
uint16_t samples[SAMPLE_CNT];
bool Trigger = false;

//Encoder Stuff

int32_t encoder_position;

#define SS_SWITCH        24
#define SS_NEOPIX        6

#define SEESAW_ADDR          0x36

Adafruit_seesaw ss;

seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);



// ADC Setup
// Sampling setup 
ADC *adc = new ADC();


const int SAMPLE_COUNT = 4000;
double ch1_samples[SAMPLE_COUNT];
double ch1_max ;
double ch1_min ; 
double ch1_mid ;

double ch2_samples[SAMPLE_COUNT];
double ch2_max ;
double ch2_min ; 
double ch2_mid ; 




void trigger(){
	Trigger = !(Trigger && Trigger);
}

void change_val(int& m, bool increase) {
	if (mode == 0 || mode == 1) { // X or Y scaling
		if (increase) m++;
		else m--;
		m = constrain(m, -100, 100);
	} else if (mode == 2) { // Channel select
		if (increase) m++;
		else m--;
		m = constrain(m, 0, 2);
	}
}

void printCh1(){
	Serial.println("azsfbajsbkvds");
	Serial.println("CH 1");
	for (int i = 0 ; i < SAMPLE_CNT ; i++){
		Serial.print(ch1_samples[i]);
		Serial.print(",");
	}
	Serial.println("asdsnvsodfnvb");
	Serial.println("CH 2");
	for (int i = 0 ; i < SAMPLE_CNT ; i++){
		Serial.print(ch2_samples[i]);
		Serial.print(",");
	}
	Serial.println("asdsnvsodfnvb");
}

void refresh_screen(){
	
		tft.fillScreen(ILI9341_WHITE);  
		tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
		tft.setCursor(0, 0);
		tft.print("Mode: ");
		if(mode == 0) tft.println("X Scale");
		else if (mode == 1) tft.println("Y Scale");
		else if (mode == 2) tft.println("Channel Select:");

		//tft.print("X_Scale: ");
		//tft.print(mode_values[0]);
		//tft.print("| Y Scale:");
		//tft.print(mode_values[1]);
		tft.print("Channel ");
		if (mode_values[2] == 2) tft.println("Both");
		else tft.println(mode_values[2]+1);
		if (Trigger){
			tft.println("Trigger On");
		}

		
	
}

void draw_sine_wave(float amplitude, float frequency, uint16_t color) {
  	tft.fillScreen(ILI9341_WHITE);
		tft.setTextColor(ILI9341_BLACK); tft.setTextSize(2);
		tft.setCursor(0, 0);
		tft.print("Mode: ");
		if(mode == 0) tft.println("X Scale");
		else if (mode == 1) tft.println("Y Scale");
		else if (mode == 2) tft.println("Channel Select:");

		tft.print("X_Scale: ");
		tft.print(mode_values[0]);
		tft.print("| Y Scale:");
		tft.print(mode_values[1]);
		tft.print("| Channel ");
		if (mode_values[2] == 2) tft.println("Both");
		else tft.println(mode_values[2]+1);
		tft.print("Trigger: ");
		if (Trigger) tft.println("On");
		else tft.println("Off");
		// Draw midline
		tft.drawLine(0, SLY / 2, SLX, SLY / 2, ILI9341_LIGHTGREY);

  for (int x = 0; x < SLX - 1; x++) {
    float radians1 = (2 * PI * frequency * x) / SLX;
    float radians2 = (2 * PI * frequency * (x + 1)) / SLX;

    int y1 = SLY / 2 - sin(radians1) * amplitude;
    int y2 = SLY / 2 - sin(radians2) * amplitude;

    tft.drawLine(x, y1, x + 1, y2, color);
  }
}

void gather_samples() {

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    adc->adc0->startSingleRead(14);
    adc->adc1->startSingleRead(15);

    while (!adc->adc0->isComplete() || !adc->adc1->isComplete());

    int raw_ch1 = adc->adc0->readSingle();
    int raw_ch2 = adc->adc1->readSingle();

		if (i == 0){
			ch1_max = raw_ch1;
			ch1_min = raw_ch1;
			ch2_max = raw_ch2;
			ch2_min = raw_ch2;
		}
		if ( ch1_max < raw_ch1){
			ch1_max = raw_ch1;
		}
		else if (ch1_min > raw_ch1){
			ch1_min = raw_ch1;
		}
		if ( ch2_max < raw_ch2){
			ch2_max = raw_ch2;
		}
		else if (ch2_min > raw_ch2){
			ch2_min = raw_ch2;
		}
	

		ch1_samples[i] = raw_ch1;
		ch2_samples[i] = raw_ch2;
	}
	ch1_mid = ((ch1_max-ch1_min) / 2) + ch1_min;
	ch2_mid = ((ch2_max-ch2_min) / 2) + ch2_min;
}

void draw_waveform() {
	int x_factor = mode_values[0];
	int y_factor = mode_values[1];

	float zoom = (x_factor >= 0) ? (x_factor + 1) : 1.0 / (abs(x_factor) + 1);
	float yscale = (y_factor >= 0) ? (y_factor + 1) : 1.0 / (abs(y_factor) + 1);
	int channel = mode_values[2];

	int visible_points = SLX * zoom;               // how many samples to show
	int start_index = max(0, SAMPLE_COUNT - visible_points);  // show last N points

	for (int x = 0; x < SLX - 1; x++) {
		int i1 = start_index + x * zoom;
		int i2 = start_index + (x + 1) * zoom;

		if (i2 >= SAMPLE_COUNT) break;

		if (channel == 0 || channel == 2) {
			int y1 = SLY / 2 - (ch1_samples[i1] - ch1_mid) * yscale;
			int y2 = SLY / 2 - (ch1_samples[i2] - ch1_mid) * yscale;
			tft.drawLine(x, y1, x + 1, y2, ILI9341_BLUE);
		}

		if (channel == 1 || channel == 2) {
			int y1 = SLY / 2 - (ch2_samples[i1] - ch2_mid) * yscale;
			int y2 = SLY / 2 - (ch2_samples[i2] - ch2_mid) * yscale;
			tft.drawLine(x, y1, x + 1, y2, ILI9341_RED);
		}
	}
}



void setup() {
	Serial.begin(115200);
	// put your setup code here, to run once:
	tft.begin();
	tft.setRotation(3);        // Try values from 0–3
	tft.invertDisplay(true);
	tft.fillScreen(ILI9341_WHITE);
	ss.begin(SEESAW_ADDR);
	encoder_position = ss.getEncoderPosition();

	adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // How long to sample
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // How fast to convert
  adc->adc0->setAveraging(1);                                           // No averaging for fastest speed
	adc->adc0->setResolution(12);


	adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);     // How long to sample
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED); // How fast to convert
  adc->adc1->setAveraging(1);                                           // No averaging for fastest speed
	adc->adc1->setResolution(12);

	

	ss.pinMode(SS_SWITCH, INPUT_PULLUP);
	Serial.println("Turning on interrupts");
  delay(10);
  ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
  ss.enableEncoderInterrupt();
}

void loop() {
	// DECISION MATRIX POLLING 
  if (! ss.digitalRead(SS_SWITCH)) {
		mode += 1 ;
		if (mode > 2){
			mode = 0; 
		}
		delay(300);
		if (! ss.digitalRead(SS_SWITCH)) {
			mode --;
			if (mode == -1) mode = 2;
			trigger();
			printCh1();
			while (!ss.digitalRead(SS_SWITCH)){
				delay(100);
			}
			refresh_screen();
			draw_waveform();
		}
		else{
			refresh_screen();
			draw_waveform();
		}
  }
	int32_t new_position = ss.getEncoderPosition();
  // did we move around


  if (encoder_position > new_position) {
		encoder_position = new_position;
		change_val(mode_values[mode], false);
		Serial.println(mode_values[mode]);
		refresh_screen();
		draw_waveform();
	}
	else if (encoder_position < new_position){
		encoder_position = new_position;
		change_val(mode_values[mode], true);
		Serial.println(mode_values[mode]);
		refresh_screen();
		draw_waveform();
	}
	if (!Trigger){
			gather_samples();
			refresh_screen();
		draw_waveform();
	}
	}

	
	





	//SAMPLING FROM ADC




	//DISPLAYING ON THINGY


//signal_collect
// Outputs what we want to display [Max sampling rate]




