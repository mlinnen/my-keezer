#include "TemperatureLCD.h"

int _mode;
int _lastMode;

Adafruit_LiquidCrystal _lcd(0);

void temperaturelcd_printText(int mode)
{
  switch (mode) {
    case 1:
      _lcd.setCursor(0, 0);
      _lcd.print("Current Temperature ");
      _lcd.setCursor(0, 1);
      _lcd.print("Set      Top        ");
      _lcd.setCursor(0, 2);
      _lcd.print("         Avg        ");
      _lcd.setCursor(0, 3);
      _lcd.print("Comp     Bot        ");
      break;
    case 2:
      _lcd.print("Set Temperature     ");
      _lcd.print("High                ");
      _lcd.print("Avg                 ");
      _lcd.print("Low                 ");
      break;
    case 3:
      _lcd.print("Daily Temperature   ");
      _lcd.print("Top Hi      Low     ");
      _lcd.print("Avg Hi      Low     ");
      _lcd.print("Bot Hi      Low     ");
      break;
  }

}

void temperaturelcd_printVariables(int mode)
{
  switch (mode) {
    case 1:
      _lcd.setCursor(4, 1);
      _lcd.print(temperaturecontroller_averageSetPointTemperature(),1);
      _lcd.setCursor(13, 1);
      _lcd.print(temperaturesensor_topTemperature(),1);
      _lcd.setCursor(13, 2);
      _lcd.print(temperaturesensor_averageTemperature(),1);
      _lcd.setCursor(5, 3);
      if (temperaturecontroller_compressor()) {_lcd.print("On ");}
      else {_lcd.print("Off");}
      _lcd.setCursor(13, 3);
      _lcd.print(temperaturesensor_bottomTemperature(),1);
      break;
    case 2:
      _lcd.setCursor(5, 1);
      _lcd.print(temperaturecontroller_highSetPointTemperature(),1);
      _lcd.setCursor(5, 2);
      _lcd.print(temperaturecontroller_averageSetPointTemperature(),1);
      _lcd.setCursor(5, 3);
      _lcd.print(temperaturecontroller_lowSetPointTemperature(),1);
      break;
  }

}

void temperaturelcd_setup()
{
  _mode = 1;

  // set up the LCD's number of rows and columns: 
  _lcd.begin(20, 4);
  // Print a message to the LCD.
  _lcd.print("Keezer Temperature  ");

}

void temperaturelcd_changeMode()
{
  _mode = _mode + 1;
  if (_mode>3) {_mode = 1;}
  Serial.print("Mode: ");
  Serial.println(_mode);
}

void temperaturelcd_print()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  if (_lastMode!=_mode) {
    temperaturelcd_printText(_mode);
  }
  temperaturelcd_printVariables(_mode);
  _lastMode = _mode;
}
