// RUN: autocheck -verify -Wfunction-redecl-params %s

namespace {
void function1(bool Param1, bool Param2);
void function1(bool Param2, bool Param1); // expected-warning 1 {{The identifiers used for the parameters in a re-declaration of a function shall be identical to those in the declaration}}
void function1(bool Param1, bool Param2);

void function1(bool Param1, bool Param2) {}

void function2(bool Param1, bool Param2);
void function2(bool, bool);
void function2(bool, bool Param1); // expected-warning 1 {{The identifiers used for the parameters in a re-declaration of a function shall be identical to those in the declaration}}

void function2(bool Param1, bool Param2) {}
} // namespace
