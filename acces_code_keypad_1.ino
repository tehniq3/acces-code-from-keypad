// sketch acces bloc cu cod din exterior si buton temperizat din interior
// sketch for home acces with keypad code from outside & pushbutton fron inside
// versiune 1.0 - 31.08.2016, Craiova, Romania
// autor: Nicu FLORICA (niq_ro) - http://www.tehnic.go.ro
// http://nicuflorica.blogspot.ro/
// http://arduinotehniq.blogspot.com/
// http://www.arduinotehniq.com/

#include <Keypad.h>                // library for keyboard
#include <Password.h>              // library for password
Password password = Password( "1234" );  // password

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
// tastatura plata
//byte rowPins[rows] = {1, 2, 3, 4};
//byte colPins[cols] = {5, 6, 7};
// tastatura cu butoane in relief
byte rowPins[rows] = {2, 7, 6, 4};
byte colPins[cols] = {3, 1, 5};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);



#define buton 12    // buton pus la D12
#define sens1 10
#define sens2 9
#define eroare 8
#define elmagnet 11
#define scurt 500   // timp in ms pentru eliminare 
#define tpactuator 700  // timp actionare actuatoare
#define tpelmagnet 5000 // timp actionare electromagnet

void setup()
{
keypad.addEventListener(keypadEvent); // se creaza un obiect pentru urmarire apasare taste  
  
 pinMode(buton, INPUT);
 pinMode(sens1, OUTPUT);
 pinMode(sens2, OUTPUT);
 pinMode(eroare, OUTPUT);
 pinMode(elmagnet, OUTPUT);


 digitalWrite(buton, HIGH); // intrarea butonului in 1 logic
 digitalWrite(sens1, LOW); 
 digitalWrite(sens2, LOW);
 digitalWrite(eroare, LOW);
 digitalWrite(elmagnet, LOW);
}

void loop() {
  
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
    digitalWrite(eroare, HIGH);
    delay(1000);
    digitalWrite(eroare, LOW);
  }
}

void actiune()
{
digitalWrite(elmagnet, HIGH);
digitalWrite(sens1, HIGH);
delay(tpactuator);
digitalWrite(sens1, LOW);
delay(tpelmagnet);
digitalWrite(elmagnet, LOW);
digitalWrite(sens2, HIGH);
delay(tpactuator);
digitalWrite(sens2, LOW);
}  
