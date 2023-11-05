# Introduction To Robotics (2023-2024)
This course taken at the Faculty of Mathematics and Computer Science, University of Bucharest is presenting through hands-on projects the basic know-how on microcontrollers and Robotics, with the help of Arduino.

## Homework 0
Setup created

## Homework 1
### Technical Task : 
- Use a separate potentiometer for controlling each color of the RGB LED: Red,
Green, and Blue. This control must leverage digital electronics. Specifically,
you need to read the potentiometer’s value with Arduino and then write a
mapped value to the LED pins.

### Components :
- RGB LED (At least 1)
- Potentiometers (At least 3)
- Resistors and wires as needed

#### Link video YouTube : https://youtu.be/YBlDi4F6qHw?si=--Z2Zk2bOt9gVghe

![RGB_Led](https://github.com/Smaranda02/IntroductionToRobotics/assets/62556419/1a9f3c76-5724-4067-98f2-6cd62dcbefde)

## Homework 2
### Technical Task : 
Design a control system that simulates a 3-floor elevator using the Arduino
platform. Here are the specific requirements:
- LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.
The LED corresponding to the current floor should light up. Additionally,
another LED should represent the elevator’s operational state. It should
blink when the elevator is moving and remain static when stationary.
- Buttons: Implement 3 buttons that represent the call buttons from the
3 floors. When pressed, the elevator should simulate movement towards
the floor after a short interval (2-3 seconds).
- Buzzer:
The buzzer should sound briefly during the following scenarios:
– Elevator arriving at the desired floor (something resembling a ”cling”).
– Elevator doors closing and movement (pro tip: split them into 2
different sounds)
- State Change & Timers: If the elevator is already at the desired floor,
pressing the button for that floor should have no effect. Otherwise, after
a button press, the elevator should ”wait for the doors to close” and then
”move” to the corresponding floor. If the elevator is in movement, it
should either do nothing or it should stack its decision (get to the first
programmed floor, open the doors, wait, close them and then go to the
next desired floor).
- Debounce: Remember to implement debounce for the buttons to avoid
unintentional repeated button presses.

### Components :
- LEDs (At least 4: 3 for the floors and 1 for the elevator’s operational
state)
- Buttons (At least 3 for floor calls)
- Buzzer (1) 
- Resistors and wires as needed

#### Link video YouTube : https://youtu.be/plprVaXs6RE?si=7qVTJfLfIF43Z5FZ
![elevator](https://github.com/Smaranda02/IntroductionToRobotics/assets/62556419/e48ecd26-a915-4afc-984b-167fa4bb9d89)

## Homework 3

### Task :
You will use the joystick to control the position of
the segment and ”draw” on the display. The movement between segments
should be natural, meaning they should jump from the current position
only to neighbors, but without passing through ”walls”.

### Components:
- 1 7-segment display, 1 joystick, resistors and wires (per logic)
![7_SegmentDisplay](https://github.com/Smaranda02/IntroductionToRobotics/assets/62556419/ac76bb89-bec1-4c62-ac10-d5d18891bd9c)

#### Link video YouTube : https://youtu.be/rHZj7NF1vSM?si=Sbb81Y0wDbydeFjb



