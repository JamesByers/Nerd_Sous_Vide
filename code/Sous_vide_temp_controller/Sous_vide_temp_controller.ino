
// Include Libraries
#include "Arduino.h"
#include "DS18B20.h"
#include "LiquidCrystal_PCF8574.h"
#include "Wire.h"

// Pin Definitions
#define DS18B20WP_PIN_DQ	5

// There are several different versions of the LCD I2C adapter, each might have a different address.
// Try the given addresses by Un/commenting the following rows until LCD works follow the serial monitor prints. 
// To find your LCD address go to: http://playground.arduino.cc/Main/I2cScanner and run example.
#define LCD_ADDRESS 0x27
//#define LCD_ADDRESS 0x3F // alternate address some LCD controllers use

// Define LCD characteristics
#define LCD_ROWS 4
#define LCD_COLUMNS 20
#define SCROLL_DELAY 150
#define BACKLIGHT 255

// object initialization
DS18B20 ds18b20wp(DS18B20WP_PIN_DQ);
LiquidCrystal_PCF8574 lcd20x4;

// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

const int refresh_interval = 2000;
const long target_temp = 80.00;
boolean heating = 0;


float ds18b20wpTempF = 0.0;
const int relay_pin = 6;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    pinMode(6, OUTPUT);    // sets the digital pin 6 as output
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println(F("start")); 
   
    // initialize the lcd
    lcd20x4.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT);
    lcd20x4.clear();
    lcd20x4.selectLine(1); // Set cursor at the begining of line 1  
    lcd20x4.print(F("Target: "));
    lcd20x4.print(target_temp);
    //take initial reading of the temperature
    ds18b20wpTempF = ds18b20wp.readTempF();
}

// Main logic of the circuit. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    Serial.begin(9600);
 
    // Read DS18B20 temp sensor value in degrees Fahrenheit. For degrees Celsius use ds18b20wp.ReadTempC()
    ds18b20wpTempF = ds18b20wp.readTempF();
    Serial.print(F("Temp is: ")); Serial.print(ds18b20wpTempF); Serial.println(F(" F"));
    Serial.print(F("Target temp is: ")); Serial.print(target_temp); Serial.println(F(" F"));

    if (ds18b20wpTempF <= -196.0) {  // Sensor library reports a reading of -196.60 (-127C) if it cannot read the sensor
      lcd20x4.selectLine(2); 
      lcd20x4.print(F(" Sensor disconnected"));
      lcd20x4.selectLine(3); 
      lcd20x4.print(F("   or malfuntioning"));
      lcd20x4.selectLine(4); 
      lcd20x4.print(F("                    "));        
    }    
      else if (ds18b20wpTempF < -67 || ds18b20wpTempF > 257 || ds18b20wpTempF >= target_temp -.3) {  
      heating = 0;
      Serial.println(F("Coasting"));
      digitalWrite(relay_pin, LOW);      
      lcd20x4.selectLine(2);
      lcd20x4.print(F("Actual: "));
      //lcd20x4.setCursor(8, 1);
      lcd20x4.print(ds18b20wpTempF);
      lcd20x4.print(F(" F     "));
      lcd20x4.selectLine(4);
      lcd20x4.print(F("Coasting         "));
      lcd20x4.selectLine(3);
      lcd20x4.print(F("                    ")); 
    }  
    
    else if (ds18b20wpTempF - target_temp < .2) { 
      heating = 1;
      Serial.println(F("Heat on"));
      digitalWrite(relay_pin, HIGH);
      lcd20x4.selectLine(2);          // Set cursor at the begining of line 2
      lcd20x4.print(F("Actual: "));  
      //lcd20x4.setCursor(8, 1);
      lcd20x4.print(ds18b20wpTempF);      // Print value to LCD on 1st line
      lcd20x4.print(F(" F     "));
      lcd20x4.selectLine(4); 
      lcd20x4.print(F("Heat on         "));
      lcd20x4.selectLine(3);
      lcd20x4.print(F("                    "));     
    }
/*    
    if (heating == 1) { 
      Serial.println(F("Heat on"));
      digitalWrite(relay_pin, HIGH);
      lcd20x4.selectLine(2);          // Set cursor at the begining of line 2
      lcd20x4.print(F("Actual: "));  
      //lcd20x4.setCursor(8, 1);
      lcd20x4.print(ds18b20wpTempF);      // Print value to LCD on 1st line
      lcd20x4.print(F(" F     "));
      lcd20x4.selectLine(4); 
      lcd20x4.print(F("Heat on         "));
      lcd20x4.selectLine(3);
      lcd20x4.print(F("                    ")); 
      
    }
    else if (heating == 0) { 
      Serial.println(F("Coasting"));
      digitalWrite(relay_pin, LOW);      
      lcd20x4.selectLine(2);
      lcd20x4.print(F("Actual: "));
      //lcd20x4.setCursor(8, 1);
      lcd20x4.print(ds18b20wpTempF);
      lcd20x4.print(F(" F     "));
      lcd20x4.selectLine(4);
      lcd20x4.print(F("Coasting         "));
      lcd20x4.selectLine(3);
      lcd20x4.print(F("                    ")); 
    }
    */      
    delay(refresh_interval);
}    

/***************************************************************************
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
***************************************************************************/
