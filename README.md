# Raspberry Pico/PicoW Volume Controller (Windows)
Volume Controller for Windows using the RaspberyPi Pico/PicoW. Allows you to change volume using potentiometers in real time for applications that you may choose.

# Requirements
- Windows
- 3x 10k potentiometers
- Wire
- Raspberry Pico/PicoW
  
# Setup Raspberry Pico
1. Install CircuitPython to your Pico or PicoW.
   - [Install Guide](https://learn.adafruit.com/welcome-to-circuitpython/installing-circuitpython).
   - Alternatively, you can install using [Thonny](https://thonny.org/) if you have it installed.
     - Connect Pico/PicoW and open Thonny
     - At the bottom right of Thonny click on the dropdown and select **Install CircuitPython...** \
      ![image](https://github.com/user-attachments/assets/0dfda476-09f8-414d-be96-063910b4aa27)
     - Open the **variant** dropdown and select your model of Raspberry Pico. \
      ![image](https://github.com/user-attachments/assets/47e1df08-ee5c-43c4-bfa8-961e3af0e6ac)
     - Click on **Install**
2. Copy and paste the **code.py** file from this repository into your Raspberry Pico.

3. Solder potentiometers to Pico similar to diagram. \
   <img src="https://github.com/user-attachments/assets/34f56536-324f-4436-a821-b7188dde43e8" width="400" height="500">

Raspberry Pico is now all set!

     
This repo is in early development and has hard coded values such as COMPORT name and application names.
