// RUN: autocheck -verify -Wpragma-directive-used %s

#pragma once // expected-warning 1 {{The #pragma directive shall not be used}}
