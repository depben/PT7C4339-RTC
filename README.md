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

## To Do
- Implement alarm functionality

## Limitations
This library uses 24-hour format for time representation and works from 1900/1/1 to 2099/12/31.

## Installation

1. Download the library with Arduino Library Manager, download from the [PlatformIO Registry](https://registry.platformio.org/libraries/depben/PT7C4339-RTC), or clone the repository:
   ```bash
   git clone https://github.com/depben/PT7C4339-RTC.git
   ```

2. Copy the `PT7C4339-RTC` folder into your Arduino `libraries` directory.

3. Restart Arduino IDE.

### Example Sketch

```cpp

```

## License

This library is licensed under the MIT License. See `LICENSE` for more information.

## Acknowledgements

- Both [RTClib](https://github.com/adafruit/RTClib.git) and [DS3231-RTC](https://github.com/hasenradball/DS3231-RTC.git) helped a great deal with writing this library.