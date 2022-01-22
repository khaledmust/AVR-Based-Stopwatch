# AVR_Based_Stopwatch
An implementation of a stopwatch using ATMega16 Microcontroller.

- **Timer1** with **CTC** mode is used to count the stopwatch time.
- A six common anode 7-segment display is used.
- The 7-segment is connected using the multiplexed technique, such that the MCU loops around each 7-segement individually and display the content with respect to it.
- An external interrupt **INT0** with falling edge is used for the *reset* of the time.
- An external interrupt **INT1** with rasing edge is used to *pasue* the time.
- An external interrupt **INT2** with falling edge is used to *resume* the time.
