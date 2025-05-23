// Example code for the PT7C4339-RTC library
// This example demonstrates how to set and read the date and time using both induvidual and single setters/getters.
// More info on the GitHub page: https://github.com/depben/PT7C4339-RTC 

#include <Arduino.h>
#include <Wire.h>
#include "PT7C4339-RTC.h"

// Construct PT7C4339 object called rtc
PT7C4339 rtc( &Wire, 47, 48 );

void setup()
{

    Serial.begin( 115200 );
    delay( 200 );

    // Initialize the RTC
    rtc.begin();

    // Not needed as rtc.begin() initializes Wire automatically, setting the I2C frequency to 400kHz, but can be called if wanted
    // Wire.begin();

}

void loop()
{

    // Report if RTC was stopped
    if( rtc.getRtcStopFlag() )
    {

        Serial.println( "RTC Stop Flag set! Stored time may be inaccurate!" );
        rtc.clearRtcStopFlag();

    }

    // Set the time using individual setters
    rtc.setHour( 13 );
    rtc.setMinute( 30 );
    rtc.setSecond( 45 );

    // Set the date using single setter
    PT7C4339_Date wantDate;
    wantDate.year = 2025;
    wantDate.month = 5;
    wantDate.day = 23;
    wantDate.weekDay = PT7C4339_WEEKDAY_UNKNOWN;
    rtc.setDate( wantDate );

    // Read the time using individual getters
    Serial.print( "Date and time set to: " );
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
    PT7C4339_Time gotTime;
    Serial.print( gotTime.hour );
    Serial.print( ":" );
    Serial.print( gotTime.minute );
    Serial.print( ":" );
    Serial.println( gotTime.second );
    
    delay( 5000 );

}