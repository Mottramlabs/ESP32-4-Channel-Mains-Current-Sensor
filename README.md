# ESP32-4-Channel-Mains-Current-Sensor
A 4-Channel mains sensor using an ESP32 two board versions one using a Wemos ESP32 the other uses a Node-MCU board. Both boards can work with either a current or voltage type of CT's (current transformers).

###### Firmware Version 1

Using the ESP32's own A/D (12-bit) to measure the output of the current transformer, this output voltage is measured across a burned resistor. This resistor is either on the PCB (22R) for the current type CT's or fitted inside the CT's casing for the voltage type. This version has some digital filtering which improves resolution at lower power levels.

### PCB Layout - MLP201185 (Wemos Version)

![](https://github.com/Mottramlabs/ESP32-4-Channel-Mains-Current-Sensor/blob/main/Pictures/PIX201185.jpg)

### PCB Layout - MLP201188 (Node-MCU Version)

![](https://github.com/Mottramlabs/ESP32-4-Channel-Mains-Current-Sensor/blob/main/Pictures/PIX201188.jpg)

### PCB Layout - MLP201191 (ESP£" S2 Version)

![](https://github.com/Mottramlabs/ESP32-4-Channel-Mains-Current-Sensor/blob/main/Pictures/PIX201191.jpg)
