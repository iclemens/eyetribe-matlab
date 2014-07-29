eyetribe-matlab
===============

The EyeTribe (https://theeyetribe.com) is a cheap 60Hz gaze tracker with a JSON-based API.
The vendor provides an easy to use C++ library to access that API, but to this date (July 2014)
there is no official Matlab support.

Our goal is to develop a Mex wrapper such that the C++ library can be used in Matlab.

Installation is easy, provided you have installed a C-compiler that is compatible with Matlab (e.g. xcode) *boost*:
-Clone this GIT repository, or download and extract the ZIP file.
-Go to the base directory and run install.m

The Mex file should now have been built and is ready for use.
