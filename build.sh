#!/bin/bash

# exit when any command fails
set -e

# set project root directory
project_root=$(dirname $(realpath $0 ))

# init submodule if needed
if ! [ "$(ls -A $project_root/src/llvm-project)" ]; then
    echo "Cloning llvm-project submodule"
    pushd $project_root > /dev/null
    git submodule init
    git submodule update
    popd > /dev/null
fi

# create build directory
mkdir -p build && cd build

# compile llvm/clang
echo "Building Clang"
mkdir -p llvm && cd llvm
cmake -G "Ninja" $project_root/src/llvm-project/llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_PROJECTS="clang"
ninja clang
cd ..

# compile autocheck
echo "Building Autocheck"
cmake -G "Ninja" $project_root -DCLANG_DIR="$PWD/llvm/lib/cmake/clang"
ninja autocheck