# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/shak/Desktop/coding/uni/proj api/test clion"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/test_clion.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_clion.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_clion.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_clion.dir/flags.make

CMakeFiles/test_clion.dir/main.c.o: CMakeFiles/test_clion.dir/flags.make
CMakeFiles/test_clion.dir/main.c.o: ../main.c
CMakeFiles/test_clion.dir/main.c.o: CMakeFiles/test_clion.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_clion.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test_clion.dir/main.c.o -MF CMakeFiles/test_clion.dir/main.c.o.d -o CMakeFiles/test_clion.dir/main.c.o -c "/Users/shak/Desktop/coding/uni/proj api/test clion/main.c"

CMakeFiles/test_clion.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_clion.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/shak/Desktop/coding/uni/proj api/test clion/main.c" > CMakeFiles/test_clion.dir/main.c.i

CMakeFiles/test_clion.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_clion.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/shak/Desktop/coding/uni/proj api/test clion/main.c" -o CMakeFiles/test_clion.dir/main.c.s

# Object files for target test_clion
test_clion_OBJECTS = \
"CMakeFiles/test_clion.dir/main.c.o"

# External object files for target test_clion
test_clion_EXTERNAL_OBJECTS =

test_clion: CMakeFiles/test_clion.dir/main.c.o
test_clion: CMakeFiles/test_clion.dir/build.make
test_clion: CMakeFiles/test_clion.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_clion"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_clion.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_clion.dir/build: test_clion
.PHONY : CMakeFiles/test_clion.dir/build

CMakeFiles/test_clion.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_clion.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_clion.dir/clean

CMakeFiles/test_clion.dir/depend:
	cd "/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/shak/Desktop/coding/uni/proj api/test clion" "/Users/shak/Desktop/coding/uni/proj api/test clion" "/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug" "/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug" "/Users/shak/Desktop/coding/uni/proj api/test clion/cmake-build-debug/CMakeFiles/test_clion.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/test_clion.dir/depend

