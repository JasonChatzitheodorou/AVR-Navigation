/*
 * Client-C++.cpp
 *
 * Created: 3/4/2021 11:03:36
 * Author : Jason
 */ 

#define ESP_DEBUG 0			// 1 is for testing only
							// 0 uses ESP
#define CMD 50
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "string_functions.h"
#include "uart_functions.h"
#include "lcd_functions.h"

int setupConnection(int debug = 0) {
	if(connectToServer("jasonAP", "awesomePassword12345", debug)) {
		lcd_print_string("Couldn't connect");
		return 1;
	}
	
	if(newSensor("direction", debug)) {
		lcd_print_string("Couldn't create direction");
		return 1;
	}
	
	if(newSensor("clientAck", debug)) {
		lcd_print_string("Couldn't create clientAck");
		return 1;
	}
	
	if(newSensor("x_end", debug)) {
		lcd_print_string("Couldn't create x_end");
		return 1;
	}
	
	if(newSensor("y_end", debug)) {
		lcd_print_string("Couldn't create y_end");
		return 1;
	}
	
	if(newSensor("x_start", debug)) {
		lcd_print_string("Couldn't create x_start");
		return 1;
	}
	
	if(newSensor("y_start", debug)) {
		lcd_print_string("Couldn't create y_start");
		return 1;
	}
	
	return 0;
}

/* 
 * 0 - Down
 * 1 - Left
 * 2 - Up
 * 3 - Right
*/
void lcdPrintDirection(int prev, int curr){
	if(prev == curr) lcd_print_string("Continue straight\n");
	else if((prev + 1) % 4 == curr) lcd_print_string("Turn right\n");
	else if((curr + 1) % 4 == prev) lcd_print_string("Turn left\n");
	else lcd_print_string("Turn back\n");
}

// Prepares ESP sensors to receive direction
void resetSensorsDirection(int debug = 0){
	if(requestSensorValue("direction", debug)) lcd_print_string("Couldn't set direction");

	if(requestSensorValue("clientAck", debug)) lcd_print_string("Couldn't set clientAck\n");
}

// Prepares ESP sensors to send coordinates of start and destination
void resetSensorsCoordinates(int x_start, int y_start, int x_end, int y_end, int debug = 0){
	if(setSensorInt("x_start", x_start, debug)) lcd_print_string("Couldn't set x_start\n");
	if(setSensorInt("y_start", y_start, debug)) lcd_print_string("Couldn't set y_start\n");
	
	if(setSensorInt("x_end", x_end, debug)) lcd_print_string("Couldn't set x_end\n");
	if(setSensorInt("y_end", y_end, debug)) lcd_print_string("Couldn't set y_end\n");
	
	if(requestSensorValue("clientAck", debug)) lcd_print_string("Couldn't set clientAck\n");
}

// Sends user's coordinates of start and destination
int sendCoordinates(int x_start, int y_start, int x_end, int y_end, int debug = 0){
	char ack[CMD] = "";
	while(1){
		resetSensorsCoordinates(x_start, y_start, x_end, y_end, debug);
		
		// Delay so that the server will have enough time to send
		_delay_ms(200);
		
		if(clientTransmit(debug)) lcd_print_string("Couldn't transmit destination\n");
		
		getSensorString("clientAck", ack, debug);
		lcd_print_string(ack);
		
		// Check whether the message was received by the server
		if(!strcmp(ack, "request\n")){
			_delay_ms(500);
			continue;
		}
		
		else break;
	}
	
	return 0;
}

// Reads directions from the server
// Returns the direction {0, 1, 2, 3} or -1 if we reached the destination
int readDirections(int prev_direction, int debug = 0){
	while(1){
		resetSensorsDirection(debug);
		
		// Delay so that the server will have enough time to send
		_delay_ms(200);
		
		if(clientTransmit(debug)) lcd_print_string("Couldn't transmit\n");
		
		char ack[CMD] = "";
		getSensorString("clientAck", ack, debug);
		lcd_print_string(ack);
		
		int direction = getSensorValue("direction", debug);
		lcd_print_int(direction);
		
		// Check whether the direction was received
		if(direction == -1 || !strcmp(ack, "request\n")) {
			_delay_ms(1000);
			continue;
		}
		
		else if(direction == 4){
			lcd_print_string("Finished!\n");
			return -1;
		}
		
		else if(direction == 5){
			lcd_print_string("Impossible destination\n");
			return -1;
		}
		
		else{
			lcdPrintDirection(prev_direction, direction);
			
			return direction;
		}
	}
}

int main(void)
{
    DDRB = 0;
    usart_init();
    lcd_init();
	lcd_print_string("Starting ...\n");
	
	int curr_direction, prev_direction;
	int x_end, y_end;
	int x_start, y_start;
	
	restart(ESP_DEBUG);

    /* Replace with your application code */
    while (1)
    {
		x_start = y_start = 0;
		x_end = 6;
		y_end = 1;
		curr_direction = prev_direction = 0; // Assume that we always start walking heading down the grid
	    
		// Read keyboard for destination
		//lcd_print_string("Give x of destination with #\n");
		//x_end = readKeyboardInt(delimeter);
		
		//lcd_print_string("Give y of destination with #\n");
		//y_end = readKeyboardInt(delimeter);
		
		// Connect to server
		lcd_print_string("Connecting to server ...\n");
		while(setupConnection(ESP_DEBUG)) transmitString("Retry to connect\n");
		
		// Send destination to server
		sendCoordinates(x_start, y_start, x_end, y_end, ESP_DEBUG);
		
		// Prompt to start
	    lcd_print_string("Press B0 to continue ...\n");
	    while(!PINB) _delay_ms(2);
		while(PINB) _delay_ms(2);
		
		// Keep reading directions until we finish
		while(curr_direction != -1){
			prev_direction = curr_direction;
			
			lcd_print_string("Waiting for direction ...\n");
			curr_direction = readDirections(prev_direction, ESP_DEBUG);
			
			while(!PINB) _delay_ms(2);
			while(PINB) _delay_ms(2);
			
			_delay_ms(500);
		}
    }
}

