# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2018.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kekor/C_HomeWorks/dz2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kekor/C_HomeWorks/dz2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/dz2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dz2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dz2.dir/flags.make

CMakeFiles/dz2.dir/main.c.o: CMakeFiles/dz2.dir/flags.make
CMakeFiles/dz2.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kekor/C_HomeWorks/dz2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/dz2.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dz2.dir/main.c.o   -c /home/kekor/C_HomeWorks/dz2/main.c

CMakeFiles/dz2.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dz2.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kekor/C_HomeWorks/dz2/main.c > CMakeFiles/dz2.dir/main.c.i

CMakeFiles/dz2.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dz2.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kekor/C_HomeWorks/dz2/main.c -o CMakeFiles/dz2.dir/main.c.s

# Object files for target dz2
dz2_OBJECTS = \
"CMakeFiles/dz2.dir/main.c.o"

# External object files for target dz2
dz2_EXTERNAL_OBJECTS =

dz2: CMakeFiles/dz2.dir/main.c.o
dz2: CMakeFiles/dz2.dir/build.make
dz2: CMakeFiles/dz2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kekor/C_HomeWorks/dz2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable dz2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dz2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dz2.dir/build: dz2

.PHONY : CMakeFiles/dz2.dir/build

CMakeFiles/dz2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dz2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dz2.dir/clean

CMakeFiles/dz2.dir/depend:
	cd /home/kekor/C_HomeWorks/dz2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kekor/C_HomeWorks/dz2 /home/kekor/C_HomeWorks/dz2 /home/kekor/C_HomeWorks/dz2/cmake-build-debug /home/kekor/C_HomeWorks/dz2/cmake-build-debug /home/kekor/C_HomeWorks/dz2/cmake-build-debug/CMakeFiles/dz2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dz2.dir/depend

