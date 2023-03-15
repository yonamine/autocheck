# Supported AUTOSAR rules

| Rule | Text | Flag |
| ---  | ---  | ---  |
| A0-1-2 | The value returned by a function having a non-void return type that is not an overloaded operator shall be used. | unused-return-value |
| A0-1-3 | Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used. | unused-function-or-method |
| A0-1-6 | There should be no unused type declarations. | unused-typedef |
| A2-3-1 | Only those characters specified in the C++ Language Standard basic source character set shall be used in the source code. | non-cplusplus-standard-char-used |
| A2-5-1 | Trigraphs shall not be used. | trigraphs-used |
| A2-5-2 | Digraphs shall not be used. | digraphs-used |
| A2-7-1 | The character \ shall not occur as a last character of a C++ comment. | line-comment-last-char |
| M2-7-1 | The character sequence /* shall not be used within a C-style comment. | comment |
| M2-10-1 | Different identifiers shall be typographically unambiguous. | similar-identifiers |
| A2-10-6 | A class or enumeration name shall not be hidden by a variable, function or enumerator declaration in the same scope. | shadow-class-or-enum |
| A2-11-1 | Volatile keyword shall not be used. | volatile-keyword-used |
| A2-13-1 | Only those escape sequences that are defined in ISO/IEC 14882:2014 shall be used. | non-iso-escape-sequence |
| M2-13-2 | Octal constants (other than zero) and octal escape sequences (other than “\0” ) shall not be used. | ocatal-constant-used |
| A2-13-3 | Type wchar_t shall not be used. | type-wchart-used |
| M2-13-3 | A "U" suffix shall be applied to all octal or hexadecimal integer literals of unsigned type. | unsigned-literal-suffix |
| M2-13-4 | Literal suffixes shall be upper case. | literal-suffix-lower-case |
| A2-13-5 | Hexadecimal constants should be upper case. | hex-const-upper-case |
| A2-13-6 | Universal character names shall be used only inside character or string literals. | non-universal-names |
| A3-1-2 | Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx". | header-extension |
| A3-1-4 | When an array with external linkage is declared, its size shall be stated explicitly. | extern-array-implicit-size |
| A3-3-2 | Static and thread-local objects shall be constant-initialized. | constant-initializer |
| M3-4-1 | An identifier declared to be an object or type shall be defined in a block that minimizes its visibility. | broad-scope-identifier |
| A3-9-1 | Fixed width integer types from \<cstdint\>, indicating the size and signedness, shall be used in place of the basic numerical types. | fixed-width-integer-types |
| A4-5-1 | Expressions with type enum or enum class shall not be used as operands to built-in and overloaded operators other than the subscript operator [ ], the assignment operator =, the equality operators == and !=, the unary & operator, and the relational operators \<, \<=, \>, \>=. | invalid-enum-expression |
| M4-5-1 | Expressions with type bool shall not be used as operands to built-in operators other than the assignment operator =, the logical operators &&, \|\|, !, the equality operators == and !=, the unary & operator, and the conditional operator. | invalid-bool-expression |
| M4-5-3 | Expressions with type (plain) char and wchar_t shall not be used as operands to built-in operators other than the assignment operator =, the equality operators == and !=, and the unary & operator. | invalid-char-expression |
| A4-10-1 | Only nullptr literal shall be used as the null-pointer-constant. | nullptr-only-null-ptr-const |
| M4-10-1 | NULL shall not be used as an integer value. | null-to-integer-value |
| M4-10-2 | Literal zero (0) shall not be used as the null-pointer-constant. | zero-to-null-pointer |
| A5-0-2 | The condition of an if-statement and the condition of an iteration statement shall have type bool. | condition-not-bool |
| A5-0-3 | The declaration of objects shall contain no more than two levels of pointer indirection. | pointer-indirection-levels |
| M5-0-5 | There shall be no implicit floating-integral conversions. | implicit-float-integral-conversion |
| M5-0-6 | An implicit integral or floating-point conversion shall not reduce the size of the underlying type. | implicit-size-reduction-conversion |
| M5-0-10 | If the bitwise operators ~and \<\< are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand. | implicit-bitwise-result-conversion |
| M5-0-11 | The plain char type shall only be used for the storage and use of character values. | char-storage |
| M5-0-12 | Signed char and unsigned char type shall only be used for the storage and use of numeric values. | sign-char-storage |
| M5-0-20 | Non-constant operands to a binary bitwise operator shall have the same underlying type. | implicit-bitwise-binop-conversion |
| A5-1-2 | Variables shall not be implicitly captured in a lambda expression. | implicitly-captured-var |
| A5-1-6 | Return type of a non-void return type lambda expression should be explicitly specified. | lambda-return-type-unspecified |
| A5-1-8 | Lambda expressions should not be defined inside another lambda expression. | nested-lambda |
| A5-2-1 | dynamic_cast should not be used. | dynamic-cast-used |
| A5-2-4 | reinterpret_cast shall not be used. | reinterpret-cast-used |
| M5-2-10 | The increment (++) and decrement (−−) operators shall not be mixed with other operators in an expression. | inc-dec-op-mixed |
| M5-2-12 | An identifier with array type passed as a function argument shall not decay to a pointer. | array-decays-to-pointer |
| M5-3-4 | Evaluation of the operand to the sizeof operator shall not contain side effects. | sizeof-side-effect |
| A5-7-1 | A lambda shall not be an operand to decltype or typeid. | lambda-decltype-typeid |
| A5-16-1 | The ternary conditional operator shall not be used as a sub-expression. | ternary-op-sub-expr |
| M6-2-1 | Assignment operators shall not be used in sub-expressions. | assignment-op-sub-expr |
| M6-2-3 | Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character. | null-stmt-not-alone |
| M6-4-2 | All if ... else if constructs shall be terminated with an else clause. | if-else-if-terminated |
| M6-4-6 | The final clause of a switch statement shall be the default-clause. | switch-stmt-default-clause |
| A6-5-1 | A for-loop that loops through all elements of the container and does not use its loop-counter shall not be used. | container-loop-ill-formed |
| A6-5-2 | A for loop shall contain a single loop-counter which shall not have floating-point type. | single-loop-counter-float-type |
| M6-5-2 | If loop-counter is not modified by −− or ++, then, within condition, the loop-counter shall only be used as an operand to \<=, \<, \> or \>=. | cond-expr-inc-dec-loop |
| A6-5-3 | Do statements should not be used. | do-while-used |
| M6-5-3 | The loop-counter shall not be modified within condition or statement. | loop-counter-modified |
| A6-5-4 | For-init-statement and expression should not perform actions other than loop-counter initialization and modification. | init-inc-parts-loop-counter |
| M6-5-4 | The loop-counter shall be modified by one of: −−, ++, −= n, or += n; where n remains constant for the duration of the loop. | loop-counter-ill-modified |
| M6-5-5 | A loop-control-variable other than the loop-counter shall not be modified within condition or expression. | non-loop-counter-modified |
| M6-5-6 | A loop-control-variable other than the loop-counter which is modified in statement shall have type bool. | non-loop-counter-bool |
| A6-6-1 | The goto statement shall not be used. | goto-used |
| M6-6-1 | Any label referenced by a goto statement shall be declared in the same block, or in a block enclosing the goto statement. | goto-label-block |
| M6-6-3 | The continue statement shall only be used within a well-formed for loop. | continue-stmt-well-formed-loop |
| A7-1-1 | Constexpr or const specifiers shall be used for immutable data declaration. | const-unused-for-immutable-data |
| A7-1-3 | CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name. | cv-qualifiers-placed-left |
| A7-1-6 | The typedef specifier shall not be used. | typedef-used |
| A7-1-7 | Each expression statement and identifier declaration shall be placed on a separate line. | separate-line-expression-statement |
| A7-2-2 | Enumeration underlying base type shall be explicitly defined. | enum-type-not-defined |
| M7-3-6 | Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files. | using-inside-header |
| A7-4-1 | The asm declaration shall not be used. | asm-declaration-used |
| M8-3-1 | Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments. | changed-default-arguments |
| A8-4-1 | Functions shall not be defined using the ellipsis notation. | variadic-function-used |
| M8-4-2 | The identifiers used for the parameters in a re-declaration of a function shall be identical to those in the declaration. | function-redecl-params |
| A8-4-7 | "in" parameters for "cheap to copy" types shall be passed by value. | in-parm-passed-by-value |
| A8-5-2 | Braced-initialization {}, without equals sign, shall be used for variable initialization. | non-braced-init-used |
| M8-5-2 | Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures. | missing-braces-or-elements |
| A8-5-3 | A variable of type auto shall not be initialized using {} or ={} braced-initialization. | auto-var-braced-init |
| M9-3-1 | Const member functions shall not return non-const pointers or references to class-data. | non-const-returned-from-const |
| A9-5-1 | Unions shall not be used. | unions-used |
| M10-1-1 | Classes should not be derived from virtual bases. | derived-from-virtual |
| A10-3-1 | Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final. | virtual-func-specifier |
| A10-3-2 | Each overriding virtual function shall be declared with the override or final specifier. | override-func-specifier |
| A10-3-3 | Virtual functions shall not be introduced in a final class. | virtual-method-final-class |
| M10-3-3 | A virtual function shall only be overridden by a pure virtual function if it is itself declared as pure virtual. | pure-func-override |
| A11-0-1 | A non-POD type should be defined as class. | non-pod-as-class |
| M11-0-1 | Member data in non-POD class types shall be private. | member-data-private |
| A11-3-1 | Friend declarations shall not be used. | friend-used |
| A12-1-2 | Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type. | nsdmi-and-ctor-init-used |
| A12-1-4 | All constructors that are callable with a single argument of fundamental type shall be declared explicit. | explicit-ctors |
| A12-4-2 | If a public destructor of a class is non-virtual, then the class should be declared final. | final-class-destructor |
| A12-8-2 | User-defined copy and move assignment operators should use user-defined no-throw swap function. | swap-not-in-copy-and-move-assignment |
| A12-8-5 | A copy assignment and a move assignment operators shall handle self-assignment. | self-assignment-unhandled |
| A13-2-1 | An assignment operator shall return a reference to "this". | assignment-operator-return-type |
| A13-2-3 | A relational operator shall return a boolean value. | rel-op-bool-value |
| A13-5-1 | If “operator[]” is to be overloaded with a non-const version, const version shall also be implemented. | subscript-op-overload |
| A13-5-2 | All user-defined conversion operators shall be defined explicit. | implicit-conversion-op |
| A13-5-3 | User-defined conversion operators should not be used. | conversion-op-used |
| A13-6-1 | Digit sequences separators ’ shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits. | digit-sequence-separator |
| A15-4-1 | Dynamic exception-specification shall not be used. | deprecated-dynamic-exception-spec |
| A15-4-2 | If a function is declared to be noexcept, noexcept(true) or noexcept(\<true condition\>), then it shall not exit with an exception. | throw-in-noexcept-func |
| A15-5-1 | All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception. A noexcept exception specification shall be added to these functions as appropriate. | ctors-dtors-deallocation-move-swap-noexcept |
| M16-0-5 | Arguments to a function-like macro shall not contain tokens that look like pre-processing directives. | embedded-directive |
| M16-0-8 | If the # token appears as the first token on a line, then it shall be immediately followed by a pre-processing token. | preprocessor-token-error |
| M16-1-1 | The defined pre-processor operator shall only be used in one of the two standard forms. | expansion-to-defined |
| A16-2-1 | The ’, ", /*, //, \ characters shall not occur in a header file name or in #include directive. | include-directive-restricted-char |
| A16-2-2 | There shall be no unused include directives. | headers-unused |
| M16-3-1 | There shall be at most one occurrence of the # or ## operators in a single macro definition. | both-hash-op-used |
| M16-3-2 | The # and ## operators should not be used. | hash-hash-op-used |
| A16-6-1 | #error directive shall not be used. | directive-error-used |
| A16-7-1 | The #pragma directive shall not be used. | pragma-directive-used |
| A17-0-1 | Reserved identifiers, macros and functions in the C++ standard library shall not be defined, redefined or undefined. | reserved-identifiers |
| M17-0-5 | The setjmp macro and the longjmp function shall not be used. | setjmp-longjmp-used |
| A18-0-1 | The C library facilities shall only be accessed through C++ library headers. | clib-header-used |
| A18-0-3 | The library \<clocale\> (locale.h) and the setlocale function shall not be used. | locale-header-used |
| M18-0-3 | The library functions abort, exit, getenv and system from library \<cstdlib\> shall not be used. | cstdlib-func-used |
| M18-0-4 | The time handling functions of library \<ctime\> shall not be used. | ctime-used |
| M18-0-5 | The unbounded functions of library \<cstring\> shall not be used. | cstring-func-used |
| A18-1-1 | C-style arrays shall not be used. | c-style-array-used |
| A18-1-2 | The std::vector\<bool\> specialization shall not be used. | bool-vector-specialization-used |
| M18-2-1 | The macro offsetof shall not be used. | offsetof-used |
| A18-5-1 | Functions malloc, calloc, realloc and free shall not be used. | cstdlib-memfunc-used |
| A18-5-3 | The form of delete operator shall match the form of new operator used to allocate the memory. | mismatched-new-delete |
| M18-7-1 | The signal handling facilities of \<csignal\> shall not be used. | csignal-used |
| A18-9-1 | The std::bind shall not be used. | std-bind-used |
| A18-9-3 | The std::move shall not be used on objects declared const or const&. | std-move-used-const |
| M19-3-1 | The error indicator errno shall not be used. | errno-used |
| A21-8-1 | Arguments to character-handling functions shall be representable as an unsigned char. | character-handling-functions |
| A26-5-1 | Pseudorandom numbers shall not be generated using std::rand(). | std-rand-used |
| A26-5-2 | Random number engines shall not be default-initialized. | random-engine-default-initialized |
| M27-0-1 | The stream input/output library \<cstdio\> shall not be used. | cstdio-used |
| A27-0-4 | C-style strings shall not be used. | c-style-string-used |
