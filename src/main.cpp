#include <Arduino.h>
#include <Wire.h>
#include "PT7C4339-RTC.h"

PT7C4339 rtc( &Wire, 47, 48 );

void setup() 
{

    Serial.begin( 115200 );
    Wire.begin( 47, 48 );
    Wire.setClock( 400000 );
    delay( 200 );

}

void loop()
{



}