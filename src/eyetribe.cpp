#include "mex.h"
#include "gazeapi.h"
#include "utility.h"


/**
 * Global variable containing an instance of the API class.
 * It is constructed on eyetribe('connect') and
 * destructed on eyetribe('remove').
 */
gtl::GazeApi *gaze_api;


/**
 * Connects to the eye tribe system. Should be invoked using:
 * eyetribe('connect', SERVER, PORT)
 *
 * Due to a bug SERVER is currently ignored.
 */
void connect(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nrhs < 3) {
        mexWarnMsgIdAndTxt("EyeTribe:argumentCount", "EyeTribe('connect') requires at least three arguments.");
        return;
    }

    if(mexIsLocked() == 1) {
        mexWarnMsgIdAndTxt("EyeTribe:alreadyConnected", "Already connected, disconnect first.");
        return;
    }
    
    mexLock();
    gaze_api = NULL;
    
    // Extract server name from first argument
    char *server = arrayToString(prhs[1]);
    
    if(!server) {
        mexWarnMsgIdAndTxt("EyeTribe:argumentError", "Could not extract server name.");        
        mexUnlock();
        return;        
    }
    
    // Extract port number from second argument
    int port;
    
    if(arrayToInteger(prhs[2], &port) != 0) {
        mexWarnMsgIdAndTxt("EyeTribe:argumentError", "Could not extract port number.");
        mxFree(server);
        mexUnlock();
        return;
    }
    
    // Connect
    try {        
        mexPrintf("Establishing connection with %s:%d.\n", server, port);
        gaze_api = new gtl::GazeApi();
        gaze_api->connect(false, port);     // FIXME: Need to change c++ client
        mexPrintf("Connection established.\n");
    } catch(...) {
        mexWarnMsgIdAndTxt("EyeTribe:connect", "Connection error...");
        if(gaze_api) {
            delete gaze_api;
            gaze_api = NULL;
        }
        mexUnlock();
    }
    
    mxFree(server);
}


/**
 * Disconnects from the eye tribe system. Should be invoked as:
 * eyetribe('disconnect')
 */

void disconnect(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    try {
        delete gaze_api;
        gaze_api = NULL;
    } catch(...) {
        mexPrintf("Destructor caused an exception...\n");
    }
    
    mexUnlock();
}


/**
 * eyetribe('command', arg1, ..., argn)
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nrhs < 1) {
        mexWarnMsgIdAndTxt("EyeTribe:argumentCount", "EyeTribe() requires at least one argument.");
        return ;
    }
    
    char *function = arrayToString(prhs[0]);
    
    if(function == NULL) {
        mexWarnMsgIdAndTxt("EyeTribe:argumentError", "Could not parse first arugment.");
        return;
    }
    
    if(mexIsLocked() == 0) {
        if(strncmp(function, "connect", 7) == 0) {
            connect(nlhs, plhs, nrhs, prhs);
        } else {    
            mexWarnMsgIdAndTxt("EyeTribe:notConnected", "The EyeTribe is not connected.");
        }
    } else {
        if(strncmp(function, "get_frame", 9) == 0) {
            //etGetFrame(nlhs, plhs, nrhs, prhs);
        } else if(strncmp(function, "disconnect", 10) == 0) {
            disconnect(nlhs, plhs, nrhs, prhs);
        } else {
            mexWarnMsgIdAndTxt("EyeTribe:invallidCommand", "Invalid command");
        }
    }
    
    mxFree(function);
}
