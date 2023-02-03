// RUN: autocheck -verify -Wnon-iso-escape-sequence %s

#include <string>

void test1() {
    std::string str;

    str = "\c"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\d"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\g"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\h"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
}

void test2() {
    std::string str;

    str = "\e"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
    str = "\E"; // expected-warning {{Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used}}
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
}

int main() {
    return 0;
}
