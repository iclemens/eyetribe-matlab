#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "mex.h"

char *arrayToString(const mxArray *str);
int arrayToInteger(const mxArray *array, int* out);

#endif
