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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/b9c8e08b/camera_velocity_calculator.o \
	${OBJECTDIR}/_ext/b9c8e08b/velocity_calculator.o \
	${OBJECTDIR}/_ext/b9c8e08b/velocity_tracker.o \
	${OBJECTDIR}/_ext/cc787d41/camera_async.o \
	${OBJECTDIR}/src/appsettings.o \
	${OBJECTDIR}/src/basicserial.o \
	${OBJECTDIR}/src/camera_base.o \
	${OBJECTDIR}/src/camera_depth.o \
	${OBJECTDIR}/src/camera_depth_realsense.o \
	${OBJECTDIR}/src/camera_depth_zed.o \
	${OBJECTDIR}/src/camera_file.o \
	${OBJECTDIR}/src/camera_flir_blackfly.o \
	${OBJECTDIR}/src/camera_flir_blackfly_system.o \
	${OBJECTDIR}/src/camera_lucid_triton.o \
	${OBJECTDIR}/src/camera_recorder.o \
	${OBJECTDIR}/src/camera_usb.o \
	${OBJECTDIR}/src/camera_velocity_calculator.o \
	${OBJECTDIR}/src/detector_base.o \
	${OBJECTDIR}/src/detector_hsv.o \
	${OBJECTDIR}/src/detector_motion.o \
	${OBJECTDIR}/src/error_base.o \
	${OBJECTDIR}/src/filetools.o \
	${OBJECTDIR}/src/hsvcalibration.o \
	${OBJECTDIR}/src/msgserver.o \
	${OBJECTDIR}/src/navmath.o \
	${OBJECTDIR}/src/stopwatch.o \
	${OBJECTDIR}/src/thread_base.o \
	${OBJECTDIR}/src/timed_averaging.o \
	${OBJECTDIR}/src/tracking_object.o \
	${OBJECTDIR}/src/tracking_object_multi.o \
	${OBJECTDIR}/src/tracking_position.o \
	${OBJECTDIR}/src/tracking_velocity.o \
	${OBJECTDIR}/src/tracking_velocity_multi.o \
	${OBJECTDIR}/src/vision_warp.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibterraclear.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibterraclear.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liblibterraclear.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/b9c8e08b/camera_velocity_calculator.o: /home/izzy/TerraClear/yolo-detect/libterraclear/src/camera_velocity_calculator.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/b9c8e08b
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b9c8e08b/camera_velocity_calculator.o /home/izzy/TerraClear/yolo-detect/libterraclear/src/camera_velocity_calculator.cpp

${OBJECTDIR}/_ext/b9c8e08b/velocity_calculator.o: /home/izzy/TerraClear/yolo-detect/libterraclear/src/velocity_calculator.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/b9c8e08b
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b9c8e08b/velocity_calculator.o /home/izzy/TerraClear/yolo-detect/libterraclear/src/velocity_calculator.cpp

${OBJECTDIR}/_ext/b9c8e08b/velocity_tracker.o: /home/izzy/TerraClear/yolo-detect/libterraclear/src/velocity_tracker.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/b9c8e08b
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/b9c8e08b/velocity_tracker.o /home/izzy/TerraClear/yolo-detect/libterraclear/src/velocity_tracker.cpp

${OBJECTDIR}/_ext/cc787d41/camera_async.o: /home/koos/Desktop/Dropbox/Private/Code/cpp/libterraclear/src/camera_async.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/cc787d41
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/cc787d41/camera_async.o /home/koos/Desktop/Dropbox/Private/Code/cpp/libterraclear/src/camera_async.cpp

${OBJECTDIR}/src/appsettings.o: src/appsettings.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/appsettings.o src/appsettings.cpp

${OBJECTDIR}/src/basicserial.o: src/basicserial.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/basicserial.o src/basicserial.cpp

${OBJECTDIR}/src/camera_base.o: src/camera_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_base.o src/camera_base.cpp

${OBJECTDIR}/src/camera_depth.o: src/camera_depth.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth.o src/camera_depth.cpp

${OBJECTDIR}/src/camera_depth_realsense.o: src/camera_depth_realsense.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth_realsense.o src/camera_depth_realsense.cpp

${OBJECTDIR}/src/camera_depth_zed.o: src/camera_depth_zed.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_depth_zed.o src/camera_depth_zed.cpp

${OBJECTDIR}/src/camera_file.o: src/camera_file.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_file.o src/camera_file.cpp

${OBJECTDIR}/src/camera_flir_blackfly.o: src/camera_flir_blackfly.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_flir_blackfly.o src/camera_flir_blackfly.cpp

${OBJECTDIR}/src/camera_flir_blackfly_system.o: src/camera_flir_blackfly_system.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_flir_blackfly_system.o src/camera_flir_blackfly_system.cpp

${OBJECTDIR}/src/camera_lucid_triton.o: src/camera_lucid_triton.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_lucid_triton.o src/camera_lucid_triton.cpp

${OBJECTDIR}/src/camera_recorder.o: src/camera_recorder.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_recorder.o src/camera_recorder.cpp

${OBJECTDIR}/src/camera_usb.o: src/camera_usb.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_usb.o src/camera_usb.cpp

${OBJECTDIR}/src/camera_velocity_calculator.o: src/camera_velocity_calculator.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/camera_velocity_calculator.o src/camera_velocity_calculator.cpp

${OBJECTDIR}/src/detector_base.o: src/detector_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/detector_base.o src/detector_base.cpp

${OBJECTDIR}/src/detector_hsv.o: src/detector_hsv.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/detector_hsv.o src/detector_hsv.cpp

${OBJECTDIR}/src/detector_motion.o: src/detector_motion.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/detector_motion.o src/detector_motion.cpp

${OBJECTDIR}/src/error_base.o: src/error_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/error_base.o src/error_base.cpp

${OBJECTDIR}/src/filetools.o: src/filetools.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/filetools.o src/filetools.cpp

${OBJECTDIR}/src/hsvcalibration.o: src/hsvcalibration.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hsvcalibration.o src/hsvcalibration.cpp

${OBJECTDIR}/src/msgserver.o: src/msgserver.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/msgserver.o src/msgserver.cpp

${OBJECTDIR}/src/navmath.o: src/navmath.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/navmath.o src/navmath.cpp

${OBJECTDIR}/src/stopwatch.o: src/stopwatch.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stopwatch.o src/stopwatch.cpp

${OBJECTDIR}/src/thread_base.o: src/thread_base.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread_base.o src/thread_base.cpp

${OBJECTDIR}/src/timed_averaging.o: src/timed_averaging.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timed_averaging.o src/timed_averaging.cpp

${OBJECTDIR}/src/tracking_object.o: src/tracking_object.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tracking_object.o src/tracking_object.cpp

${OBJECTDIR}/src/tracking_object_multi.o: src/tracking_object_multi.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tracking_object_multi.o src/tracking_object_multi.cpp

${OBJECTDIR}/src/tracking_position.o: src/tracking_position.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tracking_position.o src/tracking_position.cpp

${OBJECTDIR}/src/tracking_velocity.o: src/tracking_velocity.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tracking_velocity.o src/tracking_velocity.cpp

${OBJECTDIR}/src/tracking_velocity_multi.o: src/tracking_velocity_multi.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tracking_velocity_multi.o src/tracking_velocity_multi.cpp

${OBJECTDIR}/src/vision_warp.o: src/vision_warp.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vision_warp.o src/vision_warp.cpp

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
