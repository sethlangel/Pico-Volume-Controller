# Raspberry Pico/PicoW Volume Controller (Windows)
Volume Controller for Windows using the RaspberyPi Pico/PicoW. Allows you to change volume using potentiometers in real time for applications that you may choose.

# How it works
This project uses both C++ on the PC side to handle the Windows API calls to make changes to the PC's volume and CiruitPython on the Pico/PicoW side to send the potentiometers data through COM ports.

# Requirements
- Windows
- 3x 10k potentiometers
- Wire
- Raspberry Pico/PicoW
  
# Setup Raspberry Pico
1. Install CircuitPython to your Raspberry Pico or PicoW.
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

# Configure settings.json
1. Configure COM port.
    - Open **Device Manager**.
    - Open the **Ports (COM & LPT)** dropdown.
    - **USB Serial Device (COM##)** is what you are looking for, specifically **COM##**.
    - Open settings.json and put the COM port number into the **comport** value.
2. Configure applications.
    - ```master``` is the PC's master volume.
    - Any application you want to assign to a potentiometer needs to be written as ```application.exe```.
    - You can have multiple applications assigned to a single potentiometer.
    - Id's are mapped to ADC pins on Pico/PicoW respectively.

Here is an example of ADC0 controlling the master volume, ADC1 controlling Spotify and Google Chrome, and ADC2 controlling Discord.
  ```
    "applications": [
  	{
  		"id": 0,
  		"name": [ "master" ]
  	},
  	{
  		"id": 1,
  		"name": [ "spotify.exe", "chrome.exe" ]
  	},
  	{
  		"id": 2,
  		"name": [ "discord.exe" ]
  	}]
  ```
