
cmake_exec = '/usr/local/bin/cmake';

% Create directory for TET client
if ~exist('vendor', 'dir'); mkdir('vendor'); end;

% Download C++ client
target = 'vendor/master.zip';

if ~exist(target, 'file')
    disp 'Downloading C++ client';
    url = 'https://github.com/EyeTribe/tet-cpp-client/archive/master.zip';
    urlwrite(url, target);
end

% Unpack C++ client
if ~exist('vendor/tet-cpp-client-master', 'dir')
    disp 'Unpacking C++ client';
    unzip(target, 'vendor');

    cd 'vendor/tet-cpp-client-master';
    system 'patch -p1 < ../../tribe.patch';
    cd '../..';
end

% Building C++ client
gaze_lib = 'vendor/tet-cpp-build/libGazeApiLib.a';
build_dir = 'vendor/tet-cpp-build';

if ~exist(gaze_lib, 'file')
    if ~exist(build_dir, 'dir'), mkdir('vendor/tet-cpp-build'); end;
    cd(build_dir);

    disp 'Compiling C++ client (cmake)';
    [status, cmdout] = system([cmake_exec ' ../tet-cpp-client-master']);

    if status ~= 0 
        cd '../..';
        error(cmdout);
    end

    disp 'Compiling C++ client (make)';
    [status, cmdout] = system('make');

    if status ~= 0
        cd '../..';
        error(cmdout);
    end

    cd '../..';
end

% Verify that the client library was compiled
if ~exist(gaze_lib, 'file')
    error(['The C++ client library could not be located at: ' gaze_lib]);
end

% Compile our MEX file
mex CXXFLAGS='-std=c++11 -stdlib=libc++' ...
    -Ivendor/tet-cpp-client-master/include ...
    -Lvendor/tet-cpp-build ...
    -lGazeApiLib -lc++ -lboost_thread-mt -lboost_system-mt ...
    -v -cxx src/eyetribe.cpp src/utility.cpp src/datatypes.cpp

% All done...
disp(' ')
