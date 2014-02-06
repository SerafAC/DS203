In order to cross-compile the simulator for Windows, a cross compiler toolchain is required. 
Most distributions for PC offer preconfigured Win32 toolchains. However the preferred way to get a decent cross toolchain would be the MXE Project (http://mxe.cc/), which allows to compile toolchain and useful libraries from scratch using current packages.

The makefiles uses the C++ compiler only. You can override the default on the command line using the CXX variable (i.e. make CXX=i386-pc-mingw32-g++ or whatever you C++ compiler is named).
