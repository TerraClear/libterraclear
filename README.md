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
- realsense2 libs  (OPTIONAL - Compile with -DTC_USE_REALSENSE): https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md

*NOTES - Optional dependency support added by either an #include or compiler switch..