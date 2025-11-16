Automatic Water Level ControllerThis Arduino project automatically controls a water pump based on a tank's water level, measured by an ultrasonic sensor. It features visual alerts and a latching emergency stop.
Key FeaturesAutomatic Control: 
Activates a pump (via an active-HIGH relay) when the water is LOW or MEDIUM and stops it when FULL.
Visual/Audible Alerts: Uses LEDs (Red/Yellow/Green) and a buzzer for different water levels.
Emergency Stop: A pushbutton instantly halts all operations, displays an alert, and beeps.
The system must be reset to continue.
Hardware
Arduino (Uno/Nano), HC-SR04 Ultrasonic Sensor16x2, I2C LCD (Address 0x26), 4x LEDs (R, Y, G, E-Stop), Pushbutton & Buzzer, Jumper Wires
Core Connections see block or Schematics diagrams 
Logic   
LOW (>= 200 cm): Motor ON, Red LED ON, Beeps 3x.
MEDIUM (>= 80 cm): Motor ON, Yellow LED ON.
FULL (< 80 cm): Motor OFF, Green LED ON.
E-STOP (Button Press): Motor OFF, E-Stop LED ON, Beeps 5x. System halts until reset.
Library 
LiquidCrystal_I2C.h
