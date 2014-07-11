#include "mex.h"
#include "gazeapi.h"


#define WARN_FIELD(field) \
    mexWarnMsgIdAndTxt("EyeTribe:invalidStructure", "Field \"" field "\" not found in \"screen\" structure."); \
    return 1;


#define COPY_FIELD(field) \
    mxArray *field = mxGetField(array, 0, #field); \
    if(!field) WARN_FIELD(#field); \
    screen.field = *mxGetPr(field);  


/**
 * Create a matrix containing one double.
 * @param double Value of the double.
 */
mxArray* muCreateDouble(double value)
{
    mxArray *var = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(var) = value;
    return var;
}


/**
 * Create Point2D structure
 * @param point2d
 */
mxArray* muCreatePoint2D(gtl::Point2D point2d)
{
    // Create structure
    mwSize dims[2] = {1, 1};
    const char *point2d_fields[] = {"x", "y"};
    mxArray *array = mxCreateStructArray(2, dims, 2, point2d_fields);
    
    // Fill fields
    mxSetField(array, 0, "x", muCreateDouble(point2d.x));
    mxSetField(array, 0, "y", muCreateDouble(point2d.y));
    
    return array;
}


/**
 * Create Eye structure
 */
mxArray* muCreateEye(int neyes, gtl::Eye eye)
{
    // Setup structure
    mwSize dims[2] = {1, 1};
    const char *eye_fields[] = {"raw", "avg", "psize", "pcenter"};
    mxArray *array = mxCreateStructArray(2, dims, 4, eye_fields);
    
    // Fill fields
    mxSetField(array, 0, "raw",     muCreatePoint2D(eye.raw));
    mxSetField(array, 0, "avg",     muCreatePoint2D(eye.avg));
    mxSetField(array, 0, "psize",   muCreateDouble(eye.psize));
    mxSetField(array, 0, "pcenter", muCreatePoint2D(eye.pcenter));
    
    return array;
}


/**
 * Create GazeData structure
 */
mxArray *muCreateGazeData(gtl::GazeData gazeData)
{
    mwSize dims[2] = {1, 1};
    const char *field_names[] = {"time", "fix", "state", "raw", "avg", "lefteye", "righteye"};
    mxArray *array = mxCreateStructArray(2, dims, 7, field_names);
    
    mxSetField(array, 0, "time",     muCreateDouble(gazeData.time));
    mxSetField(array, 0, "fix",      muCreateDouble(gazeData.fix));
    mxSetField(array, 0, "state",    muCreateDouble(gazeData.state));
    mxSetField(array, 0, "raw",      muCreatePoint2D(gazeData.raw));
    mxSetField(array, 0, "avg",      muCreatePoint2D(gazeData.avg));
    mxSetField(array, 0, "lefteye",  muCreateEye(1, gazeData.lefteye));
    mxSetField(array, 0, "righteye", muCreateEye(1, gazeData.righteye));
    
    return array;
}




int muGetScreen(const mxArray *array, gtl::Screen &screen)
{
    COPY_FIELD(screenindex);
    COPY_FIELD(screenresw);
    COPY_FIELD(screenresh);
    COPY_FIELD(screenpsyw);
    COPY_FIELD(screenpsyh);
    
    return 0;
}


mxArray *muCreateScreen(gtl::Screen screen)
{
    const char *screen_fields[] = {"screenindex", "screenresw", "screenresh", "screenpsyw", "screenpsyh"};
    
    mwSize dims[2] = {1, 1};
    mxArray *array = mxCreateStructArray(2, dims, 5, screen_fields);
    
    mxSetFieldByNumber(array, 0, 0, muCreateDouble(screen.screenindex));
    mxSetFieldByNumber(array, 0, 1, muCreateDouble(screen.screenresw));
    mxSetFieldByNumber(array, 0, 2, muCreateDouble(screen.screenresh));
    mxSetFieldByNumber(array, 0, 3, muCreateDouble(screen.screenpsyw));
    mxSetFieldByNumber(array, 0, 4, muCreateDouble(screen.screenpsyh));
    
    return array;
}


/**
 * Create ServerState structure
 */
mxArray *muCreateServerstate(gtl::ServerState serverstate)
{
    const char *serverstate_fields[] =
    {"push", "heartbeatinterval", "version", "trackerstate",
     "framerate", "iscalibrated", "iscalibrating"};
     
     mwSize dims[2] = {1, 1};
     mxArray *array = mxCreateStructArray(2, dims, 7, serverstate_fields);
     
     mxSetFieldByNumber(array, 0, 0, muCreateDouble(serverstate.push));
     mxSetFieldByNumber(array, 0, 1, muCreateDouble(serverstate.heartbeatinterval));
     mxSetFieldByNumber(array, 0, 2, muCreateDouble(serverstate.version));
     mxSetFieldByNumber(array, 0, 3, muCreateDouble(serverstate.trackerstate));
     mxSetFieldByNumber(array, 0, 4, muCreateDouble(serverstate.framerate));
     mxSetFieldByNumber(array, 0, 5, muCreateDouble(serverstate.iscalibrated));
     mxSetFieldByNumber(array, 0, 6, muCreateDouble(serverstate.iscalibrating));
     
     return array;
}


mxArray* muCreateCalibpoint(int npoints)
{
    const char *calibpoints_fields[] = {"state", "cp", "mecp", "acd", "mepix", "asdp"};
    mwSize dims[2] = {1, npoints};
    return mxCreateStructArray(2, dims, 6, calibpoints_fields);
}


void muFillCalibpoint(mxArray *array, int i, gtl::CalibPoint calibpoint)
{
    mxSetField(array, i, "state", muCreateDouble(calibpoint.state));
    mxSetField(array, i, "cp",    muCreatePoint2D(calibpoint.cp));
    mxSetField(array, i, "mecp",  muCreatePoint2D(calibpoint.mecp));
    
    const char *acd_fields[]   = {"ad", "adl", "adr"};
    const char *mepix_fields[] = {"mep", "mepl", "mepr"};
    const char *asdp_fields[]  = {"asd", "asdl", "asdr"};
    
    mwSize dims[2] = {1, 1};
    
    mxArray *acd =  mxCreateStructArray(2, dims, 3, acd_fields);
    mxArray *mepix = mxCreateStructArray(2, dims, 3, acd_fields);
    mxArray *asdp =  mxCreateStructArray(2, dims, 3, acd_fields);
    
    mxSetFieldByNumber(acd,   0, 0, muCreateDouble(calibpoint.acd.ad));
    mxSetFieldByNumber(acd,   0, 1, muCreateDouble(calibpoint.acd.adl));
    mxSetFieldByNumber(acd,   0, 2, muCreateDouble(calibpoint.acd.adr));
    mxSetFieldByNumber(mepix, 0, 0, muCreateDouble(calibpoint.mepix.mep));
    mxSetFieldByNumber(mepix, 0, 1, muCreateDouble(calibpoint.mepix.mepl));
    mxSetFieldByNumber(mepix, 0, 2, muCreateDouble(calibpoint.mepix.mepr));
    mxSetFieldByNumber(asdp,  0, 0, muCreateDouble(calibpoint.asdp.asd));
    mxSetFieldByNumber(asdp,  0, 1, muCreateDouble(calibpoint.asdp.asdl));
    mxSetFieldByNumber(asdp,  0, 2, muCreateDouble(calibpoint.asdp.asdr));
    
    mxSetField(array, i, "acd", acd);
    mxSetField(array, i, "mepix", mepix);
    mxSetField(array, i, "asdp", asdp);
}


mxArray *muCreateCalibresult(gtl::CalibResult calibResult)
{
    const char *calibresult_fields[] = {"result", "deg", "degl", "degr", "calibpoints"};
    mwSize dims[2] = {1, 1};
    mxArray *array = mxCreateStructArray(2, dims, 5, calibresult_fields);
    
    mxSetField(array, 0, "results", muCreateDouble(calibResult.result));
    mxSetField(array, 0, "deg", muCreateDouble(calibResult.deg));
    mxSetField(array, 0, "degl", muCreateDouble(calibResult.degl));
    mxSetField(array, 0, "degr", muCreateDouble(calibResult.degr));
    
    int npoints = calibResult.calibpoints.size();
    mxArray *calibpoints = muCreateCalibpoint(npoints);
    
    for(int i = 0; i < npoints; i++)
        muFillCalibpoint(calibpoints, i, calibResult.calibpoints.at(i));
    
    return array;
}
