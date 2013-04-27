CurrentMonitor is a dual-channel current and voltage monitor based on two INA138 current shunt monitoring ICs. It's based on an atmega168 and should run between 3.3 and 5V input (5V tested).

In the given configuration, the maximum values are:

Input Voltage: 3.3V - 5.5V
Maximum Monitoring Current: 2.5A
Maximum Monitoring Voltage: 25V
Current Gain: 2 (1mA = 2mV)
Voltage Dividor: 5 (1M/200k)
Self Power Usage: 5mA

By adjusting the resistors for the voltage divider and the gain resistor, you can adapt the circuit to your own needs. No communication or storage is present; you need to add your own I²C and serial communication busses to it.

The schematic uses a mix of through-hole and SMD components, because I wasn't sure if my chosen resitor values would fit; if they wouldn't, I could simply replace them. Feel free to replace them all with SMD resistors. But at the end, it turned out that my calculations were correct; but I haven't gotten around to change all components to SMD.

I'm using a 1% 10k gain resistor, which results in a gain of 2. You can use other values as well; refer to the INA138 datasheet where they listed.

As voltage and current monitoring is pretty much important, I'm planning to sell PCBs with serial, I²C and SD card logging capabilities.

The firmware isn't completed and will be uploaded as soon as it's done.
