# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aidong/industrial-display-qt-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aidong/industrial-display-qt-cpp/build

# Include any dependencies generated for this target.
include CMakeFiles/ADAS_System.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ADAS_System.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ADAS_System.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ADAS_System.dir/flags.make

CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o: CMakeFiles/ADAS_System.dir/flags.make
CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o: ADAS_System_autogen/mocs_compilation.cpp
CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o: CMakeFiles/ADAS_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o -MF CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o -c /home/aidong/industrial-display-qt-cpp/build/ADAS_System_autogen/mocs_compilation.cpp

CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aidong/industrial-display-qt-cpp/build/ADAS_System_autogen/mocs_compilation.cpp > CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.i

CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aidong/industrial-display-qt-cpp/build/ADAS_System_autogen/mocs_compilation.cpp -o CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.s

CMakeFiles/ADAS_System.dir/main.cpp.o: CMakeFiles/ADAS_System.dir/flags.make
CMakeFiles/ADAS_System.dir/main.cpp.o: ../main.cpp
CMakeFiles/ADAS_System.dir/main.cpp.o: CMakeFiles/ADAS_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ADAS_System.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ADAS_System.dir/main.cpp.o -MF CMakeFiles/ADAS_System.dir/main.cpp.o.d -o CMakeFiles/ADAS_System.dir/main.cpp.o -c /home/aidong/industrial-display-qt-cpp/main.cpp

CMakeFiles/ADAS_System.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ADAS_System.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aidong/industrial-display-qt-cpp/main.cpp > CMakeFiles/ADAS_System.dir/main.cpp.i

CMakeFiles/ADAS_System.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ADAS_System.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aidong/industrial-display-qt-cpp/main.cpp -o CMakeFiles/ADAS_System.dir/main.cpp.s

CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o: CMakeFiles/ADAS_System.dir/flags.make
CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o: ../adasdisplay.cpp
CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o: CMakeFiles/ADAS_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o -MF CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o.d -o CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o -c /home/aidong/industrial-display-qt-cpp/adasdisplay.cpp

CMakeFiles/ADAS_System.dir/adasdisplay.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ADAS_System.dir/adasdisplay.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aidong/industrial-display-qt-cpp/adasdisplay.cpp > CMakeFiles/ADAS_System.dir/adasdisplay.cpp.i

CMakeFiles/ADAS_System.dir/adasdisplay.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ADAS_System.dir/adasdisplay.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aidong/industrial-display-qt-cpp/adasdisplay.cpp -o CMakeFiles/ADAS_System.dir/adasdisplay.cpp.s

CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o: CMakeFiles/ADAS_System.dir/flags.make
CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o: ../draggablecamerapanel.cpp
CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o: CMakeFiles/ADAS_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o -MF CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o.d -o CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o -c /home/aidong/industrial-display-qt-cpp/draggablecamerapanel.cpp

CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aidong/industrial-display-qt-cpp/draggablecamerapanel.cpp > CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.i

CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aidong/industrial-display-qt-cpp/draggablecamerapanel.cpp -o CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.s

CMakeFiles/ADAS_System.dir/icon.cpp.o: CMakeFiles/ADAS_System.dir/flags.make
CMakeFiles/ADAS_System.dir/icon.cpp.o: ../icon.cpp
CMakeFiles/ADAS_System.dir/icon.cpp.o: CMakeFiles/ADAS_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ADAS_System.dir/icon.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ADAS_System.dir/icon.cpp.o -MF CMakeFiles/ADAS_System.dir/icon.cpp.o.d -o CMakeFiles/ADAS_System.dir/icon.cpp.o -c /home/aidong/industrial-display-qt-cpp/icon.cpp

CMakeFiles/ADAS_System.dir/icon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ADAS_System.dir/icon.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aidong/industrial-display-qt-cpp/icon.cpp > CMakeFiles/ADAS_System.dir/icon.cpp.i

CMakeFiles/ADAS_System.dir/icon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ADAS_System.dir/icon.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aidong/industrial-display-qt-cpp/icon.cpp -o CMakeFiles/ADAS_System.dir/icon.cpp.s

# Object files for target ADAS_System
ADAS_System_OBJECTS = \
"CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/ADAS_System.dir/main.cpp.o" \
"CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o" \
"CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o" \
"CMakeFiles/ADAS_System.dir/icon.cpp.o"

# External object files for target ADAS_System
ADAS_System_EXTERNAL_OBJECTS =

ADAS_System: CMakeFiles/ADAS_System.dir/ADAS_System_autogen/mocs_compilation.cpp.o
ADAS_System: CMakeFiles/ADAS_System.dir/main.cpp.o
ADAS_System: CMakeFiles/ADAS_System.dir/adasdisplay.cpp.o
ADAS_System: CMakeFiles/ADAS_System.dir/draggablecamerapanel.cpp.o
ADAS_System: CMakeFiles/ADAS_System.dir/icon.cpp.o
ADAS_System: CMakeFiles/ADAS_System.dir/build.make
ADAS_System: /usr/lib/aarch64-linux-gnu/libQt5Widgets.so.5.15.3
ADAS_System: /usr/lib/libopencv_gapi.so.4.8.0
ADAS_System: /usr/lib/libopencv_highgui.so.4.8.0
ADAS_System: /usr/lib/libopencv_ml.so.4.8.0
ADAS_System: /usr/lib/libopencv_objdetect.so.4.8.0
ADAS_System: /usr/lib/libopencv_photo.so.4.8.0
ADAS_System: /usr/lib/libopencv_stitching.so.4.8.0
ADAS_System: /usr/lib/libopencv_video.so.4.8.0
ADAS_System: /usr/lib/libopencv_videoio.so.4.8.0
ADAS_System: /usr/lib/aarch64-linux-gnu/libQt5Gui.so.5.15.3
ADAS_System: /usr/lib/aarch64-linux-gnu/libQt5Core.so.5.15.3
ADAS_System: /usr/lib/libopencv_imgcodecs.so.4.8.0
ADAS_System: /usr/lib/libopencv_dnn.so.4.8.0
ADAS_System: /usr/lib/libopencv_calib3d.so.4.8.0
ADAS_System: /usr/lib/libopencv_features2d.so.4.8.0
ADAS_System: /usr/lib/libopencv_flann.so.4.8.0
ADAS_System: /usr/lib/libopencv_imgproc.so.4.8.0
ADAS_System: /usr/lib/libopencv_core.so.4.8.0
ADAS_System: CMakeFiles/ADAS_System.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aidong/industrial-display-qt-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ADAS_System"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ADAS_System.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ADAS_System.dir/build: ADAS_System
.PHONY : CMakeFiles/ADAS_System.dir/build

CMakeFiles/ADAS_System.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ADAS_System.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ADAS_System.dir/clean

CMakeFiles/ADAS_System.dir/depend:
	cd /home/aidong/industrial-display-qt-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aidong/industrial-display-qt-cpp /home/aidong/industrial-display-qt-cpp /home/aidong/industrial-display-qt-cpp/build /home/aidong/industrial-display-qt-cpp/build /home/aidong/industrial-display-qt-cpp/build/CMakeFiles/ADAS_System.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ADAS_System.dir/depend

