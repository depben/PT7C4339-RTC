#include <Arduino.h>
#include <Wire.h>
#include "PT7C4339-RTC.h"

PT7C4339 rtc( &Wire );

void setup()
{
    Serial.begin( 115200 );
    delay( 200 );

    rtc.begin();

    // The rtc.begin() function calls Wire.begin() automatically, and sets the I2C clock to 400kHz
    // Wire.begin();
}

void loop()
{
    if( rtc.getRtcStopFlag() )
    {
        Serial.println( "RTC Stop Flag set! Stored time may be inaccurate!" );
        rtc.clearRtcStopFlag();
    }

    rtc.setHour(13);
    rtc.setMinute(30);
    rtc.setSecond(45);

    PT7C4339_Date date;
    date.year = 2025;
    date.month = 5;
    date.day = 23;
    date.weekDay = PT7C4339_WEEKDAY_UNKNOWN;
    rtc.setDate( date );

    Serial.print("Date and time set to: ");
    Serial.print( rtc.getYear() );
    Serial.print("/");
    Serial.print( rtc.getMonth() );
    Serial.print("/");
    Serial.print( rtc.getDay() );
    Serial.print(" ");
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

    Serial.print( rtc.getHour() );
    Serial.print(":");
    Serial.print( rtc.getMinute() );
    Serial.print(":");
    Serial.println( rtc.getSecond() );
    delay( 5000 );
}