# MidiBox3

My attempt to make new Waveblaster-compatible interface board.

## Features

* Work with all current loop MIDI source
* Additional Note On volume control with rotary encoder
* Can work without microcontroller as simple interface board but other features will gone.

## Plans


Measure power consumption. What maximum current on +5V line is? Do I really need output filter and what parameters it must have? So, I need to characterize all imortant parameters.

Next, there are more components on PCB that really needed for working. Also some components are more expensive than necessary. It will be reasonable to find cheaper ones.

Finally, STM32F103CBT6 is a beast and I love it. It have USB support and I planned use it in next revision of PCB. I don't know about ADC out via USB, but as power source and MIDI in port it will definitely work. Maybe it also may work as HardMPU, but this will require special DOS driver and I think it has lower priority.
