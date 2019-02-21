# libterraclear

TerraClear Shared C++ libary

Folder Structures
------------------
Its recommended to make a /data/sources folder on target system, these are absolute paths
used by the compiler & linker. Then clone all depencies & sources into that folder.

Dependencies:
----------------
- libcppjson-dev :  sudo apt-get install libjsoncpp-dev
- libserialport (OPTIONAL - Compile with -DTC_USE_SERIAL): get from https://sigrok.org/wiki/Libserialport (requires autoconf and libtool install)
- realsense2 libs  (OPTIONAL - Compile with -DTC_USE_REALSENSE): https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md

Make Notes
----------
- standard "make" will NOT include Intel RealSense support.
- use "make CONF=Debug_RealSense" to build with RealSense support & add compiler "-DTC_USE_REALSENSE" switch 
- If making librealsense, ensure -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=true -DBUILD_WITH_CUDA=true -DBUILD_WITH_OPENMP=true

