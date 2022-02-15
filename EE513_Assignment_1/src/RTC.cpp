/*
 * RTC.cpp
 *
 *  Created on: 8 Feb 2022
 *      Author: megswalsh
 */

#include <iostream> //imports streams needed
using namespace std;
#include<stdio.h> //The stdio.h header defines three variable types,several macros, and various functions for performing input and output.
#include<fcntl.h> // Construct for file control
#include<sys/ioctl.h> // standards for the Kernel
#include<unistd.h> //On Unix-like systems, unistd.h is typically made up largely of system call wrapper functions such as fork, pipe and I/O primitives (read, write, close, etc.).
#include<linux/i2c-dev.h> // bus interfaceioctl
#include<stdint.h>
#include<bitset>
#include<sstream>
#include<iomanip>
#include <time.h>
#include <system_error>
#include <ctime>
using namespace std;
#define BUFFER_SIZE 19  //allocates the memory for the OS  //0x00 to 0x12

class RTC {


protected:

	//states
	int addr;
	char buf[BUFFER_SIZE];
	char writeBuffer[1] = { 0x00 };

public:

	RTC(){


		addr=0x68; //address
	}

    //functions
	virtual int bcdToDec(char b) { return (b / 16) * 10 + (b % 16);}
	string display(uint8_t a) {
		stringstream ss;
		ss << setw(3) << (int) a << "(" << bitset<8>(a) << ")";
		return ss.str();
	}

	//get date AS-IS

	virtual void ReadDate_Time(){



	printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
			bcdToDec(buf[1]), bcdToDec(buf[0]));

	printf("The RTC date is %02d:%02d:%03d\n", bcdToDec(buf[4]),
			bcdToDec(buf[5]), bcdToDec(buf[6]));

	}

	virtual void Read_Temp(){

    int addrTemp=0x11;
    int addrTempLow=0x12;
	printf("Temperature is %2d.%2d degress \n",(buf[addrTemp]),(buf[addrTempLow]));
		}



	virtual void writeDate_Time(){


		   // current date/time based on current system
		   time_t now = time(0);

		   cout << "Number of sec since January 1,1970 is:: " << now << endl;

		   tm *ltm = localtime(&now);


		   // print various components of tm structure.
		   cout << "Year:" << 1900 + ltm->tm_year<<endl;
		   cout << "Month: "<< 1 + ltm->tm_mon<< endl;
		   cout << "Day: "<< ltm->tm_mday << endl;

	       buf[0] = ltm->tm_sec;//Seconds
	       buf[1] = ltm->tm_min; // Minutes
	       buf[2] = 5+ltm->tm_hour; //Hours
	       printf("The RTC current time is %02d:%02d:%03d\n", bcdToDec(buf[0]),
	       			bcdToDec(buf[1]), bcdToDec(buf[2]));
	       printf("Time amended on the RTC to current");

	       buf[] = ltm->tm_sec;//Seconds
	       buf[1] = ltm->tm_min; // Minutes
	      	       buf[2] = 5+ltm->tm_hour; //Hours
	      	       printf("The RTC current time is %02d:%02d:%03d\n", bcdToDec(buf[0]),
	      	       			bcdToDec(buf[1]), bcdToDec(buf[2]));
	      	       printf("Time amended on the RTC to current");
	}


};

// the time is in the registers in encoded decimal form

int main() {

    int file;
    RTC rtc;//create object of the class
    int addr=0x68;

	// creates a integer value File
	printf("Starting the DS3231 test application\n"); //messagae to the user


	if ((file = open("/dev/i2c-1", O_RDWR)) < 0) {
				perror("failed to open the bus\n");
			} else {
				printf("Opened the bus\n");
			}

	if (ioctl(file, I2C_SLAVE,addr) < 0) {
		perror("Failed to connect to the sensor\n");
		return 1;
	} else {
		printf("Connected to the DS3231 RTC Clock\n ");
	}

	char writeBuffer[1] = { 0x00 };
	if (write(file, writeBuffer, 1) != 1) {
		perror("Failed to reset the read address\n");
		return 1;
	}

	char buf[BUFFER_SIZE];
	if (read(file, buf, BUFFER_SIZE) != BUFFER_SIZE) {
		perror("Failed to read in the buffer\n");
		return 1;
	}


	rtc.ReadDate_Time(); // read current
	rtc.Read_Temp(); // Get temperature
	rtc.writeDate_Time();//set time and date


	close(file);
	return 0;
}
