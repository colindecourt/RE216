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
CMAKE_SOURCE_DIR = /home/elsa/Documents/RE216/jalon04/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elsa/Documents/RE216/jalon04/build

# Include any dependencies generated for this target.
include jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/depend.make

# Include the progress variables for this target.
include jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/progress.make

# Include the compile flags for this target's objects.
include jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/flags.make

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/flags.make
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o: /home/elsa/Documents/RE216/jalon04/src/jalon04/src/client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elsa/Documents/RE216/jalon04/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o   -c /home/elsa/Documents/RE216/jalon04/src/jalon04/src/client.c

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.i"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/elsa/Documents/RE216/jalon04/src/jalon04/src/client.c > CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.i

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.s"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/elsa/Documents/RE216/jalon04/src/jalon04/src/client.c -o CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.s

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.requires:

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.requires

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.provides: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.requires
	$(MAKE) -f jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build.make jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.provides.build
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.provides

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.provides.build: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o


jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/flags.make
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o: /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/common_tools.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elsa/Documents/RE216/jalon04/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o   -c /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/common_tools.c

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.i"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/common_tools.c > CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.i

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.s"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/common_tools.c -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.s

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.requires:

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.requires

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.provides: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.requires
	$(MAKE) -f jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build.make jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.provides.build
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.provides

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.provides.build: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o


jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/flags.make
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o: /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/client_tools.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elsa/Documents/RE216/jalon04/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o   -c /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/client_tools.c

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.i"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/client_tools.c > CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.i

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.s"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/client_tools.c -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.s

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.requires:

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.requires

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.provides: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.requires
	$(MAKE) -f jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build.make jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.provides.build
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.provides

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.provides.build: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o


jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/flags.make
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o: /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/lines.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elsa/Documents/RE216/jalon04/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o   -c /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/lines.c

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.i"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/lines.c > CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.i

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.s"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/elsa/Documents/RE216/jalon04/src/jalon04/src/include/lines.c -o CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.s

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.requires:

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.requires

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.provides: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.requires
	$(MAKE) -f jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build.make jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.provides.build
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.provides

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.provides.build: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o


# Object files for target RE216_JALON04_CLIENT
RE216_JALON04_CLIENT_OBJECTS = \
"CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o" \
"CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o" \
"CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o" \
"CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o"

# External object files for target RE216_JALON04_CLIENT
RE216_JALON04_CLIENT_EXTERNAL_OBJECTS =

jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o
jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o
jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o
jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o
jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build.make
jalon04/src/RE216_JALON04_CLIENT: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/elsa/Documents/RE216/jalon04/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable RE216_JALON04_CLIENT"
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RE216_JALON04_CLIENT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build: jalon04/src/RE216_JALON04_CLIENT

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/build

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/requires: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/client.c.o.requires
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/requires: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/common_tools.c.o.requires
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/requires: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/client_tools.c.o.requires
jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/requires: jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/include/lines.c.o.requires

.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/requires

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/clean:
	cd /home/elsa/Documents/RE216/jalon04/build/jalon04/src && $(CMAKE_COMMAND) -P CMakeFiles/RE216_JALON04_CLIENT.dir/cmake_clean.cmake
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/clean

jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/depend:
	cd /home/elsa/Documents/RE216/jalon04/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elsa/Documents/RE216/jalon04/src /home/elsa/Documents/RE216/jalon04/src/jalon04/src /home/elsa/Documents/RE216/jalon04/build /home/elsa/Documents/RE216/jalon04/build/jalon04/src /home/elsa/Documents/RE216/jalon04/build/jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jalon04/src/CMakeFiles/RE216_JALON04_CLIENT.dir/depend

