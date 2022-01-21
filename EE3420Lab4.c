/* Author: Ringtarih Tamfu
 * 
 * Microprocessor Laboratory 3: Raspberry Pi 4 Code
 * 
 * Spring 2021 - EE 3420 - 001
 * 
 * Instructor: William Stapleton
 * 
 * Description: 
 */

//All Libraries used in this program
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pigpio.h>
#include "ee3420_pi.h"
#include "keypad4x4.h"
#include "character_lcd.h"

//Defining GPIO pins to be used with the Stepper motor
#define PIN1 17
#define PIN2 27
#define PIN3 22
#define PIN4 23
#define KEY 2

//State Functions
void PressedOne();
void PressedTwo();
void PressedThree();
void PressedA();
void PressedB();
void PressedC();
void PressedD();
void PressedE();
void PressedF();

//Varisables being used
time_t current_time;
time_t start_time;
time_t desired_duration;
char test_string[]="ONE:TWO:Three:Four:five:sIx:?\n";
char command_string[100];
char command_parameters[max_parameter_limit][max_parameter_length];
int delay = 25000;
int increasedDelay = 26000;
int decreasedDelay = 24000;

//Main Driver Function
int main()
{
	//The ee3420_startup() function from ee3420_pi.h should be the first function called
	if(ee3420_startup() <0)
	{ return(ee3420_shutdown()); }	//if unable to intialize completely, end the program
	
	//demonstrating how a command string is parsed using a sample string
	printf("\nParsing test string into parameters ...\n");
	printf("%s",test_string);
	
	command_parameter_counter=parse_command_string(test_string,command_parameters, parameter_separators);
	
	for(int i=0; i<command_parameter_counter; i++)
	{
		if(command_parameters[i][0]=='\0') {break;}
		printf("%s\n",&command_parameters[i][0]);
	}
	printf("\n");
	
	printf("Standardizing parameters to uppercase\n");
	
	make_command_parameters_uppercase(command_parameters);
		
	for(int i=0; i<command_parameter_counter; i++)
	{
		if(command_parameters[i][0]=='\0') {break;}
		printf("%s\n",&command_parameters[i][0]);
	}
	
	
	/////////////////////////////////////////////////////////////////////////
	//MAIN DRIVER CODE	
	/////////////////////////////////////////////////////////////////////////
	
	printf("\n\nThis program will control the stepper motor, keypad and LCD\n");
	
	//initializing the gpios for the stepper motor
	gpioSetMode(PIN1, PI_OUTPUT);
	gpioSetMode(PIN2, PI_OUTPUT);
	gpioSetMode(PIN3, PI_OUTPUT);
	gpioSetMode(PIN4, PI_OUTPUT);
	gpioSetMode(KEY, PI_INPUT);
	
	int run = 1;       //variable to keep the loop running
	
	while (run == 1)
	{
		sprintf(command_string, "KEYPAD:?\n");
		printf("Sending request ... %s",command_string);
		serial_send_request(serial_handle, command_string, serial_receive_buffer);
		printf("Response ...\n");
		printf("%s\n",serial_receive_buffer);
	
		//if(process_serial_response(serial_receive_buffer) < 0)
		//{
			//display_serial_device_error(serial_device.last_error_code);
		//}
		
		//This takes the char that was being returned by the command above and you can save it 
		//into a variable of your choice
		parse_command_string(serial_receive_buffer, serial_command_parameters, parameter_separators);
		make_command_parameters_uppercase(serial_command_parameters);
		
		char letter = serial_command_parameters[1][0];
		
		gpioDelay(250000);
		
		if (letter == '1')
		{
			PressedOne();
		}
		else if (letter == '2')
		{
			PressedTwo();
		}
		else if (letter == '3')
		{
			PressedThree();
		}
		else if (letter == 'A')
		{
			PressedA();
		}
		else if (letter == 'B')
		{
			PressedB();
		}
		else if (letter == 'C')
		{
			PressedC();
		}
		else if (letter == 'D')
		{
			PressedD();
		}
		else if (letter == '*')
		{
			PressedE();
		}
		else if (letter == '#')
		{
			PressedF();
		}
	}
	
	
	
	//Properly terminating everything
	gpioTerminate();
	return(ee3420_shutdown());
	return 0;
}

void PressedOne()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedTwo()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedThree()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedA()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedB()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CCW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedC()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(decreasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(decreasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(decreasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(decreasedDelay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", decreasedDelay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedD()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(increasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(increasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(increasedDelay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(increasedDelay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", increasedDelay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedE()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 1);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 1);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 1);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 1);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:CW\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

void PressedF()
{
	int loop = 1;
	
	while (loop == 1) {
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	gpioWrite(PIN1, 0);
	gpioWrite(PIN2, 0);
	gpioWrite(PIN3, 0);
	gpioWrite(PIN4, 0);
	gpioDelay(delay);
	
	sprintf(command_string, "LCD:TEXT:STOPPED\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:NEXT\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:TEXT:%5ims\n", delay);
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
	
	sprintf(command_string, "LCD:CURSOR:BEGIN\n");
	printf("Sending request ... %s",command_string);
	serial_send_request(serial_handle, command_string, serial_receive_buffer);
	printf("Response ...\n");
	printf("%s\n",serial_receive_buffer);
	
	if(process_serial_response(serial_receive_buffer) < 0)
	{
		display_serial_device_error(serial_device.last_error_code);
	}
}
}

