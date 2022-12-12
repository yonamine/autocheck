# autocheck
Following AUTOSAR guidelines can be easy and free!

## Clone the repo

Since we use ```llvm-project``` as submodule, we need to clone the submodule as well:

```
$ git clone --recurse-submodules
```
As well as to ```pull``` the code, we need to do it as follows:

```
$ git pull --recurse-submodules
```
## Build the tool

```
$ mkdir build && cd build
$ cmake -G "Ninja" ../autocheck/ -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_DIR="${path_to_the_build_dir_we_created_above}/build/src/llvm-project/llvm/lib/cmake/llvm"
$ ninja autocheck
```

Please do note that ```$ ninja``` will build whole LLVM, so we recommend using ```$ ninja autocheck```.

## Run the tool

```
$ ./bin/autocheck 
=== Autocheck - Modern and Free Autosar checker
```
