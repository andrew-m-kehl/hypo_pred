Building From Command Line
==========================

StlEval is written in C++14. It is tested with MSVC 2017 and g++ 7.3.
It uses CMake (3.9+ required) as the build system.

- [Windows](#windows)
- [Linux](#linux)

Windows
-------

1. Install Visual Studio 2017.
   You can get the free Community Edition [here](https://visualstudio.microsoft.com/vs/community/).
   Be sure to select 'Desktop development with C++' during installation.

2. Install CMake from [here](https://cmake.org/download/).
   Version 3.9 or higher is required.
   Make sure that CMake's `bin` directory is added to `PATH`.

3. Install Git for Windows from [here](https://git-scm.com/download/win).
   To be able to see git commit information in the output of `stle -v`, add
   Git's `bin` directory to `PATH`.

4. Open git command prompt. Clone the repository and `cd` into it.
```bash
$ git clone https://gitlab.com/abakhirkin/StlEval.git
$ cd StlEval
```

5. Initialize and update git submodules. This will fetch a version of Googletest
   that is required to run tests.
```bash
$ git submodule init
$ git submodule update
```

6. Create a build directory, e.g., a directory `build` in the source tree.
   You may name and place it differently; in this case you will need to adjust
   the paths given in these instructions.
```
$ mkdir build
```

7. Open Visual Studio command prompt for your platform and target.
   To compile x64 binaries on x64 machine (prefer this if not sure), open
   `Start -> All Programs -> Visual Studio 2017 -> Visual Studio Tools -> VC ->
    x64 Native Tools Command Prompt`.
   
   Then, `cd` to the build directory. 
  
8. Run CMake and NMake.
```
$ cmake -G "NMake Makefiles" ..
$ nmake
```
   The last argument of `cmake` should point to the source directory.
   If you created the build directory inside the source directory as suggested
   above, then `..` will point to the source directory.
   
   You may pass extra options to `cmake`:
   - `-DCMAKE_BUILD_TYPE=Debug` to build the debug version
     (release version is built by default).
   - `-DBUILD_SHARED_LIBS=ON` to also build DLL versions of the libraries
     (only static versions are built by default).

9. Locate the output files in the directories `bin`, `lib`, and `include`.

10. Run tests.
    The current directory should be the source directory.   
```
$ cd ..
$ build\bin\tests.exe
```
   If you built the shared versions of the libraries, run `tests_shared`,
   which runs the same tests as `tests`, but is linked to the dll versions
   of the libraries.
```
$ cd ..
$ build\bin\tests_shared.exe
```


Linux
-----

1. Make sure you have the requred packages.
   For Debian/Ubuntu these are:
   `g++`, `make`, `cmake`, `git`.
   CMake version 3.9 or higher is required.

2. Clone the repository and `cd` into it.
```bash
$ git clone https://gitlab.com/abakhirkin/StlEval.git
$ cd StlEval
```

3. Initialize and update git submodules. This will fetch a version of Googletest
   that is required to run tests.
```bash
$ git submodule init
$ git submodule update
```

6. Create a build directory, e.g., a directory `build` in the source tree and
   `cd` into it.
   You may name and place it differently; in this case you will need to adjust
   the paths given in these instructions.
```bash
$ mkdir build
$ cd build
```
  
5. Run CMake and Make.
```bash
$ cmake ..
$ make
```
   The last argument of `cmake` should point to the source directory.
   If you created the build directory inside the source directory as suggested
   above, then `..` will point to the source directory.
   
   You may pass extra options to `cmake`:
   - `-DCMAKE_BUILD_TYPE=Debug` to build the debug version
     (release version is built by default).
   - `-DBUILD_SHARED_LIBS=ON` to also build shared versions of the libraries
     (only static versions are built by default).

6. Locate the output files in the directories `bin`, `lib`, and `include`.

7. Run tests.
   The current directory should be the source directory `tests`.
```bash
$ cd ..
$ build/bin/tests
```
   If you built the shared versions of the libraries, run `tests_shared`,
   which runs the same tests as `tests`, but is linked to the shared versions
   of the libraries.
   The `libcstle.so.1` should be present in the linker search path.
```bash
$ cd ..
$ LD_LIBRARY_PATH="./build/lib:$LD_LIBRARY_PATH" build/bin/tests-shared
```
