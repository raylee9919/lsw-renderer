#!/bin/bash


# Stop at errors.
set -e

Compiler=g++
CC="-O0 -g -ggdb -std=c++17 -fno-exceptions -Wall -Wextra -Wno-unused-function"
#-msse4.2 -maes

CurDir=$(pwd)
BuildDir="$CurDir/../build"
DataDir="$CurDir/../data"

[ -d $BuildDir ] || mkdir -p $BuildDir
[ -d $DataDir ] || mkdir -p $DataDir

pushd $BuildDir > /dev/null

echo Building LINUX testbed.
$Compiler $CC "$CurDir/linux.cpp" -o linux -lX11

echo Build Completed.
popd > /dev/null
