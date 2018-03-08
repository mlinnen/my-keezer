#include "TemperatureLCD.h"

TemperatureController *_tempController;
Adafruit_LiquidCrystal _lcd(0);

TemperatureLCD::TemperatureLCD(TemperatureController *tempController)
{
    _tempController = tempController;
    _mode = 1;
}

void printText(int mode)
{
  _lcd.setCursor(0, 1);
  switch (mode) {
    case 1:
      _lcd.println("Current Temperature");
      _lcd.println("Set      Top");
      _lcd.println("         Avg");
      _lcd.println("Comp     Bot");
      break;
    case 2:
      _lcd.println("Set Temperature");
      _lcd.println("High");
      _lcd.println("Avg");
      _lcd.println("Low");
      break;
    case 3:
      _lcd.println("Daily Temperature");
      _lcd.println("Top Hi      Low");
      _lcd.println("Avg Hi      Low");
      _lcd.println("Bot Hi      Low");
      break;
  }

}

void printVariables(int mode)
{
  switch (mode) {
    case 1:
      _lcd.setCursor(4, 1);
      _lcd.print(_tempController->averageSetPointTemperature(),1);
      _lcd.setCursor(13, 1);
      _lcd.print(_tempController->topTemperature(),1);
      _lcd.setCursor(13, 2);
      _lcd.print(_tempController->averageTemperature(),1);
      _lcd.setCursor(4, 3);
      if (_tempController->compressor()) {_lcd.print("On");}
      else {_lcd.print("Off");}
      _lcd.setCursor(13, 3);
      _lcd.print(_tempController->bottomTemperature(),1);
      break;
    case 2:
      _lcd.setCursor(5, 1);
      _lcd.print(_tempController->highSetPointTemperature(),1);
      _lcd.setCursor(5, 2);
      _lcd.print(_tempController->averageSetPointTemperature(),1);
      _lcd.setCursor(5, 3);
      _lcd.print(_tempController->lowSetPointTemperature(),1);
      break;
  }

}

void TemperatureLCD::setup()
{
  // set up the LCD's number of rows and columns: 
  _lcd.begin(20, 4);
  // Print a message to the LCD.
  _lcd.println("Keezer Temperature");

}

void TemperatureLCD::changeMode()
{
  _mode = _mode + 1;
  if (_mode>3) {_mode = 1;}
  Serial.print("Mode: ");
  Serial.println(_mode);
}

void TemperatureLCD::print()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  if (_lastMode!=_mode) {
    printText(_mode);
  }
  printVariables(_mode);
  _lastMode = _mode;
}
