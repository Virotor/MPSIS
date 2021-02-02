#include "utils.h"

void reverse(char s[])
{
	char* end = s;
	while(*end)
		++end;
	--end;

	while(s < end)
	{
		char tmp = *end;
		*end = *s;
		*s = tmp;

		++s;
		--end;
	}
}

void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
	 n = -n;
	i = 0;
	do {
	 s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
	 s[i++] = '-';
	else
	    s[i++]='+';
	s[i] = '\0';
	reverse(s);
}
