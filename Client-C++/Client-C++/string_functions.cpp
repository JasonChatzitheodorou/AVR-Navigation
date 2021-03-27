/*
 * string_functions.cpp
 *
 * Created: 3/7/2021 11:21:43
 *  Author: Jason
 */ 

int strcmp(const char *s1, const char *s2) {
	const unsigned char *p1 = (const unsigned char *)s1;
	const unsigned char *p2 = (const unsigned char *)s2;

	while (*p1 != '\0') {
		if (*p2 == '\0') return  1;
		if (*p2 > *p1)   return -1;
		if (*p1 > *p2)   return  1;

		p1++;
		p2++;
	}

	if (*p2 != '\0') return -1;

	return 0;
}

// Make sure to allocate enough size on dest
char *strcat(char *dest, const char *src)
{
	int i,j;
	for (i = 0; dest[i] != '\0'; i++);
	for (j = 0; src[j] != '\0'; j++) dest[i+j] = src[j];
	dest[i+j] = '\0';
	return dest;
}

// dest should be at least as long as src
char *strcpy(char *dest, char const *src){
	int i = 0;
	
	for(i = 0; src[i] != '\0'; i++) dest[i] = src[i];
	dest[i] = '\0';
	
	return dest;
}

// Int to string
int atoi(const char *c){
	int value = 0;
	int sign = 1;
	if( *c == '+' || *c == '-' )
	{
		if( *c == '-' ) sign = -1;
		c++;
	}
	while (*c >= '0' && *c <= '9')
	{
		value *= 10;
		value += (int) (*c-'0');
		c++;
	}
	return (value * sign);
}

// Yet, another good itoa implementation
// returns: the length of the number string
int itoa(int value, char *sp, int radix)
{
	char tmp[16];// be careful with the length of the buffer
	char *tp = tmp;
	int i;
	unsigned v;

	int sign = (radix == 10 && value < 0);
	if (sign)
	v = -value;
	else
	v = (unsigned)value;

	while (v || tp == tmp)
	{
		i = v % radix;
		v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
		if (i < 10)
		*tp++ = i+'0';
		else
		*tp++ = i + 'a' - 10;
	}

	int len = tp - tmp;

	if (sign)
	{
		*sp++ = '-';
		len++;
	}

	while (tp > tmp)
	*sp++ = *--tp;

	return len;
}