#include "keezerLCD.h"

int _lastMode = -1;

Adafruit_LiquidCrystal _lcd(0);

void keezerlcd_printText(int mode)
{
  switch (mode) {
    case 0:
      _lcd.setCursor(0, 0);
      _lcd.print("Wifi/MQTT Setup ");
      _lcd.setCursor(0, 1);
      _lcd.print("Connect to:");
      _lcd.setCursor(0, 2);
      _lcd.print(WIFI_AP_NAME);
      break;
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

void keezerlcd_printVariables(int mode)
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

void keezerlcd_setup()
{
  // set up the LCD's number of rows and columns: 
  _lcd.begin(20, 4);
  // Print a message to the LCD.
  _lcd.print("My Keezer  ");
}

void keezerlcd_print()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  if (_lastMode!=temperaturecontroller_mode()) {
    keezerlcd_printText(temperaturecontroller_mode());
  }
  keezerlcd_printVariables(temperaturecontroller_mode());
  _lastMode = temperaturecontroller_mode();
}

void keezerlcd_on() {
  _lcd.display();
  _lcd.setBacklight(HIGH);
}

void keezerlcd_off() {
  _lcd.noDisplay();
  _lcd.setBacklight(LOW);
}