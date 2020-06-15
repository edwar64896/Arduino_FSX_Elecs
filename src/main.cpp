
#include <Arduino.h>
/*
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
*/

#include <U8x8lib.h>
#include <gspswitch.h>
#include <gspserialresponse.h>
#include <gspflash.h>

#define MODE_BATT     2
#define MODE_MAINBUS  3
#define MODE_GEN1     4
#define MODE_GEN2     5
#define MODE_APU      6

U8X8_SSD1306_128X64_NONAME_HW_I2C display;

int active = 0;

void setModeBatt();
void setModeMainBus();
void setModeGen1();
void setModeGen2();
void setModeApu();

gspSwitch  swBatt(MODE_BATT,setModeBatt,1);
gspSwitch  swMainBus(MODE_MAINBUS,setModeMainBus,1);
gspSwitch  swGen1(MODE_GEN1,setModeGen1,1);
gspSwitch  swGen2(MODE_GEN2,setModeGen2,1);
gspSwitch  swApu(MODE_APU,setModeApu,1);

void display_bAmps(char* szValue) {
  if (active==MODE_BATT) {
    display.drawString(6,0,"      ");
    display.drawString(6,0,szValue);
  }
}

void display_bVolts(char* szValue) {
  Serial.println(szValue);
  if (active==MODE_BATT) {
    display.drawString(6,6,"      ");
    display.drawString(6,6,szValue);
  }
}

void display_mbAmps(char* szValue) {
  if (active==MODE_MAINBUS) {
    display.drawString(6,0,"      ");
    display.drawString(6,0,szValue);
  }
}

void display_mbVolts(char* szValue) {
  if (active==MODE_MAINBUS) {
    display.drawString(6,6,"      ");
    display.drawString(6,6,szValue);
  }
}

void display_g1Amps(char* szValue) {
  if (active==MODE_GEN1) {
    display.drawString(6,0,"      ");
    display.drawString(6,0,szValue);
  }
}

void display_g1Volts(char* szValue) {
  if (active==MODE_GEN1) {
    display.drawString(6,6,"      ");
    display.drawString(6,6,szValue);
  }
}

void display_g2Amps(char* szValue) {
  if (active==MODE_GEN2) {
    display.drawString(6,0,"      ");
    display.drawString(6,0,szValue);
  }
}

void display_g2Volts(char* szValue) {
  if (active==MODE_GEN2) {
    display.drawString(6,6,"      ");
    display.drawString(6,6,szValue);
  }
}

void display_aVolts(char* szValue) {
  if (active==MODE_APU) {
    display.drawString(6,6,"      ");
    display.drawString(6,6,szValue);
  }
}

gspSerialResponse srbVolts("?I", 4, display_bVolts );
gspSerialResponse srbAmps("?J", 4, display_bAmps  );
gspSerialResponse srmbVolts("?K", 4, display_mbVolts);
gspSerialResponse srmbAmps("?L", 3, display_mbAmps);
gspSerialResponse srg1Amps("?l", 4, display_g1Amps);
gspSerialResponse srg2Amps("?m", 4, display_g2Amps);
gspSerialResponse srg1Volts("<y", 2, display_g1Volts);
gspSerialResponse srg2Volts("<z", 2, display_g2Volts);
gspSerialResponse sraVolts("$i", 2, display_aVolts);


gspFlash led(LED_BUILTIN);

void setup()  // Start of setup
{

  gspGrouped::register_instance(&swBatt);
  gspGrouped::register_instance(&swMainBus);
  gspGrouped::register_instance(&swGen1);
  gspGrouped::register_instance(&swGen2);
  gspGrouped::register_instance(&swApu);
  
  gspGrouped::register_instance(&srbVolts );
  gspGrouped::register_instance(&srbAmps );
  gspGrouped::register_instance(&srmbVolts );
  gspGrouped::register_instance(&srmbAmps );
  gspGrouped::register_instance(&srg1Amps );
  gspGrouped::register_instance(&srg2Amps );
  gspGrouped::register_instance(&srg1Volts );
  gspGrouped::register_instance(&srg2Volts );
  gspGrouped::register_instance(&sraVolts );

  gspGrouped::register_instance(&led);

  Serial.begin(115200);

  delay(1000);  // This delay is needed to let the display to initialize

  if(!display.begin()) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  led.flashFast();

  active=MODE_BATT;

  display.setFont(u8x8_font_8x13B_1x2_r);  // Set a custom font
  display.drawString(0,0,"AMPs");
  display.drawString(0,6,"VLTs");

}  // End of setup

void drawMode(char * szMode) {
  display.setFont(u8x8_font_8x13B_1x2_r);  // Set a custom font
  display.drawString(1,3,szMode);
}

void setModeBatt(){
  active=MODE_BATT;
  drawMode("Battery       ");
  srbAmps.update();
  srbVolts.update();
}

void setModeMainBus(){
  active=MODE_MAINBUS;
  drawMode("Main Bus      ");
  srmbAmps.update();
  srmbVolts.update();
}

void setModeGen1(){
  active=MODE_GEN1;
  drawMode("Generator 1   ");
  srg1Amps.update();
  srg1Volts.update();
}

void setModeGen2(){
  active=MODE_GEN2;
  drawMode("Generator 2   ");
  srg2Amps.update();
  srg2Volts.update();  
} 

void setModeApu(){
  active=MODE_APU;
  drawMode("APU           ");
  sraVolts.update();
}

void loop() 
{

  gspSwitch::checkAll();
  gspSerialResponse::checkAll();
  gspFlash::checkAll();

} 