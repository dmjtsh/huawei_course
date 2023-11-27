#ifndef AKINATOR_ERRORS_H
#define AKINATOR_ERRORS_H

enum AkinatorErrorBits
{
	AKINATOR_PTR_NULL        = 1 << 0,
	AKINATOR_TREE_ERROR      = 1 << 1,
	AKINATOR_LOGGER_ERROR    = 1 << 2,
	AKINATOR_GRAPH_ERROR     = 1 << 3,
	AKINATOR_TEXT_INFO_ERROR = 1 << 4,
	PROPERTY_STACK_ERROR     = 1 << 5,
	AKINATOR_DELETED         = 1 << 6
};

void AkinatorDump(Akinator* akinator, FILE* logger);

unsigned AkinatorVerifier(Akinator* akinator);

#endif // AKINATOR_ERRORS_H