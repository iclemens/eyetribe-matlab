#include "utility.h"

/**
 * Converts an array into a C-type string. If conversion fails,
 * it will print a warning and return NULL;
 *
 * Note that the returned string needs to be mxFree-ed later!
 */
char *arrayToString(const mxArray *str)
{
    char *buffer;
    int buflen = mxGetM(str) * mxGetN(str) + 1;
    
    buffer = (char *) mxCalloc(buflen, sizeof(char));
    
    if(buffer == NULL) {
        mexWarnMsgIdAndTxt("EyeTribe:mallocFailed", "Could not allocate string buffer");
        return NULL;
    }
    
    if(mxGetString(str, buffer, buflen) != 0) {
        mexWarnMsgIdAndTxt("EyeTribe:getString", "Could not get string");
        mxFree(buffer);
        return NULL;
    }
    
    return buffer;
}


/**
 * Converts an array into an integer.
 */
int arrayToInteger(const mxArray *array, int* out)
{
    if(mxIsNumeric(array)) {
        *out = int(*mxGetPr(array));
        return 0;
    }
    
    char *string = arrayToString(array);
    
    if(!string)
        return 1;
    
    *out = atoi(string);
    mxFree(string);
    return 0;
}  
