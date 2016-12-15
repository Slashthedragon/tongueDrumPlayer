/*
 * getNunchuckPosition.c
 * 
 * Copyright 2016 denis <denis@denis-ThinkPad>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
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
int NUNCHUCK_DEVICE = 0x52; // This is found using line command $ sudo i2cdetect -y -r 1 
// Use wiringPi.h to setup nunchuck device  
void setup(){
    wiringPiSetup();
    int nunfd = wiringPiI2CSetup(NUNCHUCK_DEVICE);
    if (nunfd < 0) {
        printf("Error setting up I2C: %d\n", errno);
        exit(0);
    }
    wiringPiI2CWriteReg8(nunfd, 0x40, 0x00);
    delayMicroseconds(500);

    
}   
// Get a position from the nunchuck
int getPosition(){
	int fd = wiringPiI2CSetup(NUNCHUCK_DEVICE); // a wiringPiI2C function
	int pPosition[3];
    int i=0;
    int j=0;
    int k=0;
    char pos=0;
    //set a range for the joystick away from center.
    int hiJoy = 0x90; 
    int loJoy = 0x50;
    
    int pPos[3][3] = {{7,4,1},{8,5,2},{9,6,3}}; // These are the 9 positions from the joystick in a phone dial pad order.
	wiringPiI2CWrite(fd, 0x00); // a wiringPiI2C function
    delayMicroseconds(50000);
    for (i=0; i<4; i++) { // Geting the y and x values
		pPosition[i] = wiringPiI2CRead(fd);  // a wiringPiI2C function
		delayMicroseconds(100000);  // added to reduce errors
		if (pPosition[i]== -1){ // look for errors getting nunchuck positions
			printf("error restarting \n"); //report errors
			i =0;  // if an error happens while getting values start again
		}
		// Restart for loop if a bad position from the joystick 
		if (pPosition[0]== 255 || pPosition[1]== 255 ){ // look for errors getting nunchuck positions
			printf("over range y %d\tx %d\t\n",pPosition[0],pPosition[1]); //report errors
			i =0;  // if an error happens while getting values start again
			
		}
    }
    printf("y %d\tx %d\t\n",pPosition[0],pPosition[1]);
    
	j = 0; 
	k = 0;
	
	if (pPosition[0] < loJoy) j = 0;
	if (pPosition[0] > hiJoy) j = 2;
	if (pPosition[0] > loJoy && pPosition[0] < hiJoy) j = 1;
	if (pPosition[1] < loJoy) k = 0;
	if (pPosition[1] > hiJoy) k = 2;
	if (pPosition[1] > loJoy && pPosition[1] < hiJoy) k = 1;
	pos = pPos[j][k];
	delayMicroseconds(50000);
	close(fd);
	return pos;
}

