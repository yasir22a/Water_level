#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x26, 16, 2);

int buzz = 13;
int trig = 11;
int echo = 10;
int rled = 9;
int yled = 8;
int gled = 7;
int motor = 2;
int button = 4;
int E_led = 3;

// veriables global 

long time;
float distance;
bool redBeeped = false; // limited numbers of beep sound
bool emergencyStopActive = false; // for emergency E-Stop

void ultrasound_fun() {
  // Don't run sensor if system is halted
  if (emergencyStopActive) return; 

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  time = pulseIn(echo, HIGH);
  distance = time * 0.0343 / 2;
}

void light_fun() {
  // Check for E-Stop button press
  int b = digitalRead(button);
  
  if (b == LOW) {
    emergencyStopActive = true; // LATCH the emergency state ON
  }
  
  
  if (emergencyStopActive) { 
    // EMERGENCY STATE
    // This code runs *after* the button is pressed and latched
    
    lcd.print("EMERGENCY STOP ");
    digitalWrite(E_led, HIGH);
    
    // STOP ALL FUNCTIONS
    digitalWrite(motor, LOW);
    digitalWrite(rled, LOW);
    digitalWrite(yled, LOW);
    digitalWrite(gled, LOW);
    
    //EMERGENCY situation sound
    if (!redBeeped) { // We re-use 'redBeeped' to mean "alarm has sounded"
      for (int i = 0; i < 5; i++) {
        tone(buzz, 1000);
        delay(600);
        noTone(buzz);
        delay(600);
      }
      redBeeped = true; // Set flag so it doesn't just keep beeping
    }
  }
  
  // no emergency, run normal logic
  else { 
    digitalWrite(E_led, LOW); // Turn off E-Stop LED

    // LOW → RED LED → Motor ON
    if (distance >= 200) { 
      lcd.print("LOW    ");
      digitalWrite(rled, HIGH);
      digitalWrite(yled, LOW);
      digitalWrite(gled, LOW);
      digitalWrite(motor, HIGH);

      if (!redBeeped) {
        for (int i = 0; i < 3; i++) {
          tone(buzz, 800);
          delay(500);
          noTone(buzz);
          delay(500);
        }
        redBeeped = true;
      }
    }
    // MEDIUM → YELLOW LED → Motor ON
    else if (distance >= 80 && distance < 200) { 
      lcd.print("MEDIUM ");
      digitalWrite(rled, LOW);
      digitalWrite(yled, HIGH);
      digitalWrite(gled, LOW);
      digitalWrite(motor, HIGH); 
      redBeeped = false;
    }
    // FULL → GREEN LED → Motor OFF
    else { 
      lcd.print("FULL   ");
      digitalWrite(rled, LOW);
      digitalWrite(yled, LOW);
      digitalWrite(gled, HIGH);
      digitalWrite(motor, LOW);
      redBeeped = false;
    }
  }
}

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(rled, OUTPUT);
  pinMode(yled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(motor, OUTPUT);
  
  pinMode(button, INPUT_PULLUP); // Use INPUT_PULLUP
  pinMode(E_led, OUTPUT);
}

void loop() {
  // We only run the sensor if E-Stop is NOT active
  if (!emergencyStopActive) {
    ultrasound_fun();
  }
  
  lcd.setCursor(0, 0);
  
  // Only update water level if not in E-Stop
  if (!emergencyStopActive) {
    lcd.print("Water Level:    ");
  }

  lcd.setCursor(0, 1);
  light_fun();
  
  delay(100); //for stability of system
}


//Restart system after "EMERGENCY" 
