/*
 * string_functions.h
 *
 * Created: 3/7/2021 11:33:46
 *  Author: Jason
 */ 


#ifndef STRING_FUNCTIONS_H_
#define STRING_FUNCTIONS_H_

int strcmp(const char*, const char*);

char *strcat(char *, const char *);

char *strcpy(char *,char const*);

int atoi(const char *s);

int itoa(int val, char *s, int radix);

#endif /* STRING_FUNCTIONS_H_ */