#include <assert.h>

void SetError(unsigned* error, int error_bit)
{
	assert(error != NULL);
	*error |= error_bit;
}

void UnsetError(unsigned* error, int error_bit)
{
	assert(error != NULL);
	*error &= ~error_bit;
}