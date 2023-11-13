#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>

int GetFileSize(const char* file_name);

char* GetFileText(const char* file_name);

int CountTextStrings(const char* text);

size_t BordersRand(size_t left, size_t right);

const double EPSILON = 10e-15;
bool IsDoubleNumsEqual(double num1 , double num2);

void SetErrorBit(unsigned* error, int error_bit);

void UnsetErrorBit(unsigned* error, int error_bit);

double StrToNum(const char* str, size_t str_len, bool* is_str_num);

#endif // UTILITIES_H