#include <MCP2515.h> // the OBD2 library depends on the CAN library
#include <OBD2.h>
#include <U8g2lib.h>

#define DEMO false

#define image_width  10
#define image_height 6

// define battery Symbol Bitmap
static unsigned char battery_icon[] U8X8_PROGMEM  = {0x86,0xfd,0xff,0xff,0xfb,0xff,0x31,0xfe,0xfb,0xff,0xff,0xff};

#define CS_PIN 17
#define INT_PIN 20
#define SCL 10
#define SI 11
#define CS_LCD 13
#define RS 21
#define RSE 22

#define PID_ENGINE_RPM                0x0C
#define PID_VEHICLE_SPEED             0x0D
#define PID_COOLANT_TEMP              0x05
#define PID_ENGINE_OIL_TEMPERATURE    0x5C
#define PID_CONTROL_MODULE_VOLTAGE    0x42

U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2(U8G2_R0, SCL, SI, CS_LCD, RS, RSE);

int rpm = 0; 
int temp = 0;
int spd = 0;
double volt = 0;

int demoRPM[4] = { 5, 55, 555, 5555};
int demoTMP[4] = {-55, -5,  5, 55};
int demoSPD[4] = {5,55,555,555};
double demoVOLT[4] = {5,55,55,55};

const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

void setup() {
  Serial.begin(115200);
  delay (100);
  u8g2.begin();
  u8g2.setContrast (70); //LCD contrast
  u8g2.enableUTF8Print();
  draw();
  
  while (!DEMO) {
    Serial.print("Connect to CAN bus ... ");
    CAN.setPins(CS_PIN, INT_PIN);
    CAN.begin(500E3);
    
//    CAN.setClockFrequency(8E6);
    if (!OBD2.begin()) {
      Serial.println("failed!");
      rpm = 0; 
      temp = 0;
      spd = 0;
      volt = 0;
      delay(1000);
    } else {
      Serial.println("success");
      break;
    }
  }
}

void loop() {
  if(!DEMO){
    processPid(PID_ENGINE_RPM);
    processPid(PID_VEHICLE_SPEED);
    processPid(PID_COOLANT_TEMP);
    processPid(PID_CONTROL_MODULE_VOLTAGE);
    draw();
  } else {
    for(int i=0; i <150; i++) {
      rpm = i * 20;
      temp = i - 20;
      spd = i;
      volt = i / 10;
      draw();
      delay(100);
    }
  }
}

void processPid(int pid) {
  if (!OBD2.pidSupported(pid)) {
    // PID not supported, continue to next one ...
    return;
  }

  if (!OBD2.pidValueRaw(pid)) {
    // read the PID value
    float pidValue = OBD2.pidRead(pid);

    switch (pid) {
      case PID_ENGINE_RPM: 
        rpm = pidValue;
        break;
      case PID_COOLANT_TEMP: 
        temp = pidValue;
        break;
      case PID_VEHICLE_SPEED: 
        spd = pidValue;
        break;
      case PID_CONTROL_MODULE_VOLTAGE: 
        volt = pidValue;
        break;        
    }

    if (!isnan(pidValue)) {
      Serial.print(pidValue);
      Serial.print(" ");
      Serial.print(OBD2.pidUnits(pid));
      Serial.println();
    }
  }
  delay(100);
}

void draw()
{
  u8g2.clearBuffer();
        
    // set Volt text position
    u8g2.setFont(u8g2_font_5x7_mf);
    if(volt <= 9.99) {
      u8g2.setCursor(62, 64);
      u8g2.print(volt);
    } else if (volt >= 10.00) {
      u8g2.setCursor(57, 64);
      u8g2.print(volt);      
    }
    u8g2.setCursor(83, 64);
    u8g2.print("V");
    
    // set Big Speed Screen position (km/h)
    u8g2.setFont(u8g2_font_7Segments_26x42_mn);
    if(spd >= 100) {
      u8g2.setCursor(0, 46);       
    } else if(spd >= 10) {
      u8g2.setCursor(32, 46);
    } else {
      u8g2.setCursor(64, 46);
    }
    u8g2.print(spd);
    u8g2.setFont(u8g2_font_unifont_te);
    u8g2.setCursor(96, 46); 
    u8g2.print("km/h");
    u8g2.drawLine(0, 49, 127, 49);
    u8g2.drawLine(0, 0, 127, 0);
    
    // set Water Temperature position (Celsius)
    u8g2.setFont(u8g2_font_helvB10_tf);
    if(temp >= 100) {
      u8g2.setCursor(92, 64); // 555    
    } else if(temp >= 10) {
      u8g2.setCursor(100, 64); //  55
    } else if(temp >= 0) {
      u8g2.setCursor(108, 64); //   5
    } else if(temp <= -10) {
      u8g2.setCursor(96, 64); // -55
    } else {
      u8g2.setCursor(104, 64); //  -5
    }
    u8g2.print(temp);

    // set Coolant Degree Symbol and text 
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setCursor(116, 64);
    u8g2.print(DEGREE_SYMBOL);   
    u8g2.setCursor(122, 64); 
    u8g2.print("C");
    
    // set Coolant Symbol and text positions
    u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
    u8g2.drawGlyph(0, 63, 243); 
    u8g2.setFont(u8g2_font_helvB10_tf);
    if(rpm >= 1000) {
      u8g2.setCursor(10, 64);      
    } else if(rpm >= 100) {
      u8g2.setCursor(18, 64);
    } else if(rpm >= 10) {
      u8g2.setCursor(26, 64);
    } else {
      u8g2.setCursor(34, 64);
    }
    u8g2.print(rpm);    

    // set Battery Symbol Bitmap position
    u8g2.setBitmapMode(true);
    u8g2.drawXBMP(46, 58, 10, 6, battery_icon);
    u8g2.setBitmapMode(false);
    
  u8g2.sendBuffer();
}
