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
CMAKE_SOURCE_DIR = /home/xsl/Desktop/IM-Server01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xsl/Desktop/IM-Server01/build

# Include any dependencies generated for this target.
include common/CMakeFiles/common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include common/CMakeFiles/common.dir/compiler_depend.make

# Include the progress variables for this target.
include common/CMakeFiles/common.dir/progress.make

# Include the compile flags for this target's objects.
include common/CMakeFiles/common.dir/flags.make

common/CMakeFiles/common.dir/global.cpp.o: common/CMakeFiles/common.dir/flags.make
common/CMakeFiles/common.dir/global.cpp.o: ../common/global.cpp
common/CMakeFiles/common.dir/global.cpp.o: common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xsl/Desktop/IM-Server01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object common/CMakeFiles/common.dir/global.cpp.o"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT common/CMakeFiles/common.dir/global.cpp.o -MF CMakeFiles/common.dir/global.cpp.o.d -o CMakeFiles/common.dir/global.cpp.o -c /home/xsl/Desktop/IM-Server01/common/global.cpp

common/CMakeFiles/common.dir/global.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common.dir/global.cpp.i"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xsl/Desktop/IM-Server01/common/global.cpp > CMakeFiles/common.dir/global.cpp.i

common/CMakeFiles/common.dir/global.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common.dir/global.cpp.s"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xsl/Desktop/IM-Server01/common/global.cpp -o CMakeFiles/common.dir/global.cpp.s

common/CMakeFiles/common.dir/threadpool.cpp.o: common/CMakeFiles/common.dir/flags.make
common/CMakeFiles/common.dir/threadpool.cpp.o: ../common/threadpool.cpp
common/CMakeFiles/common.dir/threadpool.cpp.o: common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xsl/Desktop/IM-Server01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object common/CMakeFiles/common.dir/threadpool.cpp.o"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT common/CMakeFiles/common.dir/threadpool.cpp.o -MF CMakeFiles/common.dir/threadpool.cpp.o.d -o CMakeFiles/common.dir/threadpool.cpp.o -c /home/xsl/Desktop/IM-Server01/common/threadpool.cpp

common/CMakeFiles/common.dir/threadpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common.dir/threadpool.cpp.i"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xsl/Desktop/IM-Server01/common/threadpool.cpp > CMakeFiles/common.dir/threadpool.cpp.i

common/CMakeFiles/common.dir/threadpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common.dir/threadpool.cpp.s"
	cd /home/xsl/Desktop/IM-Server01/build/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xsl/Desktop/IM-Server01/common/threadpool.cpp -o CMakeFiles/common.dir/threadpool.cpp.s

# Object files for target common
common_OBJECTS = \
"CMakeFiles/common.dir/global.cpp.o" \
"CMakeFiles/common.dir/threadpool.cpp.o"

# External object files for target common
common_EXTERNAL_OBJECTS =

common/libcommon.a: common/CMakeFiles/common.dir/global.cpp.o
common/libcommon.a: common/CMakeFiles/common.dir/threadpool.cpp.o
common/libcommon.a: common/CMakeFiles/common.dir/build.make
common/libcommon.a: common/CMakeFiles/common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xsl/Desktop/IM-Server01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libcommon.a"
	cd /home/xsl/Desktop/IM-Server01/build/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean_target.cmake
	cd /home/xsl/Desktop/IM-Server01/build/common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
common/CMakeFiles/common.dir/build: common/libcommon.a
.PHONY : common/CMakeFiles/common.dir/build

common/CMakeFiles/common.dir/clean:
	cd /home/xsl/Desktop/IM-Server01/build/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean.cmake
.PHONY : common/CMakeFiles/common.dir/clean

common/CMakeFiles/common.dir/depend:
	cd /home/xsl/Desktop/IM-Server01/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xsl/Desktop/IM-Server01 /home/xsl/Desktop/IM-Server01/common /home/xsl/Desktop/IM-Server01/build /home/xsl/Desktop/IM-Server01/build/common /home/xsl/Desktop/IM-Server01/build/common/CMakeFiles/common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : common/CMakeFiles/common.dir/depend

