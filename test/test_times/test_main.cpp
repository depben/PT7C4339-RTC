#include <Arduino.h>
#include <unity.h>
#include "PT7C4399-RTC.h"


PT7C4339 rtc( &Wire, 47, 48 );

void test_times_all()
{

    uint32_t success = 0;
    uint32_t fail = 0;
    uint32_t totalTimes = 24 * 60 * 60;

    for( uint8_t hour = 0; hour < 24; hour++ )
    {
    
        for( uint8_t minute = 0; minute < 60; minute++ )
        {

            for( uint8_t second = 0; second < 60; second++ )
            {
            
                PT7C4339_Time setTime = { hour, minute, second };
                bool setSuccess = rtc.setTime( setTime );
                PT7C4339_Time getTime = rtc.getTime();

                bool timeOk = setSuccess && getTime.hour == setTime.hour && getTime.minute == setTime.minute && getTime.second == setTime.second;
                if( timeOk ) success++;
                else fail++;

            }

        }

    }

    TEST_ASSERT_EQUAL_UINT32_MESSAGE( 0, fail, "Some time tests failed" );
    TEST_ASSERT_EQUAL_UINT32( totalTimes, success + fail );

}

void setUp( void )
{

    rtc.begin();
    rtc.reset();

}

void tearDown(void)
{

    rtc.reset();

}

void setup() 
{

    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_times_all );
    
    UNITY_END();

}

void loop()
{



}