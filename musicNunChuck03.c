/* Nunchuck controls GPIO outputs.
 * Must be compiled with the -lwiringPi option. Example: gcc filename.c -lwiringPi
*/ 

/* Glossary
 * GPIO6, GPIO12. etc	GPIO PINS 
 * wiringPiI2CSetup 	a wiringPiI2C function
 * wiringPiI2CWriteReg8	a wiringPiI2C function
 * wiringPiI2CRead()	a wiringPiI2C function
 * wiringPiI2CWrite()	a wiringPiI2C function
 * wiringPiSetup()		a wiringPi function
 * writeGPIO()	 		Write a string value to to gpio file
 * GPIOWrite()			Function to send output to control GPIO as defined 
 * pPosition[] 			position from joystick
 * getPosition()		get a pin number from the joystick arrange in a phone touch pad order
 * pPos[][]				An array to hold 9 positions of the joystick. Centerx, +x, -x, Centery, +y, -y and their combinations
 * GPIO_PATH's 			Several variables for the different file descriptors from the gpio pins.
 * i, j, k, 			short lived variables for loops.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <wiringPi.h> // Used for nunchuck
#include <wiringPiI2C.h> // Used for nunchuck part of wiringPi
#include <errno.h>
//#include "getNunchuckPosition.c" // Uncomment if using nunchuck, comment out if not using nunchuck
#include "getRandomPosition.c" // Uncomment if using random, comment out if not using random


/*pre load all functions*/
//int NUNCHUCK_DEVICE = 0x52; // This is found using line command $ sudo i2cdetect -y -r 1 
int getPosition(); // Get a  position from auxillary file
void writeGPIO(char filename[], char value[]); // Write a string value to to gpio file
int GPIOWrite(int pin, int value); // Function to send output to control GPIO as defined 
void setup();  // Use wiringPi.h to setup nunchuck device 



#define GPIO_PATH "/sys/class/gpio/gpio"  // paths to gpio folder
#define GPIO_SYSFS "/sys/class/gpio/"
#define IN  0
#define OUT 1
#define LOW  0
#define HIGH 1
/* Use GPIO pins for this project */
#define GPIO6 "6" /* PIN 31 GPIO6 */
#define GPIO12 "12" /* PIN 32 GPIO12 */
#define GPIO13 "13" /* PIN 33 GPIO13 */
#define GPIO19 "19" /* PIN 35 GPIO19 */
#define GPIO16 "16" /* PIN 36 GPIO116*/
#define GPIO26 "26" /* PIN  37 GPIO26 */
#define GPIO20 "20" /* PIN 38 GPIO20 */
#define GPIO21 "21" /* PIN 40 GPIO21 */

#define GPIO_SYSFS "/sys/class/gpio/"
#define GPIO_PATH06 "/sys/class/gpio/gpio6/"
#define GPIO_PATH12 "/sys/class/gpio/gpio12/"
#define GPIO_PATH13 "/sys/class/gpio/gpio13/"
#define GPIO_PATH19 "/sys/class/gpio/gpio19/"
#define GPIO_PATH16 "/sys/class/gpio/gpio16/"
#define GPIO_PATH26 "/sys/class/gpio/gpio26/"
#define GPIO_PATH20 "/sys/class/gpio/gpio20/"
#define GPIO_PATH21 "/sys/class/gpio/gpio21/"


int main(int argc, char **argv){

	setup(); // setup the nunchuck 

	/* Enable GPIOs  */
	writeGPIO(GPIO_SYSFS "export", GPIO6);
    usleep(100000);                 
    writeGPIO(GPIO_PATH06 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO12);
    usleep(100000);                  
    writeGPIO(GPIO_PATH12 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO13);
    usleep(100000);                 
    writeGPIO(GPIO_PATH13 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO19);
    usleep(100000);                  
    writeGPIO(GPIO_PATH19 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO16);
    usleep(100000);                  
    writeGPIO(GPIO_PATH16 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO26);
    usleep(100000); 
    writeGPIO(GPIO_PATH26 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO20);
    usleep(100000);
    writeGPIO(GPIO_PATH20 "direction", "out");
	writeGPIO(GPIO_SYSFS "export", GPIO21);
    usleep(100000);
    writeGPIO(GPIO_PATH21 "direction", "out");

	
	//  Turn all GPIOs on to turn off relays.
	int pPins[9] = {12,6,13,21,0,16,26,20,19}; 
	int i=0;
	for (i=0;i<9;i++){
		GPIOWrite(pPins[i] , 1);  // turn all outputs high to turn off the relays
	}
	while (1) { // infinite loop. Read the value to  select the GPIO.
		int	delay = 50000;
		int pos = getPosition(); // Get the position relay.
		if (pos == -1){
			printf("Error getting position");
			continue;
		}
		printf("position %d\n",pos);
		GPIOWrite(pPins[pos] , 0);
		usleep(50000); // Close relay for 50 ms
		GPIOWrite(pPins[pos] , 1);
		usleep(delay);
	}
}



// Write a string value to to gpio file
void writeGPIO(char filename[], char value[]){ 
	FILE* fd;                           // create a file pointer fp
	fd = fopen(filename, "w+");         // open file for writing
	fprintf(fd, "%s", value);           // send the value to the file
	fclose(fd);                         // close the file using fp
}
/* Function to send output to control GPIO as defined */
int GPIOWrite(int pin, int value){
	static const char s_values_str[] = "01";
	#define VALUE_MAX 35
	char path[VALUE_MAX];
	int fd;
 	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	write(fd, &s_values_str[LOW == value ? 0 : 1], 1);
	close(fd);
	return 0;
}
/* ignore this it's for my computer 
 * ssh pi@192.168.69.10 
 * ssh pi@10.42.0.160 
 * sudo shutdown -h now
 * gcc -g -Wall musicNunChuck03.c -lwiringPi
 * sudo gdb a.out
 * */
