#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

int GetFileSize(const char* file_name);

char* GetFileText(const char* file_name);

int CountTextStrings(const char* text);

void SetErrorBit(unsigned* error, int error_bit);

void UnsetErrorBit(unsigned* error, int error_bit);

size_t BordersRand(size_t left, size_t right);

const double EPSILON = 10e-15;

bool IsDoubleNumsEqual(double num1 , double num2);

#endif // UTILITIES_H