# AVR Based Stopwatch
An implementation of a stopwatch using ATMega16 Microcontroller.
![Alt text here](https://github.com/khaledmust/AVR-Based-Stopwatch/blob/4d11df499fb5bd4f055dd9117b06e1c1b33c5a51/Stopwatch%20Preview.png)

## Outline of the project
- **Timer1** with **CTC** mode is used to count the stopwatch time.
- A six common anode 7-segment display is used.
- The 7-segment is connected using the multiplexed technique, such that the MCU loops around each 7-segement individually and display the content with respect to it.
- An external interrupt **INT0** with falling edge is used for the *reset* of the time.
- An external interrupt **INT1** with rasing edge is used to *pause* the time.
- An external interrupt **INT2** with falling edge is used to *resume* the time.

## Code explanation
### Timer1
- Timer1 is configured in CTC mode with an inital count value of 0, and having its interrupt enabled.
- The clock supplied to the timer is of a prescaler value of 64.
- In order for the timer to count and produce an interrupt every 1 second the value of the compare register is 0x3D08 (15624 clock):
  - This value is calculated as following:
![Alt text here](https://github.com/khaledmust/AVR_Based_Stopwatch/blob/main/OCR_Calc.svg)

- So every 1 second an interrupt is triggered resulting in the increment of the variable `seconds` in the ISR (Interrupt Service Routine), and the calculations of minutes and hours are done accordingly.

### INT0 (Interrupt 0)
- The purpose of this interrupt is to reset the timer.
- The interrupt is triggered via a push button resulting in the exection of the interrupt's ISR.
- Which results in the zeroing of the timers parameters.

### INT1 (Interrupt 1)
- The purpose of this interrupt is to pause the timer from counting.
- The interrupt is triggered via a push button resulting in the execution of the interrupt's ISR.
- The clock source is disables resulting in the stop of the count of the timer.

### INT2 (Interrupt 2)
- The purpose of this interrupt is to resume the couting of the timer.
- The interrupt is triggerd via a push button resulting in the exection of the interrupt's ISR.
- The clock source is re-enabled resulting the resume of the count of the timer.
