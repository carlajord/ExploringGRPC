# Intro

Creates a Visual Studio project with protos and source files.

# Requirements

Requires CMake (https://cmake.org/documentation/) and Conan (https://docs.conan.io/en/latest/) installed

# Setup 

Add center.conan.io to the list of conan remote. The grpc package and its dependencies will come from this source.
``` 
conan remote add conan-center https://center.conan.io
``` 

# Build

- create build directory
``` 
mkdir build && cd build
```
- install conan libraries
``` 
conan install .. -s build_type=Debug --build=missing -r conan-center
``` 
- run cmake and build
``` 
cmake ..
cmake --build .
``` 