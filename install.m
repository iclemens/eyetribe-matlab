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
end

% Building C++ client
build_dir = 'vendor/tet-cpp-build';
if ~exist(build_dir, 'dir'), mkdir('vendor/tet-cpp-build'); end;
cd(build_dir);

disp 'Compiling C++ client';
[status, cmdout] = system([cmake_exec ' ../tet-cpp-client-master']);

if(status ~= 0) 
    cd('../..');
    error(cmdout);
end

cd('../..');
