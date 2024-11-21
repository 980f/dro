# dro
Digital Read Out for NC machinery.

This device watches a quadrature sensor to present a physical position to a UI.  
It estimates a speed to go along with the position, but that is not as accurate as the position, at different speeds the accuracy will be different. 
When standing still the signal can oscillate wildly, requiring some sort of filtering to ensure there is no false creep at standstill.
There will be some analog filtering which will create a limit on the speed in terms of position quanta per second that can be tracked. 
That limitation is outside the scope of this device's specification, one would have to set that when building an actual product.   

For devices without a network interface there will be an ascii protocol over a serial port for commands and data.  
For devices with a network interface (ESP8266/ESP32) there will be a webpage for this information.  

The position can be zeroed by host command. 

An index marker is supported. That is a signal that indicates the position is at some absolute reference point.
The low level implementation records the speed and position when the index sensor changes, with separate values for low to high and high to low.
Due to potential hysteresis and backlash this device will not automatically zero on the index marker change, but does support various combinations of the recorded transistions to be referenced by the zero command so that the host doesn't have to deal with reading the various values to compute a best estimate.

It can be put into "circular mode" for counting shaft rotations. The host supplies a counts per revolution and when the position crosses that value a rev's counter is incremented (or decremented) and the position zeroed. The value reported can be either signed or unsigned. 

While the host may poll for values at any time it can also program for them to be sent periodically by time or distance (or both).

---
Speed considerations:
1/5 uM per tick, 0.1/0.2 inches are commonly available.
A job speed of 1 foot per second seems adequate for manual milling, which is what this is target at.  
30 cm/s = 300 mm/sec = 300,000 um/sec.
If each tick is one fourth of a cycle then each signal runs at 600,000 per second.
A cortex-M3 takes around 30 clocks to do a minimal interrupt handler, if we have two such isr's running then we have 60 clocks * 600,000 = 36 Million clocks per second required. RP2040's run at 133Mhz.

That means we can handle quadrature tracking w/ a 72Mhz stm32F10x and still have 50% cpu time for talking to the host. If we use DMA for uarts and the like then that is quite easy to make work.
If we use DMA triggered by the timers, storing gpio into DMA buffers and counting from the stored data we can go even faster. 

---
Raspberry Pi Pico PIO device usage:  
The 2 bits of input cycle between 00,01,11,10 either forwards or backwards. If we use the PIO program counter to track these we can have two separate units whose counts are then merged by the host processor (CM0+). 
Each tracker toggles between states looking for 1 and looking for 0. 
When looking for a one we loop until we shift in a 1. When that happens if the other pin is low we increment, if high we decrement, and either way go to "looking for zero". That gets us 2 ticks per cycle of the sensor. 
The last bit of precision is only meaningful when the load is barely moving in which case one looks at the input for the other to generate the LSB, IE when moving slow we compute 2*counter + (A^B), or something very close.

At six clocks per transition with 133Mhz part that is 11M transitions per second.


---
Cheap DRO found online quoted 5 MHz rate, presumably 10M transitions per second. The RP2040 can handle that, barely. The second generation can do that with a bit more ease. 

