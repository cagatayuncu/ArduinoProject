
#include <OneWire.h> // OneWire kütüphanesini ekliyoruz.
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

const int buttonPin = 21;
const int ledPin = 20;
int dugmeDurumu = 0;     
int lcd_key     = 0;
int adc_key_in  = 0;
int lm35Pin = 35;
int DS18S20_Pin = 35; 
OneWire ds(DS18S20_Pin); 
void setup(void) {
  
  Serial.begin(9600);
 
   lcd.begin(16, 2);
   pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);  
}

void loop(void) {
  float temperature = getTemp();
  lcd.setCursor(0, 0); 
  lcd.print("SICAKLIK:");
  lcd.print(temperature);
  
  lcd.print(" C");
  lcd.setCursor(0, 1); 
  lcd.print(" SEVIYE:");
  delay(300);

 dugmeDurumu = digitalRead(buttonPin);
  if (dugmeDurumu == HIGH) {
    digitalWrite(ledPin, HIGH);
     lcd.setCursor(8, 1); 
    lcd.print("LiMiT!!");
    Serial.print(temperature);
  Serial.print(" ");
    Serial.println("SU SEVİYESİ: LİMİT AŞIMI");
     
  } 

  if (dugmeDurumu == LOW) {
    digitalWrite(ledPin, LOW);
    lcd.setCursor(13,1);
    lcd.print("  ");
     lcd.setCursor(8, 1);
    lcd.print("UYGUN");
    Serial.print(temperature);
    Serial.print(" ");
    Serial.println("SU SEVİYESİ: UYGUN");
    

  }

    while (dugmeDurumu == HIGH){  /* düğmeye basili olduğu surece bekle */
 dugmeDurumu = digitalRead(buttonPin);
 loop();
 //if(dugmeDurumu == LOW) {
 //getTemp();
 //}
  
} 

}


float getTemp(){
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      lcd.setCursor(0, 0);
      lcd.println("CRC Gecerli degil");
     
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      lcd.setCursor(0, 0);
      lcd.print("Cihaz Taninmadi");
      
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); 

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); 
  for (int i = 0; i < 9; i++) { 
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;

}
