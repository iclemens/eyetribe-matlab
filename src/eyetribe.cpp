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
 * eyetribe('connect', SERVER, PORT);
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
 * eyetribe('disconnect');
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
 * connected = eyetribe('is_connected');
 */
void is_connected(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(gaze_api->is_connected()) {
        // Return 1
    } else {
        // Return 0
    }
}


/**
 * eyetribe('set_screen', struct(...));
 *
 */
void set_screen(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // prhs[0] -> screen
    // gaze_api->set_screen(screen);
}


/**
 * screen = eyetribe('get_screen');
 */
void get_screen(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * frame = eyetribe('get_frame');
 */
void get_frame(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * calibresult = eyetribe('get_calib_result');
 */
void get_calib_result(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}

/**
 * serverstate = eyetribe('get_server_state');
 */
void get_server_state(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * status = eyetribe('calibration_start', npoints);
 */
void calibration_start(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * eyetribe('calibration_clear');
 */
void calibration_clear(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * eyetribe('calibration_abort');
 */
void calibration_abort(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/**
 * eyetribe('calibration_point_start', x, y);
 */
void calibration_point_start(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}

/**
 * eyetribe('calibration_point_end');
 */
void calibration_point_end(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
}


/** 
 * List of commands 
 */
struct {
    int need_lock;
    const char *command;
    void(*func)(int, mxArray**, int, const mxArray**);
} commands[] = {
    {0, "connect",      connect},
    {1, "disconnect",   disconnect},
    {1, "is_connected", is_connected},
    {1, "set_screen",   set_screen},
    {1, "get_screen",   get_screen},
    {1, "get_frame",    get_frame},
    {1, "get_calib_result",  get_calib_result},
    {1, "get_server_state",  get_server_state},
    {1, "calibration_start", calibration_start},
    {1, "calibration_clear", calibration_clear},
    {1, "calibration_abort", calibration_abort},
    {1, "calibration_point_start", calibration_point_start},
    {1, "calibration_point_end",   calibration_point_end},
    {NULL, NULL}
    };


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
    
    // Find command to execute
    int i = 0;
    while(commands[i].command != NULL) {
        if(strcmp(function, commands[i].command) == 0)
            break;
    }    
    mxFree(function);
    
    // If a command was found, run it
    if(commands[i].command != NULL) {
        // Check lock status
        if(commands[i].need_lock && !mexIsLocked()) {
            mexWarnMsgIdAndTxt("EyeTribe:notConnected", "Please connect EyeTribe before calling this function.");
            return;
        }
        
        if(~commands[i].need_lock && mexIsLocked()) {
            mexWarnMsgIdAndTxt("EyeTribe:connected", "Please disconnect EyeTribe before calling this function.");
            return;
        }

        // Invoke function
        commands[i].func(nlhs, plhs, nrhs, prhs);
    }
}
