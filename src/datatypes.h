#ifndef __DATATYPES_H__
#define __DATATYPES_H__

mxArray* muCreateDouble(double value);
mxArray* muCreateGazeData(gtl::GazeData gazeData);
mxArray *muCreateServerstate(gtl::ServerState serverstate);
mxArray *muCreateScreen(gtl::Screen screen);
mxArray *muCreateCalibresult(gtl::CalibResult calibResult);

int muGetScreen(const mxArray *array, gtl::Screen &screen);  

#endif
