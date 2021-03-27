/*
 * uart_functions.h
 *
 * Created: 3/2/2021 10:50:10
 *  Author: Jason
 */ 


#ifndef UART_FUNCTIONS_H_
#define UART_FUNCTIONS_H_

void usart_init();
void usart_transmit(char c);
char usart_receive();

void transmitString(char const *s);
void receiveString(char *source);

int restart(int debug /* = 0 */);
int createServer(char const *ssid, char const *password, int debug /* = 0 */);
int connectToServer(char const *ssid, char const *password, int debug /* = 0 */);
int newSensor(char const *sensor, int debug /* = 0 */);
int setSensorValue(char const *sensor, char const *value, int debug /* = 0 */);
int setSensorInt(char const *sensor, int value, int debug /* = 0 */);
int requestSensorValue(char const *sensor, int debug /* = 0 */);
int getSensorString(char const *sensor, char *dest, int debug /* = 0 */);
int getSensorValue(char const *sensor, int debug /* = 0 */);
int clientTransmit(int debug /* = 0 */);

#endif /* UART_FUNCTIONS_H_ */