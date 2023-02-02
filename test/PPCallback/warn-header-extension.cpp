// RUN: autocheck -verify -Wheader-extension %s

#include <iostream>
#include <string>
#include "HeaderExtension/1.h"
#include "HeaderExtension/1.hpp"
#include "HeaderExtension/1.hxx"
#include "HeaderExtension/1.hc" // expected-warning {{Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx"}}
#include "HeaderExtension/1.hcl" // expected-warning {{Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx"}}
