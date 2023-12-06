#include "akinator.h"

int main()
{
	const char* dimas = "loh";
	Akinator akinator = {};
	AkinatorCtor(&akinator);
	
	TreeGraphPrint(&akinator.tree, akinator.graph);
	
	AkinatorPerformGame(&akinator);

	AkinatorDtor(&akinator);
	return 0;
}