// RUN: autocheck -verify -Wpragma-directive-used %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#pragma once // expected-warning 1 {{The #pragma directive shall not be used}}
