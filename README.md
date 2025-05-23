# Arduino Library for the PT7C4339 RTC IC

A library for interfacing with the PT7C4339 RTC chip over I2C in the Arduino framework. Documentation can be found [here](https://depben.github.io/PT7C4339-RTC/).

## Features

- **Initialization and Communication**
  - `begin()`: Initializes the I2C bus, ensures the device is in 24-hour mode, and checks for stop flag.
  
- **Time and Date Handling**
  - `getTime()`, `setTime()`: Retrieve or set the current time (hours, minutes, seconds).
  - `getDate()`, `setDate()`: Retrieve or set the current date (year, month, day, weekday).
  - Individual getters/setters for each time/date component: `getHour()`, `setHour()`, etc.
  - Automatic weekday calculation on every call of a date setter.

- **Alarm and Output Control**
  - Output mode selection: `getIntOrSqwFlag()`, `setIntOrSqwFlag()`.
  - Square wave output configuration: `getSqwFrequency()`, `setSqwFrequency()`.
  - Alarm functionality not yet implemented.
  
- **Oscillator and Power Management**
  - Enable/disable oscillator: `isOscillatorEnabled()`, `enableOscillator()`.
  - RTC stop flag handling: `getRtcStopFlag()`, `clearRtcStopFlag()`.
  - Battery-backed interrupt setting: `isIntFromBatteryEnabled()`, `enableIntFromBattery()`.

- **Trickle Charger Configuration**
  - Query and configure the trickle charger: `getTrickleChargerEnabled()`, `getTrickleChargerDiode()`, `getTrickleChargerResistor()`, `setTrickleChargerConfig()`.

- **Device Reset**
  - `reset()`: Restores all registers to their default power-on values.

## Limitations
This library uses 24-hour format for time representation and works from 1900/1/1 to 2099/12/31.

## To Do
- Implement alarm functionality

## Installation

1. Download the library with the Arduino Library Manager, from the [PlatformIO Registry](https://registry.platformio.org/libraries/depben/PT7C4339-RTC/), or clone the repository:
   ```bash
   git clone https://github.com/depben/PT7C4339-RTC.git
   ```

2. Copy the `PT7C4339-RTC` folder into your Arduino `libraries` directory.

3. Restart Arduino IDE.

### Example Sketch
```cpp
// SetAndReadDateTime example code for the PT7C4339-RTC library
// This example demonstrates how to set and read the date and time using both induvidual and single setters/getters,
// as well as how to check for timekeeping validity using the RTC Stop flag.
// More info on the GitHub page: https://github.com/depben/PT7C4339-RTC

#include <Arduino.h>
//#include <Wire.h> No need to include as the RTC library includes it already, but can be included if wanted
#include "PT7C4339-RTC.h"

static const uint8_t SDA_PIN = SDA; // Set to the SDA pin of the microcontroller
static const uint8_t SCL_PIN = SCL; // Set to the SCL pin of the microcontroller

// Construct PT7C4339 object called rtc
PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );

void setup()
{

    Serial.begin( 115200 );
    delay( 200 );

    // Initialize the RTC
    rtc.begin();

    // Not needed as rtc.begin() initializes Wire automatically, setting the I2C frequency to 400kHz, but can be called if wanted
    // Wire.begin();

    // Set the date using single setter
    PT7C4339_Date wantDate = { 2025, 5, 23, PT7C4339_WEEKDAY_UNKNOWN }; // Weekday will be set automatically, input parameter is ignored
    rtc.setDate( wantDate );

    // Set the time using individual setters
    rtc.setHour( 23 );
    rtc.setMinute( 59 );
    rtc.setSecond( 30 );

}

void loop()
{

    // Report if RTC was stopped or other factors made the timekeeping invalid
    if( rtc.getRtcStopFlag() )
    {

        Serial.println( "RTC Stop Flag set! Stored time may be inaccurate!" );
        rtc.clearRtcStopFlag(); // Clear Stop Flag

    }
    
    //Print the time to the serial monitor every second
    uint32_t now = millis();
    static uint32_t last = 0;
    uint32_t delay = 1000;

    if( now - last >= delay )
    {

        last = now;
    
        // Read the date using individual getters
        Serial.print( "Date and time: " );
        Serial.print( rtc.getYear() );
        Serial.print( "/" );
        Serial.print( rtc.getMonth() );
        Serial.print( "/" );
        Serial.print( rtc.getDay() );
        Serial.print( " " );
        switch( rtc.getWeekDay() )
        {
            case PT7C4339_MONDAY: Serial.print("Monday "); break;
            case PT7C4339_TUESDAY: Serial.print("Tuesday "); break;
            case PT7C4339_WEDNESDAY: Serial.print("Wednesday "); break;
            case PT7C4339_THURSDAY: Serial.print("Thursday "); break;
            case PT7C4339_FRIDAY: Serial.print("Friday "); break;
            case PT7C4339_SATURDAY: Serial.print("Saturday "); break;
            case PT7C4339_SUNDAY: Serial.print("Sunday "); break;
            default: Serial.print("Weekday was set incorrectly! "); break;
        }

        // Read the time using single getter
        PT7C4339_Time gotTime = rtc.getTime();
        Serial.print( gotTime.hour );
        Serial.print( ":" );
        Serial.print( gotTime.minute );
        Serial.print( ":" );
        Serial.println( gotTime.second );

    }

}
```

## License

This library is licensed under the MIT License. See `LICENSE` for more information.

## Acknowledgements

- Both [RTClib](https://github.com/adafruit/RTClib.git) and [DS3231-RTC](https://github.com/hasenradball/DS3231-RTC.git) helped a great deal with writing this library.
- Special thanks to Lady Ada and the contributors of [adafruit/ci-arduino](https://github.com/adafruit/ci-arduino), as well as BOJIT for [platformio-publish](https://github.com/marketplace/actions/platformio-publish).