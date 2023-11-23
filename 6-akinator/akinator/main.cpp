#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "akinator.h"

int main()
{ 
	Akinator akinator = {};
	AkinatorCtor(&akinator);

	AkinatorPerformGame(&akinator);

	AkinatorDtor(&akinator);
	return 0;
}