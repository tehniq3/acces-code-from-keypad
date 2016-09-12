// sketch acces bloc cu cod din exterior si buton temperizat din interior
// versiune 1.0 - 31.08.2016, Craiova, Romania
// autor: Nicu FLORICA (niq_ro) - http://www.tehnic.go.ro
// http://nicuflorica.blogspot.ro/
// http://arduinotehniq.blogspot.com/
// http://www.arduinotehniq.com/

#include <Wire.h>                // folosire librarie pentru protocol i2c
#include <LiquidCrystal_I2C.h>   // folosire librarie pentru afisaj lcd1602 pe i2c
#include "RTClib.h"
#include <Keypad.h>                // library for keyboard http://playground.arduino.cc/uploads/Code/keypad.zip
#include <Password.h>              // library for password http://playground.arduino.cc/uploads/Code/Password.zip
Password password = Password( "1234" );  // password

// afisaj 1602 pe i2c cu adresa 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
// modul RTC cu DS1307 sau DS3231
RTC_DS1307 RTC;

const byte rows = 4;                     // four rows       
const byte cols = 3;                     // three columns
char keys[rows][cols] = {                // keys on keypad
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
// byte rowPins[rows] = {R0, R1, R2, R3};
//byte colPins[cols] = {C0, C1, C2};
// tastatura plata (flat) - Adafruit type
//byte rowPins[rows] = {1, 2, 3, 4};
//byte colPins[cols] = {5, 6, 7};
// tastatura cu butoane in relief - model KB304-PAW
byte rowPins[rows] = {2, 7, 6, 4};
byte colPins[cols] = {3, 1, 5};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);


#define buton 12    // buton pus la D12
#define sens1 10
#define sens2 9
#define asteptare 8
#define elmagnet 11
#define scurt 500   // timp in ms pentru eliminare 
#define tpactuator 700  // timp actionare actuatoare
#define tpelmagnet 5000 // timp actionare electromagnet


// http://arduino.cc/en/Reference/LiquidCrystalCreateChar
byte lacat[8] = {
  B01110,
  B10001,
  B00001,
  B00001,
  B11111,
  B11111,
  B11111,
};

byte cheie[8] = {
  B00110,
  B01001,
  B00110,
  B00010,
  B01110,
  B00010,
  B01110,
};



void setup()
{
keypad.addEventListener(keypadEvent); // se creaza un obiect pentru urmarire apasare taste  
  
 pinMode(buton, INPUT);
 pinMode(sens1, OUTPUT);
 pinMode(sens2, OUTPUT);
 pinMode(asteptare, OUTPUT);
 pinMode(elmagnet, OUTPUT);


 digitalWrite(buton, HIGH); // intrarea butonului in 1 logic
 digitalWrite(sens1, LOW); 
 digitalWrite(sens2, LOW);
 digitalWrite(asteptare, HIGH);
 digitalWrite(elmagnet, LOW);

lcd.begin();  // initializare afisaj
lcd.backlight(); // aprindere fundal
lcd.createChar(0, lacat); // creez caracter
lcd.createChar(1, cheie); // creez caracter
//lcd.write(byte(0)); // comanda pentru caracter
  
lcd.setCursor(0, 0);   // pozitionare cursor pe colana a 1-a din stanga - sus
lcd.print("Sistem acces cod"); // afisare text
lcd.setCursor(0, 1);   // mutare cursor pe coloana a 1-a din stanga - jos
lcd.print("ver.4 by niq_ro"); // afisare text

delay(3000);  
lcd.clear();

lcd.setCursor(15, 1);   // pozitionare cursor pe colana a 1-a din stanga - sus
//lcd.print("Usa incuiata.."); // afisare text
//lcd.write(byte(0));
lcd.write(byte(1));

  Wire.begin();
  
// part code from http://tronixstuff.wordpress.com/
Wire.beginTransmission(0x68);
Wire.write(0x07); // move pointer to SQW address
Wire.write(0x10); // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave
Wire.endTransmission();
// end part code from http://tronixstuff.wordpress.com/

    RTC.begin();
// if you need set clock... just remove // from line above this
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));  
    }
// RTC.adjust(DateTime(__DATE__, __TIME__));
//RTC.adjust(DateTime(2016, 9, 11, 13, 24, 0)); 
}

void loop() {

DateTime now = RTC.now();  
   lcd.setCursor(1, 0);
   if ( now.hour() < 10)
   {
     lcd.print(" "); 
     lcd.print(now.hour(), DEC);
   }
   else
   {
   lcd.print(now.hour(), DEC);
   }
   lcd.print(":");
   if ( now.minute() < 10)
   {
     lcd.print("0"); 
     lcd.print(now.minute(), DEC);
   }
   else
   {
   lcd.print(now.minute(), DEC);
   }
   lcd.print(":");
   if ( now.second() < 10)
   {
     lcd.print("0"); 
     lcd.print(now.second(), DEC);
   }
   else
   {
   lcd.print(now.second(), DEC);
   }
   lcd.print(" "); 

  lcd.setCursor(0, 1);
   if ( now.day() < 10)
   {
     lcd.print("0"); 
     lcd.print(now.day(), DEC);
   }
   else
   {
   lcd.print(now.day(), DEC);
   }
   lcd.print("/");
   if ( now.month() < 10)
   {
     lcd.print("0"); 
     lcd.print(now.month(), DEC);
   }
   else
   {
   lcd.print(now.month(), DEC);
   }
   lcd.print("/");
   lcd.print(now.year(), DEC);
   lcd.print(""); 



keypad.getKey();
 
if (digitalRead(buton) == LOW)
{    // se apasa butonul
actiune();
}

} // se termina programul


//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
//	Serial.print("Pressed: ");
//	Serial.println(eKey);
	switch (eKey){
	  case '#': checkPassword(); break;
	  case '*': password.reset(); break;
	  default: password.append(eKey);
     }
  }
}

  void checkPassword(){
  if (password.evaluate()){
//    Serial.println("Success");
//  sistem++;
  password.reset();
    //Add code to run if it works
    actiune();
  }else{
//    Serial.println("Wrong");
    //add code to run if it did not work
    password.reset();
//    digitalWrite(eroare, HIGH);
    delay(1000);
//    digitalWrite(eroare, LOW);
  }
}

void actiune()
{
lcd.clear();  
lcd.setCursor(1, 0);   // pozitionare cursor pe coloana a 1-a din stanga - sus
lcd.print("Zavor deschis"); // afisare text  
lcd.setCursor(15, 0);   // pozitionare cursor pe coloana a 15-a din stanga - sus
lcd.write(byte(0));
//lcd.write(byte(1));
digitalWrite(elmagnet, HIGH);
digitalWrite(asteptare, LOW);
digitalWrite(sens1, HIGH);
delay(tpactuator);
digitalWrite(sens1, LOW);
delay(tpelmagnet);
lcd.clear();  
lcd.setCursor(1, 0);   // pozitionare cursor pe colana a 1-a din stanga - sus
lcd.print("Zavor inchis"); // afisare text  
digitalWrite(elmagnet, LOW);
digitalWrite(sens2, HIGH);
delay(tpactuator);
digitalWrite(sens2, LOW);
digitalWrite(asteptare, HIGH);
delay(tpactuator);
lcd.clear();
lcd.setCursor(15, 1);   // pozitionare cursor pe colana a 1-a din stanga - jos
//lcd.write(byte(0));
lcd.write(byte(1));
}  
