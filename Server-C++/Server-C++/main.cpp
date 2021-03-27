/*
 * Server.c
 *
 * Created: 3/1/2021 10:53:56
 * Author : Jason
 */ 

#define ESP_DEBUG 0	// 1 for testing
					// 0 to use ESP
#define CMD 50
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "uart_functions.h"
#include "lcd_functions.h"
#include "labyrinth.h"

// This is the array that is filled by BFS_start in labyrinth with the directions
extern int queue[q_size];

/*---------------------------------------------*/

int setupConnection(int debug = 0) {
	if(createServer("jasonAP", "awesomePassword12345", debug)) {
		lcd_print_string("Couldn't create server");
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

void resetSensorsDirections(int direction, int debug = 0){
	if(setSensorInt("direction", direction, debug)) lcd_print_string("Couldn't set direction");
	
	if(setSensorInt("clientAck", 1, debug)) lcd_print_string("Couldn't set clientAck\n");
}

void resetSensorsCoordinates(int debug = 0){
	if(setSensorInt("x_start", -1, debug)) lcd_print_string("Couldn't set x_start");
	if(setSensorInt("y_start", -1, debug)) lcd_print_string("Couldn't set y_start");
	
	if(setSensorInt("x_end", -1, debug)) lcd_print_string("Couldn't set x_end");
	if(setSensorInt("y_end", -1, debug)) lcd_print_string("Couldn't set y_end");	
	
	if(setSensorInt("clientAck", 1, debug)) lcd_print_string("Couldn't set clientAck\n");

}

void waitForClient(){
	char s[CMD] = "";
	
	// Scan for client response every 2 sec
	while(1){
		transmitString("Waiting for ServedClient\n");
		
		receiveString(s);
		if(!strcmp(s, "ServedClient\n")) return;
	}
}

void sendDirections(int direction, int debug = 0){
	// Impossible destination
	if(direction == -1) direction = 5;
	
	// Refresh server sensors
	resetSensorsDirections(direction, debug);
	
	// Wait for client's response
	waitForClient();
	lcd_print_string("Sent\n");
}

void getCoordinates(int &x_start, int &y_start, int &x_end, int &y_end, int debug = 0){	
	// Reset server sensors
	resetSensorsCoordinates(debug);
	
	// Wait for client's response
	waitForClient();
	lcd_print_string("Received destination\n");
	
	// Get starting location
	x_start = getSensorValue("x_start", debug);
	y_start = getSensorValue("y_start", debug);
	
	// Get destination
	x_end = getSensorValue("x_end", debug);
	y_end = getSensorValue("y_end", debug);
}

int main(void)
{	
	int index;
	int x_end, y_end;
	int x_start, y_start;
	DDRC = 0;
	DDRB = 0xff;
	usart_init();
	lcd_init();
	restart(ESP_DEBUG);
	
	// Start the server
	lcd_print_string("Starting server ...\n");
	while(setupConnection(ESP_DEBUG)){
		_delay_ms(500);
	}
	
	/* Replace with your application code */
	while (1)
	{
		//resetSensorsDestination(ESP_DEBUG);
		
		x_start = y_start = 0;
		PORTB = 0;
		
		//lcd_print_string("Waiting for client ...\n");
		//waitForClient();
		//lcd_print_string("Client connected!\n");
		
		// ??? Get position of client ???
		
		// Get destination of client
		getCoordinates(x_start, y_start, x_end, y_end, ESP_DEBUG);
		
		// Get starting index
		index = BFS_start(x_start, y_start, x_end, y_end);
		lcd_print_string("Ready to send directions ...\n");
		
		// Send directions one by one
		while(index < q_size){
			lcd_print_int(queue[index]);
			sendDirections(queue[index], ESP_DEBUG);
		
			index++;
			
			// If there are no more directions send 4 to finish
			while(index == q_size){
				lcd_print_int(4);
				sendDirections(4, ESP_DEBUG);
				
				index++;
			}
		}
		
		_delay_ms(500);
	}
}



