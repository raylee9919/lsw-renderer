#!/bin/bash


# Stop at errors.
set -e

Compiler=g++
CC="-O0 -g -ggdb -std=c++17 -fno-exceptions -I../src/vendor -Wall -Wextra -Wno-unused-function"
#-msse4.2 -maes

CurDir=$(pwd)
BuildDir="$CurDir/../build"
DataDir="$CurDir/../data"

[ -d $BuildDir ] || mkdir -p $BuildDir
[ -d $DataDir ] || mkdir -p $DataDir

pushd $BuildDir > /dev/null

echo Building Vulkan so...
$Compiler $CC -shared -fPIC "$CurDir/linux_vulkan.cpp" -o renderer_vulkan.so -lX11 -lvulkan

echo Building LINUX testbed...
$Compiler $CC "$CurDir/linux.cpp" -o linux -lX11

echo Build Completed.
popd > /dev/null
