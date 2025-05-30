#include <unity.h>
#include "PT7C4339-RTC.h"

#define SDA_PIN 47 //Change this to the correct pin number for your board or disregard it if you are using the default SDA pin
#define SCL_PIN 48 //Change this to the correct pin number for your board or disregard it if you are using the default SCL pin

#define INT 21

PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );

void test_alarm2_disable()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setDate( { 2025, 5, 30, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 13, 45, 59 } );
    rtc.setA2DayDate( { 0, 0, 0, PT7C4339_FRIDAY } );
    rtc.setA2Time( { 13, 46, 0 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_DISABLE );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA2Flag() ) success++;
    else fail++;

    rtc.setDate( { 2000, 1, 1, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 0, 0, 0 } );
    delay( 1500 );
    if( digitalRead( INT ) && !rtc.getA2Flag() ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 disabled test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm2_every_minute()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 5;

    rtc.setTime( { 13, 45, 59 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_EVERY_MINUTE );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 13, 46, 59 } );
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 13, 47, 59 } );
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 13, 48, 59 } );
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 13, 59, 59 } );
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 every minute test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm2_minute_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setTime( { 13, 45, 59 } );
    rtc.setA2Time( { 0, 46, 0 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_MINUTES_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 14, 45, 59 } );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 minute match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm2_hour_minute_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setDay( 21 );
    rtc.setTime( { 13, 45, 59 } );
    rtc.setA2Time( { 13, 46, 0 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_HOURS_MINUTES_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setDay( 22 );
    rtc.setTime( { 13, 45, 59 } );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 hour, minute match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm2_day_hour_minute_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 3;

    rtc.setMonth( 1 );
    rtc.setDay( 25 );
    rtc.setTime( { 13, 45, 59 } );
    rtc.setA2DayDate( { 0, 0, 25, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setA2Time( { 13, 46, 0 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_DAY_HOURS_MINUTES_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setDay( 26 );
    rtc.setTime( { 13, 45, 59 } );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA2Flag() ) success++;
    else fail++;

    rtc.setMonth( 11 );
    rtc.setDay( 25 );
    rtc.setTime( { 13, 45, 59 } );
    
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 day, hour, minute match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm2_weekday_hour_minute_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 3;

    rtc.setDate( { 2025, 5, 30, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 13, 45, 59 } );
    rtc.setA2DayDate( { 0, 0, 0, PT7C4339_FRIDAY } );
    rtc.setA2Time( { 13, 46, 0 } );
    rtc.clearA2Flag();
    rtc.setA2Rate( PT7C4339_A2_WEEKDAY_HOURS_MINUTES_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setDay( 31 );
    rtc.setTime( { 13, 45, 59 } );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA2Flag() ) success++;
    else fail++;

    rtc.setMonth( 6 );
    rtc.setDay( 6 );
    rtc.setTime( { 13, 45, 59 } );
    
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA2Flag() && rtc.clearA2Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 2 weekday, hour, minute match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void setUp( void )
{
    pinMode( INT, INPUT_PULLUP );
    rtc.reset();
    rtc.begin();
    rtc.enableOscillator( true );
    rtc.enableIntFromBattery( false );
    rtc.setIntOrSqwFlag( true );
    rtc.enableA2Int( true );
}

void tearDown(void)
{
    
}

void setup() 
{
    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_alarm2_disable );
    RUN_TEST( test_alarm2_every_minute );
    RUN_TEST( test_alarm2_minute_match );
    RUN_TEST( test_alarm2_hour_minute_match );
    RUN_TEST( test_alarm2_day_hour_minute_match );
    RUN_TEST( test_alarm2_weekday_hour_minute_match );
    
    UNITY_END();
}

void loop()
{



}