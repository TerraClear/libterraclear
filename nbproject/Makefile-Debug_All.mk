#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug_All
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/cc787d41/camera_async.o \
	${OBJECTDIR}/src/appsettings.o \
	${OBJECTDIR}/src/basicserial.o \
	${OBJECTDIR}/src/camera_base.o \
	${OBJECTDIR}/src/camera_depth.o \
	${OBJECTDIR}/src/camera_depth_realsense.o \
	${OBJECTDIR}/src/camera_depth_zed.o \
	${OBJECTDIR}/src/camera_file.o \
	${OBJECTDIR}/src/camera_flir_blackfly.o \
	${OBJECTDIR}/src/camera_lucid_triton.o \
	${OBJECTDIR}/src/camera_usb.o \
	${OBJECTDIR}/src/detector_base.o \
	${OBJECTDIR}/src/detector_hsv.o \
	${OBJECTDIR}/src/error_base.o \
	${OBJECTDIR}/src/filetools.o \
	${OBJECTDIR}/src/hsvcalibration.o \
	${OBJECTDIR}/src/msgserver.o \
	${OBJECTDIR}/src/navmath.o \
	${OBJECTDIR}/src/stopwatch.o \
	${OBJECTDIR}/src/thread_base.o \
	${OBJECTDIR}/src/timed_averaging.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DTC_USE_REALSENSE -DTC_USE_SERIAL
CXXFLAGS=-DTC_USE_REALSENSE -DTC_USE_SERIAL

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs libserialport` `pkg-config --libs realsense2` `pkg-config --libs opencv`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libterraclear.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libterraclear.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libterraclear.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/cc787d41/camera_async.o: /home/koos/Desktop/Dropbox/Private/Code/cpp/libterraclear/src/camera_async.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cc787d41
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cc787d41/camera_async.o /home/koos/Desktop/Dropbox/Private/Code/cpp/libterraclear/src/camera_async.cpp

${OBJECTDIR}/src/appsettings.o: src/appsettings.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/appsettings.o src/appsettings.cpp

${OBJECTDIR}/src/basicserial.o: src/basicserial.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/basicserial.o src/basicserial.cpp

${OBJECTDIR}/src/camera_base.o: src/camera_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_base.o src/camera_base.cpp

${OBJECTDIR}/src/camera_depth.o: src/camera_depth.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth.o src/camera_depth.cpp

${OBJECTDIR}/src/camera_depth_realsense.o: src/camera_depth_realsense.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth_realsense.o src/camera_depth_realsense.cpp

${OBJECTDIR}/src/camera_depth_zed.o: src/camera_depth_zed.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth_zed.o src/camera_depth_zed.cpp

${OBJECTDIR}/src/camera_file.o: src/camera_file.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_file.o src/camera_file.cpp

${OBJECTDIR}/src/camera_flir_blackfly.o: src/camera_flir_blackfly.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_flir_blackfly.o src/camera_flir_blackfly.cpp

${OBJECTDIR}/src/camera_lucid_triton.o: src/camera_lucid_triton.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_lucid_triton.o src/camera_lucid_triton.cpp

${OBJECTDIR}/src/camera_usb.o: src/camera_usb.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_usb.o src/camera_usb.cpp

${OBJECTDIR}/src/detector_base.o: src/detector_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/detector_base.o src/detector_base.cpp

${OBJECTDIR}/src/detector_hsv.o: src/detector_hsv.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/detector_hsv.o src/detector_hsv.cpp

${OBJECTDIR}/src/error_base.o: src/error_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/error_base.o src/error_base.cpp

${OBJECTDIR}/src/filetools.o: src/filetools.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/filetools.o src/filetools.cpp

${OBJECTDIR}/src/hsvcalibration.o: src/hsvcalibration.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hsvcalibration.o src/hsvcalibration.cpp

${OBJECTDIR}/src/msgserver.o: src/msgserver.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/msgserver.o src/msgserver.cpp

${OBJECTDIR}/src/navmath.o: src/navmath.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/navmath.o src/navmath.cpp

${OBJECTDIR}/src/stopwatch.o: src/stopwatch.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stopwatch.o src/stopwatch.cpp

${OBJECTDIR}/src/thread_base.o: src/thread_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread_base.o src/thread_base.cpp

${OBJECTDIR}/src/timed_averaging.o: src/timed_averaging.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` `pkg-config --cflags realsense2` `pkg-config --cflags opencv` -std=c++11  -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timed_averaging.o src/timed_averaging.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
