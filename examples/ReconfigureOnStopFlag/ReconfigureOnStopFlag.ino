// ReconfigureOnStopFlag example code for the PT7C4339-RTC library
// This example demonstrates how to configure the RTC on power up,
// configure the trickle charger, and use the square wave output to 
// trigger an interupt once every second.
// More info on the GitHub page: https://github.com/depben/PT7C4339-RTC

#include <Arduino.h>
//#include <Wire.h> No need to include as the RTC library includes it already, but can be included if wanted
#include "PT7C4339-RTC.h"

static const uint8_t SDA_PIN = SDA; // Set to the SDA pin of the microcontroller
static const uint8_t SCL_PIN = SCL; // Set to the SCL pin of the microcontroller
static const uint8_t RTC_INT = 21; // Set to the pin on the microcontroller that is connected to the INT/SQW pin of the PT7C4339 IC

uint16_t dateTimePrintedCount = 0; // Variable to count the number of times the date and time were printed since the last reset
volatile bool interruptTriggered = false; // Variable that the ISR uses to signal the main loop

// Construct PT7C4339 object called rtc
PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );

// ISR for the esp32 platform
void IRAM_ATTR isrOnFallingEdge()
{

    interruptTriggered = true;

}

// ISR for the arduino platform - if running on arduino hardware, uncomment this and comment out the one for esp32
/*
void isrOnFallingEdge()
{

    interruptTriggered = true;

}
*/

void setup()
{

    Serial.begin( 115200 );
    delay( 200 );

    // Initialize the RTC
    rtc.begin();

    // Not needed as rtc.begin() initializes Wire automatically, setting the I2C frequency to 400kHz, but can be called if wanted
    // Wire.begin();

    pinMode( RTC_INT, INPUT_PULLUP ); // Set square vave input pin to input with pullup resistor
    
    // Attach interrupt to the square wave output pin, calling printDateTime() on rising edge
    attachInterrupt( RTC_INT, isrOnFallingEdge, FALLING); // for the esp32 platform
    // attachInterrupt( digitalPinToInterrupt( RTC_INT ), isrOnFallingEdge, FALLING); // for the arduino platform - if running on arduino hardware, uncomment this and comment out the one for esp32

}

void loop()
{

    // Report if RTC was stopped or other factors made the timekeeping invalid
    if( rtc.getRtcStopFlag() )
    {

        Serial.printf( "\nRTC Stop Flag found set - Stored time may be inaccurate!\n" );
        Serial.println( "Clearing Stop Flag and restoring configuration..." );

        bool reset = rtc.reset(); // calling reset() after Stop Flag was set is recommended, but not needed.
        bool clear = rtc.clearRtcStopFlag(); // Clear Stop Flag
        bool begin = rtc.begin(); // Reinitialize the RTC

        if( !( reset && clear && begin ) ) Serial.println( "Failed to reset the RTC!" );
        else
        {

            Serial.println( "Successfully reset the RTC." );
            Serial.println( "Configuring RTC..." );

            bool batteryInt = rtc.enableIntFromBattery( false ); // Disable interrupt/square vawe from battery
            bool sqw = rtc.setIntOrSqwFlag( false ); // Set to square wave mode
            bool sqwFreq = rtc.setSqwFrequency( PT7C4339_SQW_1HZ ); // Set square wave frequency to 1Hz
            bool trickle = rtc.setTrickleChargerConfig( PT7C4339_TRICKLE_ENABLE, PT7C4339_DIODE_DISABLE, PT7C4339_RESISTOR_2K ); // Enable trickle charger with 2K Ohm resistor and no diode
            // More info on the trickle charger can be found on pages  of the datasheet: https://www.diodes.com/datasheet/download/PT7C4339_4339C.pdf

            if( !( batteryInt && sqw && sqwFreq && trickle ) ) Serial.println( "Failed to configure the RTC!" );
            else
            {
            
                Serial.println( "RTC configured successfully." );
                Serial.println( "Setting time and date..." );

                bool date = rtc.setDate( { 2025, 5, 23, PT7C4339_WEEKDAY_UNKNOWN } );
                bool time = rtc.setTime( { 23, 59, 45 } );

                if( !( date && time ) ) Serial.println( "Failed to set the time and date!" );
                else
                {
                
                    Serial.println( "Time and date set successfully." );

                    Serial.printf( "Date and time set to: ");
                    printDateTime();
                    Serial.println( "" );

                }

            }
        
        }

    }

    // If the interrupt was triggered since the last loop iteration, print the date, time and the number of times it was printed since the last reset
    if( interruptTriggered )
    {

        interruptTriggered = false; // Reset the interrupt flag
        dateTimePrintedCount++; // Increment the print count

        Serial.print( "Current date and time: " );
        printDateTime();
        Serial.printf( "Date and time printed %d times since start of sketch.\n", dateTimePrintedCount );

    }
    
    //Periodically stop the oscillator to simulate a power failure
    uint32_t now = millis();
    static uint32_t last = 0;
    uint32_t delay = 30000; // 30 seconds

    if( now - last >= delay )
    {

        last = now;
    
        rtc.enableOscillator( false );

    }

}

void printDateTime()
{

    Serial.printf( "%04d/%02d/%02d - ", rtc.getYear(), rtc.getMonth(), rtc.getDay() );
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
    Serial.printf( "%02d:%02d:%02d\n", rtc.getHour(), rtc.getMinute(), rtc.getSecond() );

}