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
5. Typing the insight command in the command processor will give the current reference angle set on the display device(teraterm)

### Notes
* Please refer `submitted-for-review` tag in github to download
* Only the .c and .h files are uploaded. The Debug folder is not uploaded.
* The start_play_back function is used directly with only buffer name modified. Comments and additonal features are added.
* The outputs are present in the screenshot folder. A text file log is added named - 
* A vdeo is uploaded for the assignment.
* Only the required functions are exposed. Rest functions has static keyword assigned to restrict the file scope
* The macros are used in .h file and are used only if they are required in more than one module. Ex : CBFIFO_SIZE,PWM_PERIOD,SYSCLOCK_FREQUENCY
* All the function headers are desribed in .c file
* Only if required, a strucutre is used in .h file as required in multiple files.
* Every extra file will be found in AdditionalUploads

### Technical description
* Code implemented for Digital Angle Gauge for achieving 45 degree. It was mentioned to implement the code for 45 degree angle where as the current implementation is for 30,45,60 and 90. Also, the idea was very bried with that time.
* The current current implementation has more features than the one's mentioned and it has gone slight modification in the displa behaviour as ideas are shaped to better the system with develpment as state machine is also introduced.
* The features are as the one mentined above in project description.
* Refer attached diagram for state machine behavior

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


### File description
* `main.c,main.h:` This file consists if initialization of all the modules required in the project, all the automated and manual tests carried out as well as the application entry point.
* `cbfifo.c,cbfifo.h:`contains a circular buffer based implementation along with uart as interrupt mode (quick settings described are same as teraterm) for better performance of system
* `cbfifo_testcase.c,cbfifo_testcase.h:` Tests the cbfifo queue for capacity, length, enqueue operaton adn dequeue operation
* `commandprocessor.c,commandprocessor.h:` Handles the author,insight and help command as described in quick commands
* `i2c.c,i2c.h:` Handles the basic read,write and initialization for the i2c bus
* `led_tpm_testcase.c,led_tpm_testcase.h:` Tests all teh led pwm features - min, max, for each rgb as well as transition
* `led_tpm.c,led_tpm.h`: performs the initialization of tom module 0 and 2 for the rgb led''s as well as has basic set functions
* `mma8451.c,mma8451.h`: accerometer initialization as well as read and write byte code calling teh drivres in i2c.c
* `msec_timers.c,msec_timers.h:` Provides the get_timer, reset_timer functionality .Used along with systick feature operating at 50 msec frquency for 24 Mhz as the source clock
* `state_machine.c,state_machine.h:` Handles the state transition event for INIT_STATE,CALIBRATE_STATE,ANGLE_DISPLAY_STATE
* `switch.c,switch.h:` hanldes the switch interrupt for PTD3
* `sysclock.c,sysclock.h:`sets teh clock for 24 Mhz operation
* `test_module.c,test_module.h`: All the test suites are execute from here
* `touch_slider.c,touch_slider.h:` Handles the touch interrupt from the slider
* `uart.c,uart.h:` Set uart in interrupt mode for the settings mentioned in quick teraterm settings.

### Futurescope:
Add a cusomised angle setting through command processor.
Add a buzzer to sound at the exact angle.
Add more test cases for each module.
There is a manual test case sheet (excel) uplaoded describing the accerometer and state machine transition as well as touch sensor and switch verification test cases (8)

### Environment
* IDE: MCUXpressso
* version: 11.2.0
* Windows 10
* Board/Hardware: FRDM-KL25Z (NXP)

### Author
* Rishab Shah

### References
* FRDM KL25Z reference manual
* Alexander G Dean github : https://github.com/alexander-g-dean/ESF/tree/master/NXP
* Files, code snippets shared by Howdy in the lecture notes as well as the canvas
* https://www.youtube.com/watch?v=dXWbezxJb2g - link used to understand the behaviour of DAG
* Previous modules used in the assignment.
* Breakfast serial zip file shared
