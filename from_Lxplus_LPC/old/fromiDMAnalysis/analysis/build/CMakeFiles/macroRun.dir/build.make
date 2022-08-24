# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /cvmfs/sft.cern.ch/lcg/releases/CMake/3.18.4-2ffec/x86_64-centos7-gcc8-opt/bin/cmake

# The command to remove a file.
RM = /cvmfs/sft.cern.ch/lcg/releases/CMake/3.18.4-2ffec/x86_64-centos7-gcc8-opt/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /uscms/home/amalbert/nobackup/iDMAnalysis/analysis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build

# Include any dependencies generated for this target.
include CMakeFiles/macroRun.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/macroRun.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/macroRun.dir/flags.make

CMakeFiles/macroRun.dir/macroRun.cc.o: CMakeFiles/macroRun.dir/flags.make
CMakeFiles/macroRun.dir/macroRun.cc.o: ../macroRun.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/macroRun.dir/macroRun.cc.o"
	/cvmfs/sft.cern.ch/lcg/releases/gcc/8.3.0-cebb0/x86_64-centos7/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/macroRun.dir/macroRun.cc.o -c /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/macroRun.cc

CMakeFiles/macroRun.dir/macroRun.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/macroRun.dir/macroRun.cc.i"
	/cvmfs/sft.cern.ch/lcg/releases/gcc/8.3.0-cebb0/x86_64-centos7/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/macroRun.cc > CMakeFiles/macroRun.dir/macroRun.cc.i

CMakeFiles/macroRun.dir/macroRun.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/macroRun.dir/macroRun.cc.s"
	/cvmfs/sft.cern.ch/lcg/releases/gcc/8.3.0-cebb0/x86_64-centos7/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/macroRun.cc -o CMakeFiles/macroRun.dir/macroRun.cc.s

# Object files for target macroRun
macroRun_OBJECTS = \
"CMakeFiles/macroRun.dir/macroRun.cc.o"

# External object files for target macroRun
macroRun_EXTERNAL_OBJECTS =

macroRun: CMakeFiles/macroRun.dir/macroRun.cc.o
macroRun: CMakeFiles/macroRun.dir/build.make
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libPhysics.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libPostscript.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libROOTDataFrame.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libRint.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libTreePlayer.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libTree.so
macroRun: /usr/lib64/libm.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libGraf3d.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libGpad.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libGraf.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libHist.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libMatrix.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libMultiProc.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libMathCore.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libImt.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libNet.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libROOTNTuple.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libRIO.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libThread.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libROOTVecOps.so
macroRun: /cvmfs/sft.cern.ch/lcg/releases/vdt/0.4.3-992df/x86_64-centos7-gcc8-opt/lib/libvdt.so
macroRun: /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc8-opt/lib/libCore.so
macroRun: CMakeFiles/macroRun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable macroRun"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/macroRun.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/macroRun.dir/build: macroRun

.PHONY : CMakeFiles/macroRun.dir/build

CMakeFiles/macroRun.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/macroRun.dir/cmake_clean.cmake
.PHONY : CMakeFiles/macroRun.dir/clean

CMakeFiles/macroRun.dir/depend:
	cd /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /uscms/home/amalbert/nobackup/iDMAnalysis/analysis /uscms/home/amalbert/nobackup/iDMAnalysis/analysis /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build /uscms/home/amalbert/nobackup/iDMAnalysis/analysis/build/CMakeFiles/macroRun.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/macroRun.dir/depend
