# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build

# Include any dependencies generated for this target.
include CMakeFiles/createdb.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/createdb.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/createdb.dir/flags.make

CMakeFiles/createdb.dir/createdb.cpp.o: CMakeFiles/createdb.dir/flags.make
CMakeFiles/createdb.dir/createdb.cpp.o: ../createdb.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/createdb.dir/createdb.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/createdb.dir/createdb.cpp.o -c /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/createdb.cpp

CMakeFiles/createdb.dir/createdb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/createdb.dir/createdb.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/createdb.cpp > CMakeFiles/createdb.dir/createdb.cpp.i

CMakeFiles/createdb.dir/createdb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/createdb.dir/createdb.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/createdb.cpp -o CMakeFiles/createdb.dir/createdb.cpp.s

CMakeFiles/createdb.dir/createdb.cpp.o.requires:
.PHONY : CMakeFiles/createdb.dir/createdb.cpp.o.requires

CMakeFiles/createdb.dir/createdb.cpp.o.provides: CMakeFiles/createdb.dir/createdb.cpp.o.requires
	$(MAKE) -f CMakeFiles/createdb.dir/build.make CMakeFiles/createdb.dir/createdb.cpp.o.provides.build
.PHONY : CMakeFiles/createdb.dir/createdb.cpp.o.provides

CMakeFiles/createdb.dir/createdb.cpp.o.provides.build: CMakeFiles/createdb.dir/createdb.cpp.o

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o: CMakeFiles/createdb.dir/flags.make
CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o: ../contrib/docopt/docopt.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o -c /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/contrib/docopt/docopt.cpp

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/contrib/docopt/docopt.cpp > CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.i

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/contrib/docopt/docopt.cpp -o CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.s

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.requires:
.PHONY : CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.requires

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.provides: CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.requires
	$(MAKE) -f CMakeFiles/createdb.dir/build.make CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.provides.build
.PHONY : CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.provides

CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.provides.build: CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o

# Object files for target createdb
createdb_OBJECTS = \
"CMakeFiles/createdb.dir/createdb.cpp.o" \
"CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o"

# External object files for target createdb
createdb_EXTERNAL_OBJECTS =

createdb: CMakeFiles/createdb.dir/createdb.cpp.o
createdb: CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o
createdb: CMakeFiles/createdb.dir/build.make
createdb: /opt/boost/current/lib/libboost_filesystem.so
createdb: /opt/boost/current/lib/libboost_system.so
createdb: /opt/boost/current/lib/libboost_regex.so
createdb: /opt/boost/current/lib/libboost_serialization.so
createdb: libwikidb.a
createdb: ../contrib/lib/libfastdb.a
createdb: CMakeFiles/createdb.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable createdb"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/createdb.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/createdb.dir/build: createdb
.PHONY : CMakeFiles/createdb.dir/build

CMakeFiles/createdb.dir/requires: CMakeFiles/createdb.dir/createdb.cpp.o.requires
CMakeFiles/createdb.dir/requires: CMakeFiles/createdb.dir/contrib/docopt/docopt.cpp.o.requires
.PHONY : CMakeFiles/createdb.dir/requires

CMakeFiles/createdb.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/createdb.dir/cmake_clean.cmake
.PHONY : CMakeFiles/createdb.dir/clean

CMakeFiles/createdb.dir/depend:
	cd /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build /home/vofa2169/Desktop/Bachelor/visualTextAnalysis.new/trunk/ext/wikidb/build/CMakeFiles/createdb.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/createdb.dir/depend

