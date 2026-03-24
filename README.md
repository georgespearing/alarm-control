# Alarm Project

Code base for Custom Alarm 

## Features

1. Display for Time
    - Time in hour and minute. Default 24h. 
    - Indicator for Alarm Active (Sound On)
    - Indicator for Light Active (Alarm Must be active)

2. Buttons for the following
    - Time Set
    - Alarm (Set Time)
    - Hour
    - Minute
    - Alarm / Light (Whether to have a light to come on)
    - Snooze (buzzer only if light is on)

3. Buzzer for Alarm 

4. Light (12V LED strip)

5. Battery Backup
    - Reach Goal. Two phases. First phase is just to keep current time and alarm time if main power is removed. Second phase is full operation (light optional) 

## Hardware

- ESP32
- 12V LED strip (DC Jack)
- 12V input (DC Jack)

## Code 

- If the Time Set button is held, blink the screen
    - As hour is pressed, increase display
    - As minute is pressed, increase display

- If the Alarm set button is held, blink the screen
    - As hour is pressed, increase display
    - As minute is pressed, increase display

- If the Alarm / Light button is pressed, 
    - one press turns on alarm
    - two press adds light option
    - three press turns off

- If the Snooze button is pressed (only valid if alarm is actively triggered)
    - If light is on, keep light on
    - Alarm buzzer should turn off for 8 minutes
    