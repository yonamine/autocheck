# Generating a compilation database

A compilation database (`compile_commands.json`) is required to find all dependencies and includes in a project.
How to generate a compilation database depends on the build system the project is using.
For standalone files that depend only on the standard library this is not required.
For projects with multiple files or dependencies look for instructions in the section for your build system.

## CMake

CMake supports generating `compile_commands.json` out of the box. Just add `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` flag to your cmake command.

## Bazel

You can use [this extension](https://github.com/hedronvision/bazel-compile-commands-extractor) to generate `compile_commands.json`, just follow the instructions in their README.

## Meson

Meson generates `compile_commands.json` in the build directory by default, there is nothing more to do.

## Make

You can use [Bear](https://github.com/rizsotto/Bear) to generate `compile_commands.json` from a Makefile.
After installing it, run the following commands:
```console
make clean
bear -- make
```
The compilation database will be generated in the same directory you executed the command in.

*Note: Bear uses operating system features to record executed commands and uses that information to generate the file, so if the build breaks at any point the generated compile commands will not be complete and you will need to rebuild.*

## Other build systems

See the [Make section](#Make), just substitute `make` commands with the equivalent command for your build system.
This may not work for all build systems.
