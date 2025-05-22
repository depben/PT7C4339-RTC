# Arduino I2C RTC Library

This library provides an interface for accessing real-time clock (RTC) devices over the I2C protocol. It is designed for Arduino and compatible platforms.

## Features

- Read and set the current date and time
- Support for various RTC chips
- Easy to use API

## Installation

1. Download the library or clone the repository:
   ```bash
   git clone https://github.com/yourusername/Arduino-I2C-RTC.git
   ```

2. Copy the `I2C_RTC` folder into your Arduino `libraries` directory.

3. Restart the Arduino IDE.

## Example Usage

Here's a simple example of how to use the I2C RTC library.

### Example Sketch

```cpp
#include <Wire.h>
#include <I2C_RTC.h>

I2C_RTC rtc;

void setup() {
  Serial.begin(9600);
  rtc.begin();

  // set the time to January 1, 2023, 12:00:00
  rtc.setDateTime(2023, 1, 1, 12, 0, 0);
}

void loop() {
  rtc.update();
  Serial.print("Current time: ");
  Serial.print(rtc.getHour());
  Serial.print(":");
  Serial.print(rtc.getMinute());
  Serial.print(":");
  Serial.println(rtc.getSecond());
  
  delay(1000); // Wait for 1 second
}
```

## API Reference

### Initialization

```cpp
void begin();
```

### Set Date and Time

```cpp
void setDateTime(int year, int month, int day, int hour, int minute, int second);
```

### Get Current Date and Time

```cpp
int getYear();
int getMonth();
int getDay();
int getHour();
int getMinute();
int getSecond();
```

## Supported RTC Chips

- DS1307
- DS3231
- And more (check the source code for specific support)

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

## License

This library is licensed under the MIT License. See `LICENSE` for more information.

## Acknowledgements

- Inspired by [RTC libraries](https://github.com/adafruit/RTClib)

## Contact

If you have any questions or feedback, please reach out at your.email@example.com.