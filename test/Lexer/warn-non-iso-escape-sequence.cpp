// RUN: autocheck -verify -Wnon-iso-escape-sequence %s
// RUN: autocheck -verify=expected,autosar -Wall %s

#include <string>

void test1() {
    std::string str;

    str = "\c"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\d"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\g"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\h"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}

    char chr;

    chr = '\c'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    chr = '\d'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    chr = '\g'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    chr = '\h'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
}

void test2() {
    std::string str;

    str = "\e"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\E"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}

    char chr;

    chr = '\e'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    chr = '\E'; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
}

void test3() {
    std::string str;

    str = "\'";
    str = "\"";
    str = "\?";
    str = "\\";

    str = "\a";
    str = "\b";
    str = "\f";
    str = "\n";
    str = "\r";
    str = "\t";
    str = "\v";

    str = "\0123";
    str = "\x4";

    str = "\u12ab";
    str = "\U0001abcd";

    char chr;

    chr = '\'';
    chr = '\"';
    chr = '\?';
    chr = '\\';

    chr = '\a';
    chr = '\b';
    chr = '\f';
    chr = '\n';
    chr = '\r';
    chr = '\t';
    chr = '\v';

    chr = '\012';
    chr = '\x4';
}

int main() {
    return 0;
}

// autosar-warning@4 0-1 {{There shall be no unused include directives:}} // libstdc++ shows this warning, libc++ does not
// autosar-note@4 0-1 {{But one or more of it's own #include directives is used}} // libstdc++ shows this note, libc++ does not
// autosar-warning@6 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@22 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@34 1 {{The global namespace shall only contain main, namespace declarations and extern "C" declarations}}
// autosar-warning@50 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@71 1 {{Octal constants (other than zero) and octal escape sequences (other than "\0") shall not be used}}
// autosar-warning@14 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@28 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
// autosar-warning@56 1 {{Fixed width integer types from <cstdint>, indicating the size and signedness, shall be used in place of the basic numerical types}}
