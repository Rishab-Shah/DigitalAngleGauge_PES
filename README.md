## DigitalAngleGauge_PES
ECEN 5813: PES : Spring 2021 : Final Project : Digital Angle Gauge

### Project Description
In this projectt, I have used the onboard accelerometer (MMA8451) of the FRDMKL25Z over I2C serial communication to try to prototype a digital angle gauge (DAG).
#### Features:
The DAG has following features:
1. When initialised it asks for manual intervention to one time set it to 0 degree.
2. The user will get visual feedback through onboard LED's driven by a PWM. These LED's are lit momentarily and only when one of teh below angle is reached. The user has to try to roll the FRDM by it;s long edge:
Following are the different angles:
    a. Red - 30 degree.
    b. Green - 45 degree.
    c. Blue - 60 degree.
    d. White - 90 degree.
3. The user can press switch (external) to enter into a calibration state and recaibrate the relative reference and see the angle deviation w.r.t that reference and the LED will glow as described in point 2.
4. To calibrate the relative reference, the user has to press on the capacitive touch slider and again go through the point 2.
5. Typing the insight commadn in the command processor will give the current reference angle set on the display device(teraterm)

### Technical description
* Code implemented for Digital Angle Gauge meets the requirement mentioned in the Final project proposal. It was mentioned to implement the code for 45 degree angle where as the implementtion was for 30,45,60 adn 90
* The 
* Please refer `submitted-for-review` tag in github to download

### Quick Teramterm settings:
A teraterm with following settings is required.
1. Baud rate : 38,400
2. Stop bit 2
3. Data Size : 8
4. No Parity

### Quick commands:
1. author - code author
2. insight - current amgle used for reference
3. help - list of commands & their description

### Testing:
Following modules are tested:
1. An automated test script is used for CBFIFO.
2. The LED module is tested. Individual color as well as the the minimum to maximum variation of brightness.
3. The changing of LED color as per the angle value passed, is verified.Even negative values are verified.
4. After all the testing. A 0 degree calibration for MMA8451 is performed for the roll value to bring it to ground reference. The LED color is in blue.
5. Only when successful calibration to 0 degree angle occurs, a green color LED is lit momentarily.

### Notes
* Only the .c and .h files are uploaded. The Debug folder is not upload
* The start_play_back function is used directly with only buffer name modified. Comments and additonal features are added.
* The outputs are present in the screenshot folder. A text file log is added named - 
* A vdeo is uploaded for the assignment.
* Only the required functions are exposed. Rest functions has static keyword assigned to restrict the file scope
* The macros are used in .h file and are used only if they are requireed in more than one module. Ex : CBFIFO_SIZE,PWM_PERIOD,SYSCLOCK_FREQUENCY
* All the function headers are desribed in .c file

### File description
* `main.c,main.h:` This file consusts if initialization of all the modules required in the project, all the automated and manual tests carried out as well as the application entry point.

### Futurescope:
Add a cusomised angle setting through command processor.
Add a buzzer to sound at the exact angle.
Add more test cases for each module.

### Environment
* IDE: MCUXpressso
* version: 11.2.0
* Windows 10
* Board/Hardware: FRDM-KL25Z (NXP)

### Author
* Rishab Shah

### References
* Lecture notes: Analog Part -1 and Analog Part - 2
* FRDM KL25Z reference manual
* https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_6
* https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_7
* https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8
* https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_9
* Files, code snippets shared by Howdy in the lecture notes as well as the canvas
* https://www.youtube.com/watch?v=dXWbezxJb2g - lik used to understand the behaviour of DAG
* All previous modules used in the assignment.
