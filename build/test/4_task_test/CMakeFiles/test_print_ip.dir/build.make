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
CMAKE_COMMAND = /home/imelker/Загрузки/clion-2018.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/imelker/Загрузки/clion-2018.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/imelker/workspace/git/otus-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/imelker/workspace/git/otus-cpp/build

# Include any dependencies generated for this target.
include test/4_task_test/CMakeFiles/test_print_ip.dir/depend.make

# Include the progress variables for this target.
include test/4_task_test/CMakeFiles/test_print_ip.dir/progress.make

# Include the compile flags for this target's objects.
include test/4_task_test/CMakeFiles/test_print_ip.dir/flags.make

test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o: test/4_task_test/CMakeFiles/test_print_ip.dir/flags.make
test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o: ../test/4_task_test/test_print_ip.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/imelker/workspace/git/otus-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o"
	cd /home/imelker/workspace/git/otus-cpp/build/test/4_task_test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o -c /home/imelker/workspace/git/otus-cpp/test/4_task_test/test_print_ip.cpp

test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_print_ip.dir/test_print_ip.cpp.i"
	cd /home/imelker/workspace/git/otus-cpp/build/test/4_task_test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/imelker/workspace/git/otus-cpp/test/4_task_test/test_print_ip.cpp > CMakeFiles/test_print_ip.dir/test_print_ip.cpp.i

test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_print_ip.dir/test_print_ip.cpp.s"
	cd /home/imelker/workspace/git/otus-cpp/build/test/4_task_test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/imelker/workspace/git/otus-cpp/test/4_task_test/test_print_ip.cpp -o CMakeFiles/test_print_ip.dir/test_print_ip.cpp.s

# Object files for target test_print_ip
test_print_ip_OBJECTS = \
"CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o"

# External object files for target test_print_ip
test_print_ip_EXTERNAL_OBJECTS =

bin/test_print_ip: test/4_task_test/CMakeFiles/test_print_ip.dir/test_print_ip.cpp.o
bin/test_print_ip: test/4_task_test/CMakeFiles/test_print_ip.dir/build.make
bin/test_print_ip: /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.so
bin/test_print_ip: test/4_task_test/CMakeFiles/test_print_ip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/imelker/workspace/git/otus-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/test_print_ip"
	cd /home/imelker/workspace/git/otus-cpp/build/test/4_task_test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_print_ip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/4_task_test/CMakeFiles/test_print_ip.dir/build: bin/test_print_ip

.PHONY : test/4_task_test/CMakeFiles/test_print_ip.dir/build

test/4_task_test/CMakeFiles/test_print_ip.dir/clean:
	cd /home/imelker/workspace/git/otus-cpp/build/test/4_task_test && $(CMAKE_COMMAND) -P CMakeFiles/test_print_ip.dir/cmake_clean.cmake
.PHONY : test/4_task_test/CMakeFiles/test_print_ip.dir/clean

test/4_task_test/CMakeFiles/test_print_ip.dir/depend:
	cd /home/imelker/workspace/git/otus-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/imelker/workspace/git/otus-cpp /home/imelker/workspace/git/otus-cpp/test/4_task_test /home/imelker/workspace/git/otus-cpp/build /home/imelker/workspace/git/otus-cpp/build/test/4_task_test /home/imelker/workspace/git/otus-cpp/build/test/4_task_test/CMakeFiles/test_print_ip.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/4_task_test/CMakeFiles/test_print_ip.dir/depend

