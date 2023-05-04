// RUN: autocheck -verify -Wcomment %s
// RUN: autocheck -verify=expected,autosar -Wall %s

namespace {
    /* some comment, end comment marker accidentally omitted
    Perform_Critical_Safety_Function(X);
    /* this "comment" is Non-compliant */

    /* comments comments
        int i = 1;
        printf("Hello world");
        while(1)
            i++;
    /*
    */

    /*/*
        int i ;
    */

    /*
    /**/

    /*/*/
} // namespace

// expected-warning@7 1 {{The character sequence /* shall not be used within a C-style comment}}
// expected-warning@14 1 {{The character sequence /* shall not be used within a C-style comment}}
// expected-warning@17 1 {{The character sequence /* shall not be used within a C-style comment}}
// expected-warning@22 1 {{The character sequence /* shall not be used within a C-style comment}}
