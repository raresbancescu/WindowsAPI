# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/tema6new.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tema6new.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tema6new.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tema6new.dir/flags.make

CMakeFiles/tema6new.dir/P2.c.obj: CMakeFiles/tema6new.dir/flags.make
CMakeFiles/tema6new.dir/P2.c.obj: CMakeFiles/tema6new.dir/includes_C.rsp
CMakeFiles/tema6new.dir/P2.c.obj: D:/facultate/Anul\ 3/Semestrul\ 1/CSSO/tema6new/P2.c
CMakeFiles/tema6new.dir/P2.c.obj: CMakeFiles/tema6new.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tema6new.dir/P2.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tema6new.dir/P2.c.obj -MF CMakeFiles\tema6new.dir\P2.c.obj.d -o CMakeFiles\tema6new.dir\P2.c.obj -c "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\P2.c"

CMakeFiles/tema6new.dir/P2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tema6new.dir/P2.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\P2.c" > CMakeFiles\tema6new.dir\P2.c.i

CMakeFiles/tema6new.dir/P2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tema6new.dir/P2.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\P2.c" -o CMakeFiles\tema6new.dir\P2.c.s

# Object files for target tema6new
tema6new_OBJECTS = \
"CMakeFiles/tema6new.dir/P2.c.obj"

# External object files for target tema6new
tema6new_EXTERNAL_OBJECTS =

tema6new.exe: CMakeFiles/tema6new.dir/P2.c.obj
tema6new.exe: CMakeFiles/tema6new.dir/build.make
tema6new.exe: CMakeFiles/tema6new.dir/linklibs.rsp
tema6new.exe: CMakeFiles/tema6new.dir/objects1.rsp
tema6new.exe: CMakeFiles/tema6new.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tema6new.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\tema6new.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tema6new.dir/build: tema6new.exe
.PHONY : CMakeFiles/tema6new.dir/build

CMakeFiles/tema6new.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\tema6new.dir\cmake_clean.cmake
.PHONY : CMakeFiles/tema6new.dir/clean

CMakeFiles/tema6new.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new" "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new" "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug" "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug" "D:\facultate\Anul 3\Semestrul 1\CSSO\tema6new\cmake-build-debug\CMakeFiles\tema6new.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/tema6new.dir/depend

