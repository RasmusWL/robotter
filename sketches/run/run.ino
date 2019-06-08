#include <Pololu3pi.h>  // gives access to sensor interface functions
#include <PololuQTRSensors.h>  // used by Pololu3pi.h

#include <OrangutanLEDs.h>
#include <OrangutanAnalog.h>
#include <OrangutanMotors.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>

/*
 * OrangutanMotorExample for the 3pi robot, Orangutan LV-168, Orangutan SV-xx8,
 *   and Baby Orangutan B
 *
 * This example uses the OrangutanMotors library to drive
 * motors in response to the position of user trimmer potentiometer
 * and blinks the red user LED at a rate determined by the trimmer
 * potentiometer position.  It uses the OrangutanAnalog library to measure
 * the trimpot position, and it uses the OrangutanLEDs library to provide
 * limited feedback with the red user LED.
 *
 * http://www.pololu.com/docs/0J17/5.e
 * http://www.pololu.com
 * http://forum.pololu.com
 */

OrangutanAnalog analog;
OrangutanLEDs leds;
OrangutanMotors motors;

OrangutanLCD lcd;
OrangutanPushbuttons buttons;
OrangutanBuzzer buzzer;

Pololu3pi robot;
unsigned int sensors[5]; // an array to hold sensor values

int leftSpeed;
int rightSpeed;

bool isLeft;
bool started;

void setup()               // run once, when the sketch starts
{
  robot.init(2000);

  leftSpeed = 243;
  rightSpeed = 255;

  isLeft = true;
  started = false;

  motors.setSpeeds(0, 0);

  // Calibrate IR sensors
  lcd.clear();
  lcd.print("Press B");
  lcd.gotoXY(0, 1);
  lcd.print("To Calibrate");
  wait_for_button_press(BUTTON_B);
  wait_for_button_release(BUTTON_B);

  for(int i=0; i<100; i++) {
    lcd.clear();
    lcd.print(i);
    robot.calibrateLineSensors(IR_EMITTERS_ON);

    delay(20);
  }

  // Ready to go

  lcd.clear();
  lcd.print("Press B");
  lcd.gotoXY(0, 1);
  lcd.print("To Start");
  
  wait_for_button_press(BUTTON_B);
  wait_for_button_release(BUTTON_B);
  ready_engines()
}

void ready_engines() 
{
  buzzer.playNote(NOTE_E(5), 200, 15);
  delay(800);
  buzzer.playNote(NOTE_E(5), 200, 15);
  delay(800);
  buzzer.playNote(NOTE_E(5), 200, 15);
  delay(800);
  buzzer.playNote(NOTE_E(6), 400, 15);

  motors.setSpeeds(leftSpeed, rightSpeed); 
}

void loop()                // run over and over again
{
  robot.readLine(sensors, IR_EMITTERS_ON);
  if (sensors[2] < 300) 
  {
    motors.setSpeeds(0,0);
    // victory
  }

  delay(20);
}

void change_speeds()
{
  lcd.clear();
  lcd.gotoXY(0, 0);
  lcd.print("L");
  lcd.gotoXY(2, 0);
  lcd.print(leftSpeed);
    
  lcd.gotoXY(0, 1);
  lcd.print("R");
  lcd.gotoXY(2, 1);
  lcd.print(rightSpeed);

  if (isLeft) {
    lcd.gotoXY(5,0);
    lcd.print("*");
  } else {
    lcd.gotoXY(5,1);
    lcd.print("*");
  }

  unsigned char button = get_single_debounced_button_press(ALL_BUTTONS);

  if (button & BUTTON_A) {
    if (isLeft) {
      leftSpeed -= 2;
    } else {
      rightSpeed -= 2;
    }
  }
  if (button & BUTTON_B) {
    isLeft = !isLeft;
  }
  if (button & BUTTON_C) {
    if (isLeft) {
      leftSpeed += 2;
    } else {
      rightSpeed += 2;
    }
  }
}
