/*
 * lcd_functions.h
 *
 * Created: 3/3/2021 15:33:40
 *  Author: Jason
 */ 


#ifndef LCD_FUNCTIONS_H_
#define LCD_FUNCTIONS_H_

extern "C" void lcd_init_sim();
extern "C" void lcd_data_sim(uint8_t byte);
extern "C" void lcd_command_sim(uint8_t cmd);
extern "C" void wait_msec(double ms);

#define lcd_clear() lcd_command_sim(0x01)

// Prints a string to LCD without \n in the end
void lcd_print_string(char const *s) {
	int i = 0;
	lcd_clear();
	
	while (s[i] != '\n' && s[i] != '\0') {
		lcd_data_sim(s[i++]);
	}
	
	return;
}

// Prints an integer of at most 7 digits (+-1)
void lcd_print_int(int n){
	char s[10] = "";
	itoa(n, s, 10);
	lcd_print_string(s);
}

// Captures PORTD and initializes LCD
void lcd_init(void) {
	DDRD = 0xFF;
	lcd_init_sim();
	return;
}

#endif /* LCD_FUNCTIONS_H_ */