/*
 * uart_functions.h
 *
 * Created: 3/2/2021 10:50:10
 *  Author: Jason
 */ 


#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

#include <string.h>
#include <stdlib.h>
#define CMD 50

/*---------------VERY LOW LEVEL-------------*/
// set_value is used to choose whether we will pass a value
void print_cmd(char *dest, char const *command, int debug = 0, char const *s = "", char const *value = "", int set_value = 0){
	if(!debug) strcat(dest, "ESP:");
	
	strcat(dest, command);
	
	if(strcmp(s, "")){
		strcat(dest, ":");
		strcat(dest, "\"");
		strcat(dest, s);
		strcat(dest, "\"");
		
		if(set_value){
			strcat(dest, "[");
			strcat(dest, value);
			strcat(dest, "]");
		}
	}
	
	strcat(dest, "\n");
}

void usart_init(){
	UCSRA = 0;
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UBRRH = 0;
	UBRRL = 51;
	UCSRC = (1 << URSEL) | (3 << UCSZ0);
}

void usart_transmit(char c){
	while((UCSRA & (1 << UDRE)) == 0) ;
	UDR = c;
}

char usart_receive(){
	char ans;
	while((UCSRA & (1 << RXC)) == 0) ;
	ans = UDR;

	return ans;
}

/*------------------LOW LEVEL I/O-----------*/
void transmitString(char const *s){
	int i = 0;
	while(s[i] != '\n'){
		usart_transmit(s[i++]);
	}
	
	usart_transmit('\n');
}

void receiveString(char *source){
	int i = 0;
	char c;

	c = usart_receive();
	while(c != '\n'){
		source[i++] = c;
		c = usart_receive();
	}
	
	source[i] = '\n';
	
	transmitString(source);
}

// Checks whether the message received is Success
int checkSuccess(){
	char cmd[CMD] = "";
	receiveString(cmd);
	
	return strcmp(cmd, "\"Success\"\n");
}

void parseSensorString(char *dest){
	char cmd[CMD] = "";
	receiveString(cmd);
	
	int i = 0, j = 0;
	strcpy(dest, "");
	
	while(cmd[i++] != '"');
	while(cmd[i] != '"'){
		dest[j++] = cmd[i++];
	}
	dest[j] = '\n';
}

// Returns the number from the string <Sensor1:"123">
int parseSensor(){
	char cmd[CMD] = "";
	receiveString(cmd);
	
	int i = 0, j = 0;
	char buf[CMD] = "";
	
	while(cmd[i++] != '"');
	while(cmd[i] != '"'){
		buf[j++] = cmd[i++];
	}
	
	int ans = atoi(buf);
	return ans;
}

/*-------------------FUNCTIONS--------------*/
// Restarts the ESP
int restart(int debug = 0){
	// Needs a new line in the beginning
	transmitString("\n");
	
	char cmd[CMD] = "";

	print_cmd(cmd, "restart", debug);
	transmitString(cmd);
	
	// Read the answer of restart
	char out[CMD] = "";
	receiveString(out);
	receiveString(out);

	return 0;
}

// Sets default ssid that will be used by device
int setSSID(char const *ssid, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "ssid", debug, ssid);
	transmitString(cmd);

	return checkSuccess();
}

// Sets default password that will be used by device
int setPassword(char const *password, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "password", debug, password);
	transmitString(cmd);

	return checkSuccess();
}

// Initiates functioning as access point (server)
int startAP(int debug = 0){
	char cmd[CMD] = "";

	print_cmd(cmd, "APStart", debug);
	transmitString(cmd);

	return checkSuccess();
}

// Creates an access point with given ssid, password
int createServer(char const *ssid, char const *password, int debug = 0){
	return setSSID(ssid, debug) || setPassword(password, debug) || startAP(debug);
}

// Connects to access point (client)
int connect(int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "connect", debug);
	transmitString(cmd);

	return checkSuccess();
}

// Connects to server using given ssid, password
int connectToServer(char const *ssid, char const *password, int debug = 0){
	return setSSID(ssid, debug) || setPassword(password, debug) || connect(debug);
}

// Adds a new sensor that will be handled by the device
int newSensor(char const *sensor, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "addSensor", debug, sensor);
	transmitString(cmd);

	return checkSuccess();
}

// Sets the value of a sensor (string)
int setSensorValue(char const *sensor, char const *value, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "sensorValue", debug, sensor, value, 1);
	transmitString(cmd);
	
	return checkSuccess();
}

// Sets the value of a sensor (int)
int setSensorInt(char const *sensor, int value, int debug = 0){
	char v[CMD] = "";
	itoa(value, v, 10);
	
	return setSensorValue(sensor, v, debug);
}

// Sets the value of a sensor to request,
// so that it will request a value from the server on transmit
int requestSensorValue(char const *sensor, int debug = 0){
	return setSensorValue(sensor, "request", debug);
}

void getSensorString(char const *sensor, char *dest, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "getValue", debug, sensor);
	transmitString(cmd);

	parseSensorString(dest);
}

// Gets the value of given sensor
int getSensorValue(char const *sensor, int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "getValue", debug, sensor);
	transmitString(cmd);

	return parseSensor();
}

//getAllSensorValues

// Send all sensor values/requests to connected devices
int clientTransmit(int debug = 0){
	char cmd[CMD] = "";
	
	print_cmd(cmd, "clientTransmit", debug);
	transmitString(cmd);

	return checkSuccess();
}


#endif /* UART_FUNCTIONS_H_ */