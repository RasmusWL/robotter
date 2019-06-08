#include <OrangutanLEDs.h>
#include <OrangutanAnalog.h>
#include <OrangutanMotors.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>

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

int leftSpeed;
int rightSpeed;

bool isLeft;

void setup()               // run once, when the sketch starts
{
  leftSpeed = 243;
  rightSpeed = 255;

  isLeft = true;
}

void loop()                // run over and over again
{
  motors.setSpeeds(leftSpeed, rightSpeed);
  delay(10);

  char tmp_str[3];

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
