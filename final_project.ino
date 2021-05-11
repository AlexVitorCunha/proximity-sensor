//Alex Vitor Marques Moreira da Cunha
// 09/04/2021
/*A parking sensor that includes sound, LCD message and light
sensor*/
// include the library codes:
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define PIN_BUTTON 0
#define PIN_READWRITE 6
#define PIN_CONTRAST 12
#define LED_count 4
#define NOTE_AS5 932
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 4, 3, 2, A4, A5);
int cm; //proximity in cm
int buzzerPin = 9 ; //The buzzerPin is connected to pin 7 of the Arduino.
int lEDPin = 12; // LED is connected to pin 12 
int LEDCount = 4; // number of LEDs
bool proximity = false; //defines if sensor if on or off
int button1Pin = 8; //The SW1 button is connect to pin 8 of the Arduino.
Adafruit_NeoPixel strip(LEDCount, lEDPin, NEO_GRB + NEO_KHZ800);


void setup() {
  pinMode(buzzerPin, OUTPUT); //Setup red LED pin as an output pin.  
  pinMode(button1Pin, INPUT); //Setup button1 pin as an input pin.
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  strip.begin();           // INITIALIZE NeoPixel strip
  strip.show();            // Turn OFF all pixels ASAP
}

void loop() {
  if(digitalRead(button1Pin) == HIGH){ // turns sensor on and off depending on button press
    proximity = !proximity;
  }
  
  if(!proximity){ //asks user to press button to turn on sensor
  	lcd.setCursor(0,0);
  	lcd.print("Press button to");
    lcd.setCursor(0,1);
    lcd.print("activate sensor");
  }
  else{
  // measure the ping time in cm
  cm = 0.01723 * readUltrasonicDistance(13, 13);
  controlLightSensor(cm);
  controlSoundSensor(cm);
  controlLCDMessage(cm);
  }
}

//determines the proximity
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

//Changes LED depending on proximity
void controlLightSensor(long distance){
  int brightness;
  int red;
  int green;
  int blue = 0;
  
  if (distance > 200){ // if distance is over 200 cm no LED light will show
    brightness = 0;
    red = 0;
    green = 0;
  }
  else if(distance >= 100){ //green light for distance over than 100 cm
    brightness = map(distance, 100, 200, 255, 50);
    red = 0;
    green = 255;
  }
  else if(distance >= 50){ //yellow light for distance lower than 100 cm
    brightness = map(distance, 50, 100, 255, 50);
    red = 255;
    green = 255;
  }
  else{ //red light for distance lower than 50 cm
    brightness = map(distance, 0, 50, 255, 50);
    red = 255;
    green = 0;
  }
  for(int i = 0; i < 4; i++){
    strip.setPixelColor(i,red, green, blue);
  }
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(brightness);
}

//Sound alarm depending on proximity
void controlSoundSensor(long distance){ 
  int duration = 500;
  if(distance < 200){
    tone(buzzerPin, NOTE_AS5, duration);
    delay(duration + distance);
  } 
}

//shows different messages depending on the proximity
void controlLCDMessage(long distance){
  String message = "";
  lcd.clear();
  if(distance >= 100 && distance < 200){
    message = "Too Far";
  }
  else if(distance >= 50 && distance < 100){
    message = "Getting close";
  }
  else if(distance < 50){
    message = "Too close";
  }
  lcd.setCursor(0,0);
  lcd.print(message);
  delay(1000);
}