#include <unity.h>
#include "PT7C4339-RTC.h"

#define SDA_PIN 47 //Change this to the correct pin number for your board or disregard it if you are using the default SDA pin
#define SCL_PIN 48 //Change this to the correct pin number for your board or disregard it if you are using the default SCL pin

#define INT 21

PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );

void test_alarm1_disable()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setDate( { 2025, 5, 30, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 13, 45, 55 } );
    rtc.setA1DayDate( { 0, 0, 0, PT7C4339_FRIDAY } );
    rtc.setA1Time( { 13, 45, 56 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_DISABLE );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA1Flag() ) success++;
    else fail++;

    rtc.setDate( { 2000, 1, 1, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 0, 0, 1 } );
    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA1Flag() ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 disabled test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_every_second()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 5;

    rtc.setTime( { 13, 45, 59 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_EVERY_SECOND );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 every second test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_second_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setTime( { 13, 45, 54 } );
    rtc.setA1Time( { 0, 0, 55 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_SECONDS_MATCH );

    delay( 1500 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setMinute( 46 );
    rtc.setSecond( 54 );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 second match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_minute_second_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 2;

    rtc.setTime( { 13, 45, 54 } );
    rtc.setA1Time( { 0, 45, 55 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_MINUTES_SECONDS_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 14, 45, 54 } );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 minute, second match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_hour_minute_second_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 3;

    rtc.setDay( 2 );
    rtc.setTime( { 13, 45, 54 } );
    rtc.setA1DayDate( { 0, 0, 25, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setA1Time( { 13, 45, 55 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_HOURS_MINUTES_SECONDS_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setTime( { 14, 45, 54 } );

    delay( 1500 );
    if( digitalRead( INT ) && !rtc.getA1Flag() ) success++;
    else fail++;

    rtc.setDay( 25 );
    rtc.setTime( { 13, 45, 54 } );
    
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 hour, minute, second match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_day_hour_minute_second_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 3;

    rtc.setMonth( 1 );
    rtc.setDay( 25 );
    rtc.setTime( { 13, 45, 54 } );
    rtc.setA1DayDate( { 0, 0, 25, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setA1Time( { 13, 45, 55 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_DAY_HOURS_MINUTES_SECONDS_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setDay( 26 );
    rtc.setTime( { 13, 45, 54 } );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA1Flag() ) success++;
    else fail++;

    rtc.setMonth( 11 );
    rtc.setDay( 25 );
    rtc.setTime( { 13, 45, 54 } );
    
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 day, hour, minute, second match test failed!" );
    TEST_ASSERT_EQUAL_UINT32( total, success + fail );
}

void test_alarm1_weekday_hour_minute_second_match()
{
    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t total = 3;

    rtc.setDate( { 2025, 5, 30, PT7C4339_WEEKDAY_UNKNOWN } );
    rtc.setTime( { 13, 45, 54 } );
    rtc.setA1DayDate( { 0, 0, 0, PT7C4339_FRIDAY } );
    rtc.setA1Time( { 13, 45, 55 } );
    rtc.clearA1Flag();
    rtc.setA1Rate( PT7C4339_A1_WEEKDAY_HOURS_MINUTES_SECONDS_MATCH );

    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    rtc.setDay( 31 );
    rtc.setTime( { 13, 45, 54 } );

    delay( 1050 );
    if( digitalRead( INT ) && !rtc.getA1Flag() ) success++;
    else fail++;

    rtc.setMonth( 6 );
    rtc.setDay( 6 );
    rtc.setTime( { 13, 45, 54 } );
    
    delay( 1050 );
    if( !digitalRead( INT ) && rtc.getA1Flag() && rtc.clearA1Flag() && digitalRead( INT ) ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Alarm 1 weekday, hour, minute, second match test failed!" );
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
    rtc.enableA1Int( true );
}

void tearDown(void)
{
    
}

void setup() 
{
    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_alarm1_disable );
    RUN_TEST( test_alarm1_every_second );
    RUN_TEST( test_alarm1_second_match );
    RUN_TEST( test_alarm1_minute_second_match );
    RUN_TEST( test_alarm1_hour_minute_second_match );
    RUN_TEST( test_alarm1_day_hour_minute_second_match );
    RUN_TEST( test_alarm1_weekday_hour_minute_second_match );
    
    UNITY_END();
}

void loop()
{



}