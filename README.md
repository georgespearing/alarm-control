# alarm-control
building an alarm clock around STM32F103

## Planned features: 

- Clock display (LCD)
  - Backlight brightness toggle option
- Led Strip Light gradual turn on (or relay to power lamp)
- Buzzer with 2 different mode settings
- Power cord or batter backup.
	- Battery Shall power clock for >= 72 hours

- Modes: 
    - LED strip
    - Relay (120v outlet)
    - Buzzer

## I/O

- 6 Buttons
	- 1 alarm on/off toggle, press & hold to set time
	- Hour Increment
	- Minute Increment
	- Backlight settting 
	- Feature settting toggle (ext. light, buzzer, both)
	- 12/24 hour switch
- Power Cord
- Battery
- Clock
- Backlight (clock brightness)
- Plug for 12v light source (to turn on with alarm)

## Power Supplies

- On Battery Power
	- Disable 12V light output
	- Dim clock to lowest setting

- On Plug Power
	- Allow user settings
	- Charge Battery
 
- Power Requirements
	- Minimum: 
	- Maximum: 
	
	- STM32 Average: 
	- Lights: 
	- Buzzer: 

## Functions (code outline)

- All Buttons have interrupts
	- time / feature setting only active if alarm button was press & held

- callback for buzzer
- Timer for clock function
- Something to check wall power vs battery power
