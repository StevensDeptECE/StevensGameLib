# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/dkruger/git/StevensGameLib/src/opengl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dkruger/git/StevensGameLib/src/opengl/build

# Include any dependencies generated for this target.
include external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/depend.make

# Include the progress variables for this target.
include external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/flags.make

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o: ../external/glfw-3.1.2/tests/joysticks.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dkruger/git/StevensGameLib/src/opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o"
	cd /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/joysticks.dir/joysticks.c.o   -c /home/dkruger/git/StevensGameLib/src/opengl/external/glfw-3.1.2/tests/joysticks.c

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/joysticks.dir/joysticks.c.i"
	cd /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/dkruger/git/StevensGameLib/src/opengl/external/glfw-3.1.2/tests/joysticks.c > CMakeFiles/joysticks.dir/joysticks.c.i

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/joysticks.dir/joysticks.c.s"
	cd /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/dkruger/git/StevensGameLib/src/opengl/external/glfw-3.1.2/tests/joysticks.c -o CMakeFiles/joysticks.dir/joysticks.c.s

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.requires:

.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.requires

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.provides: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.requires
	$(MAKE) -f external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/build.make external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.provides.build
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.provides

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.provides.build: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o


# Object files for target joysticks
joysticks_OBJECTS = \
"CMakeFiles/joysticks.dir/joysticks.c.o"

# External object files for target joysticks
joysticks_EXTERNAL_OBJECTS =

external/glfw-3.1.2/tests/joysticks: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o
external/glfw-3.1.2/tests/joysticks: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/build.make
external/glfw-3.1.2/tests/joysticks: external/glfw-3.1.2/src/libglfw3.a
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/librt.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libm.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libX11.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libXrandr.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libXinerama.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libXi.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libXcursor.so
external/glfw-3.1.2/tests/joysticks: /usr/lib/x86_64-linux-gnu/libGL.so
external/glfw-3.1.2/tests/joysticks: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dkruger/git/StevensGameLib/src/opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable joysticks"
	cd /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/joysticks.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/build: external/glfw-3.1.2/tests/joysticks

.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/build

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/requires: external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/joysticks.c.o.requires

.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/requires

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/clean:
	cd /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/joysticks.dir/cmake_clean.cmake
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/clean

external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/depend:
	cd /home/dkruger/git/StevensGameLib/src/opengl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dkruger/git/StevensGameLib/src/opengl /home/dkruger/git/StevensGameLib/src/opengl/external/glfw-3.1.2/tests /home/dkruger/git/StevensGameLib/src/opengl/build /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests /home/dkruger/git/StevensGameLib/src/opengl/build/external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/joysticks.dir/depend

