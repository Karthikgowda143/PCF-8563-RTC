// Date and time functions using a PCF8563 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include "Wire.h"

#define SDA_PIN 4 
#define SCL_PIN 5  

RTC_PCF8563 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); 

  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif

 if (! rtc.begin(&Wire)) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  rtc.start();
}

void loop () {
  // Check for serial input
  if (Serial.available() > 0)
  {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim(); // Apply trim here to remove any leading/trailing whitespace
    
    // If user types 'set', prompt them to enter date and time
    if (inputString == "set")
    {
      Serial.println("Choose option:\nA. Set Date\nB. Set Time\nC. Set Date and Time\n");
      while (Serial.available() == 0); 
      String op = Serial.readStringUntil('\n');

      int year, month, day, hour, minute, second;
      if(op == "A")
      {
        Serial.println("Enter new Date in format YYYY MM DD:");
        while (Serial.available() == 0); // Wait for input
        String dateString = Serial.readStringUntil('\n');

        int parsedCount = sscanf(dateString.c_str(), "%d %d %d", &year, &month, &day);

        if (parsedCount == 3)
        {
          DateTime now = rtc.now(); // Get current time
          rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second())); // Adjust date only
          Serial.println("RTC updated successfully.");
        }
      }
      else if(op == "B")
      {
        Serial.println("Enter new Time in format HH MM SS:");
        while (Serial.available() == 0); // Wait for input
        String timeString = Serial.readStringUntil('\n');

        int parsedCount = sscanf(timeString.c_str(), "%d %d %d", &hour, &minute, &second);

        if (parsedCount == 3)
        {
          DateTime now = rtc.now(); // Get current date
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, second)); // Adjust time only
          Serial.println("RTC updated successfully.");
        }
      }
      else if(op == "C")
      {
        Serial.println("Enter new date and time in format YYYY MM DD HH MM SS:");
  
        // Wait for date and time input from the user
        while (Serial.available() == 0); // Wait for input
        String dateTimeString = Serial.readStringUntil('\n');

        int parsedCount = sscanf(dateTimeString.c_str(), "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &second);

        if (parsedCount == 6)
        {
          // Adjust the RTC with the new date and time
          rtc.adjust(DateTime(year, month, day, hour, minute, second));
          Serial.println("RTC updated successfully.");
        }
      }
      else
      {
        Serial.println("Invalid Input");
        
      }
    }
  }  
  

  DateTime now = rtc.now();

  Serial.print("DATE - ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(")\n");

  Serial.print("TIME - ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  Serial.print("\n");

   
  delay(1000);
}
