# autocheck
Following AUTOSAR guidelines can be easy and free!

## Install dependencies

- C++ compiler (e.g. g++, clang++, ...)
- git
- cmake
- ninja
- python 3 (for tests)

## Clone the repo

Since we use ```llvm-project``` as submodule, we need to clone the submodule as well:

```
$ git clone --recurse-submodules git@github.com:syrmia/autocheck.git
```
As well as to ```pull``` the code, we need to do it as follows:

```
$ git pull --recurse-submodules
```

## Automated build

Run the `build.sh` script located in the project root directory to automatically initialize the submodule, build llvm and build autocheck:

```console
$ ./autocheck/build.sh
```

After the first build, you can just run `ninja` or `ninja autocheck` to compile again

## Manual build

1. Create a build directory
    ```console
    $ mkdir build && cd build
    ```
2. Build LLVM/Clang
    ```console
    $ mkdir llvm && cd llvm
    $ cmake -G "Ninja" ../../autocheck/src/llvm-project/llvm \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_TARGETS_TO_BUILD="X86" \
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_ENABLE_PROJECTS="clang"
    $ ninja
    $ cd ..
    ```
3. Build autocheck
    ```console
    $ cmake -G "Ninja" ../autocheck -DCLANG_DIR="$PWD/llvm/lib/cmake/clang"
    $ ninja autocheck
    ```

After the first build, you can just run `ninja` or `ninja autocheck` to compile again

## Run the tool

From the build directory run:
```console
$ ./bin/autocheck 
=== Autocheck - Modern and Free Autosar checker
```

## Run tests

From the build directory run:
```console
$ ninja check-autocheck
```
