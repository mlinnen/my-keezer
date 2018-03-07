#include "TemperatureLCD.h"

TemperatureController *_tempController;
Adafruit_LiquidCrystal _lcd(0);

TemperatureLCD::TemperatureLCD(TemperatureController *tempController)
{
    _tempController = tempController;
}

void TemperatureLCD::setup()
{
  // set up the LCD's number of rows and columns: 
  _lcd.begin(20, 4);
  // Print a message to the LCD.
  _lcd.println("Keezer Temperature");

}

void TemperatureLCD::loop()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  _lcd.setCursor(0, 1);
  _lcd.print("Set  ");
  _lcd.print(_tempController->setTemperature(),1);
  _lcd.print(" Top ");
  _lcd.println(_tempController->topTemperature(),1);

  _lcd.print("         ");
  _lcd.print(" Avg ");
  _lcd.println(_tempController->averageTemperature(),1);

  _lcd.print("Comp ");
  _lcd.print(" On");
  _lcd.print(" Bot ");
  _lcd.println(_tempController->bottomTemperature(),1);

 
}
