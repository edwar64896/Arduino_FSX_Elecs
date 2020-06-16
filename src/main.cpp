
#include <Arduino.h>


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

#define USEFONT u8x8_font_8x13_1x2_f 

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

#define R1   0
#define R2   6
#define R1_1 6
#define R2_1 6

char * szbuf=new char[50];

char * szPad(char * szValue) {
  sprintf(szbuf,"%s   ",szValue);
  return szbuf;
}

void display_bAmps(char* szValue) {

  if (active==MODE_BATT) {
    display.drawString(R1_1,R1,"      ");
    display.drawString(R1_1,R1,szPad(szValue+1));
  }
}

void display_bVolts(char* szValue) {
  if (active==MODE_BATT) {
    display.drawString(R2_1,R2,"      ");
    display.drawString(R2_1,R2,szPad(szValue));;
  }
}

void display_mbAmps(char* szValue) {
  if (active==MODE_MAINBUS) {
    display.drawString(R1_1,R1,"      ");
    display.drawString(R1_1,R1,szPad(szValue));
  }
}

void display_mbVolts(char* szValue) {
  if (active==MODE_MAINBUS) {
    display.drawString(R2_1,R2,"      ");
    display.drawString(R2_1,R2,szPad(szValue));;
  }
}

void display_g1Amps(char* szValue) {
  if (active==MODE_GEN1) {
    display.drawString(R1_1,R1,"      ");
    display.drawString(R1_1,R1,szPad(szValue));
  }
}

void display_g1Volts(char* szValue) {
  if (active==MODE_GEN1) {
    display.drawString(R2_1,R2,"      ");
    display.drawString(R2_1,R2,szPad(szValue));;
  }
}

void display_g2Amps(char* szValue) {
  if (active==MODE_GEN2) {
    display.drawString(R1_1,R1,"      ");
    display.drawString(R1_1,R1,szPad(szValue));
  }
}

void display_g2Volts(char* szValue) {
  if (active==MODE_GEN2) {
    display.drawString(R2_1,R2,"      ");
    display.drawString(R2_1,R2,szPad(szValue));;
  }
}

void display_aVolts(char* szValue) {
  if (active==MODE_APU) {
    display.drawString(R1_1,R1,"      ");
    display.drawString(R1_1,R1,szPad(szValue));
    display.drawString(R2_1,R2,"n/a    ");
  }
}

gspSerialResponse srbVolts("?I", 4, display_bVolts );
gspSerialResponse srbAmps("?J", 4, display_bAmps  );
gspSerialResponse srmbVolts("?K", 4, display_mbVolts);
gspSerialResponse srmbAmps("?L", 2, display_mbAmps);
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

  display.setFont(USEFONT   );  // Set a custom font
  display.drawString(0,R1,"AMPs");
  display.drawString(0,R2,"VLTs");

}  // End of setup

void drawMode(char * szMode) {
  display.setFont(USEFONT  );  // Set a custom font
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
  
  //display.setFont(USEFONT  );  // Set a custom font

  gspSerialResponse::checkAll();
  gspFlash::checkAll();

} 