# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tomasz/Desktop/Mapy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomasz/Desktop/Mapy/Debug

# Include any dependencies generated for this target.
include src/CMakeFiles/aisdiMaps.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/aisdiMaps.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/aisdiMaps.dir/flags.make

src/CMakeFiles/aisdiMaps.dir/main.cpp.o: src/CMakeFiles/aisdiMaps.dir/flags.make
src/CMakeFiles/aisdiMaps.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/Desktop/Mapy/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/aisdiMaps.dir/main.cpp.o"
	cd /home/tomasz/Desktop/Mapy/Debug/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aisdiMaps.dir/main.cpp.o -c /home/tomasz/Desktop/Mapy/src/main.cpp

src/CMakeFiles/aisdiMaps.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aisdiMaps.dir/main.cpp.i"
	cd /home/tomasz/Desktop/Mapy/Debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tomasz/Desktop/Mapy/src/main.cpp > CMakeFiles/aisdiMaps.dir/main.cpp.i

src/CMakeFiles/aisdiMaps.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aisdiMaps.dir/main.cpp.s"
	cd /home/tomasz/Desktop/Mapy/Debug/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tomasz/Desktop/Mapy/src/main.cpp -o CMakeFiles/aisdiMaps.dir/main.cpp.s

src/CMakeFiles/aisdiMaps.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/aisdiMaps.dir/main.cpp.o.requires

src/CMakeFiles/aisdiMaps.dir/main.cpp.o.provides: src/CMakeFiles/aisdiMaps.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/aisdiMaps.dir/build.make src/CMakeFiles/aisdiMaps.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/aisdiMaps.dir/main.cpp.o.provides

src/CMakeFiles/aisdiMaps.dir/main.cpp.o.provides.build: src/CMakeFiles/aisdiMaps.dir/main.cpp.o


# Object files for target aisdiMaps
aisdiMaps_OBJECTS = \
"CMakeFiles/aisdiMaps.dir/main.cpp.o"

# External object files for target aisdiMaps
aisdiMaps_EXTERNAL_OBJECTS =

src/aisdiMaps: src/CMakeFiles/aisdiMaps.dir/main.cpp.o
src/aisdiMaps: src/CMakeFiles/aisdiMaps.dir/build.make
src/aisdiMaps: src/CMakeFiles/aisdiMaps.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomasz/Desktop/Mapy/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable aisdiMaps"
	cd /home/tomasz/Desktop/Mapy/Debug/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aisdiMaps.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/aisdiMaps.dir/build: src/aisdiMaps

.PHONY : src/CMakeFiles/aisdiMaps.dir/build

src/CMakeFiles/aisdiMaps.dir/requires: src/CMakeFiles/aisdiMaps.dir/main.cpp.o.requires

.PHONY : src/CMakeFiles/aisdiMaps.dir/requires

src/CMakeFiles/aisdiMaps.dir/clean:
	cd /home/tomasz/Desktop/Mapy/Debug/src && $(CMAKE_COMMAND) -P CMakeFiles/aisdiMaps.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/aisdiMaps.dir/clean

src/CMakeFiles/aisdiMaps.dir/depend:
	cd /home/tomasz/Desktop/Mapy/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomasz/Desktop/Mapy /home/tomasz/Desktop/Mapy/src /home/tomasz/Desktop/Mapy/Debug /home/tomasz/Desktop/Mapy/Debug/src /home/tomasz/Desktop/Mapy/Debug/src/CMakeFiles/aisdiMaps.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/aisdiMaps.dir/depend

