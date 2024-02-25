#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char s[1000] = {};
int p = 0;

void syn_assert(bool cond)
{
	if(!cond)
	{
		printf("pizdec");
		abort();
	}
}

int GetN()
{
	int val = 0;
	if ( '0' <= s[p] && s[p] <= '9')
		val = s[p] - '0';
	p++;
	
	return val;
}

int GetE();

int GetP()
{
	int val = 0;
	if(s[p] == '(')
	{
		p++;
		val = GetE();
		syn_assert(s[p] == ')');
		p++;
		return val;
	}
	else
		return GetN();
}

int GetT()
{
	int val1 = GetP();
	int p_old = p;
	while(s[p] == '*' || s[p] == '/')
	{
		p++;
		int val2 = GetP();
		switch(s[p_old])
		{
		case '*': val1 *= val2; break;
		case '/': val1 /= val2; break;

		default: syn_assert(0);
		}
	}

	return val1;
}

int GetE()
{
	int val1 = GetT();
	int p_old = p;
	while(s[p] == '+' || s[p] == '-')
	{
		p++;
		int val2 = GetT();
		switch(s[p_old])
		{
		case '+': val1 += val2; break;
		case '-': val1 -= val2; break;

		default: syn_assert(0);
		}
	}

	return val1;
}

int GetG(const char* str)
{
	strcpy_s(s, str);
	int val = GetE();
	syn_assert(s[p] == '\0');
	
	return val;
}