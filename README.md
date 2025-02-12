# dro
Digital Read Out for nominally manual machine tools.

This device watches a quadrature sensor to present a physical position to a UI.  
It estimates a speed to go along with the position, but that is not as accurate as the position, at different speeds the accuracy will be different. 

There will be some analog filtering of the quadrature signals which will create a limit on the speed in terms of position quanta per second that can be tracked. 
That limitation is outside the scope of this device's specification, one would have to set that when building an actual product.   

For devices without a network interface there will be an ascii protocol over a serial port for commands and data.  
For devices with a network interface (ESP8266/ESP32) there will be a webpage for this information.  

The position can be zeroed by host command. 

An index marker is supported. That is a signal that indicates the position is at some absolute reference point.
The low level implementation records the speed and position when the index sensor changes, with separate values for low to high and high to low.
Due to potential hysteresis and backlash this device will not automatically zero on the index marker change, but does support various combinations of the recorded transitions to be referenced by the zero command so that the host doesn't have to deal with reading the various values to compute a best estimate.

It can be put into "rotational mode" for counting shaft rotations. The host supplies a counts per revolution and when the position crosses that value a rev's counter is incremented (or decremented) and the position zeroed. 

While the host may poll for values at any time it can also program for them to be sent periodically by time or distance (or both).

As many axes as possible will be accomodated with rapidly decreasing value after 3.