#include<Servo.h>
#include<Wire.h>

#define I2C_ADDRESS 0x3C
#define RST_PIN -1

int magnetStat = 0;
int lowbyt; 
word highbyt;
int rawAngle; 
float degAngle;
float correctedAngle = 0;
float startAngle = 0; 
float totalAngle = 0;

void setup() 
{
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(800000L);
    checkMagnet();
    ReadAngle();
    degAngle = startAngle;
    
}

void loop() 
{
 ReadAngle();
 correctAngle();
 Serial.println(rawAngle);
}

void ReadAngle()
{
  Wire.beginTransmission(0x36);
  Wire.write(0x0D);
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);

  while(Wire.available() == 0);
  lowbyt = Wire.read();

  Wire.beginTransmission(0x36);
  Wire.write(0x0C);
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);

  while(Wire.available() == 0);  
  highbyt = Wire.read();

  highbyt = highbyt << 8;
  rawAngle = highbyt | lowbyt;
  degAngle = rawAngle * 0.087890625;
}

void correctAngle()
{
  correctedAngle = degAngle - startAngle;
  
  if(correctedAngle < 0)
  {
    correctedAngle = correctedAngle + 360;
  }

  else{ }
  
}

void checkMagnet()
{
  while((magnetStat & 32) != 32)
  {
    magnetStat = 0;
    Wire.beginTransmission(0x36);
    Wire.write(0x0B);
    Wire.endTransmission();
    Wire.requestFrom(0x36, 1);

    while(Wire.available() == 0);
    magnetStat = Wire.read();

    Serial.print("Current Magnet Status:");
    Serial.println(magnetStat, BIN);
    delay(200);
  }

  Serial.println("Magnet Found (0 0)");
  Serial.println("              (-)");
  delay(2000);
}
