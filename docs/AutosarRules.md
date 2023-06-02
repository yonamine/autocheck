# Supported AUTOSAR rules

| Rule | Text | Flag |
| ---  | ---  | ---  |
| M0-1-1 | A project shall not contain unreachable code. | unreachable-code |
| A0-1-2 | The value returned by a function having a non-void return type that is not an overloaded operator shall be used. | unused-return-value |
| A0-1-3 | Every function defined in an anonymous namespace, or static function with internal linkage, or private member function shall be used. | unused-function-or-method |
| M0-1-3 | A project shall not contain unused variables. | unused-variable |
| A0-1-4 | There shall be no unused named parameters in non-virtual functions. | unused-parameter |
| A0-1-6 | There should be no unused type declarations. | unused-typedef |
| M0-1-10 | Every defined function should be called at least once. | unused-function |
| A0-4-2 | Type long double shall not be used. | long-double-used |
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
| A2-13-4 | String literals shall not be assigned to non-constant pointers. | writable-strings |
| M2-13-4 | Literal suffixes shall be upper case. | literal-suffix-lower-case |
| A2-13-5 | Hexadecimal constants should be upper case. | hex-const-upper-case |
| A2-13-6 | Universal character names shall be used only inside character or string literals. | non-universal-names |
| A3-1-2 | Header files, that are defined locally in the project, shall have a file name extension of one of: ".h", ".hpp" or ".hxx". | header-extension |
| M3-1-2 | Functions shall not be declared at block scope. | function-at-block-scope |
| A3-1-4 | When an array with external linkage is declared, its size shall be stated explicitly. | extern-array-implicit-size |
| A3-3-2 | Static and thread-local objects shall be constant-initialized. | constant-initializer |
| M3-3-2 | If a function has internal linkage then all re-declarations shall include the static storage class specifier. | static-function-redeclaration |
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
| M5-0-4 | An implicit integral conversion shall not change the signedness of the underlying type. | impcast-changes-signedness |
| M5-0-5 | There shall be no implicit floating-integral conversions. | implicit-float-integral-conversion |
| M5-0-6 | An implicit integral or floating-point conversion shall not reduce the size of the underlying type. | implicit-size-reduction-conversion |
| M5-0-10 | If the bitwise operators ~and \<\< are applied to an operand with an underlying type of unsigned char or unsigned short, the result shall be immediately cast to the underlying type of the operand. | implicit-bitwise-result-conversion |
| M5-0-11 | The plain char type shall only be used for the storage and use of character values. | char-storage |
| M5-0-12 | Signed char and unsigned char type shall only be used for the storage and use of numeric values. | sign-char-storage |
| M5-0-14 | The first operand of a conditional-operator shall have type bool. | conditional-op-operand-type |
| M5-0-16 | A pointer operand and any pointer resulting from pointer arithmetic using that operand shall both address elements of the same array. | pointer-sub |
| M5-0-20 | Non-constant operands to a binary bitwise operator shall have the same underlying type. | implicit-bitwise-binop-conversion |
| M5-0-21 | Bitwise operators shall only be applied to operands of unsigned underlying type. | bitwise-operand-not-unsigned |
| A5-1-2 | Variables shall not be implicitly captured in a lambda expression. | implicitly-captured-var |
| A5-1-3 | Parameter list (possibly empty) shall be included in every lambda expression. | lambda-declarator-missing |
| A5-1-6 | Return type of a non-void return type lambda expression should be explicitly specified. | lambda-return-type-unspecified |
| A5-1-8 | Lambda expressions should not be defined inside another lambda expression. | nested-lambda |
| A5-2-1 | dynamic_cast should not be used. | dynamic-cast-used |
| A5-2-2 | Traditional C-style casts shall not be used. | c-style-cast-used |
| A5-2-3 | A cast shall not remove any const or volatile qualification from the type of a pointer or reference. | cast-removes-qual |
| A5-2-4 | reinterpret_cast shall not be used. | reinterpret-cast-used |
| A5-2-5 | An array or container shall not be accessed beyond its range. | array-bounds |
| M5-2-9 | A cast shall not convert a pointer type to an integral type. | cast-ptr-to-integral-type |
| M5-2-10 | The increment (++) and decrement (−−) operators shall not be mixed with other operators in an expression. | inc-dec-op-mixed |
| M5-2-11 | The comma operator, && operator and the \|\| operator shall not be overloaded. | comma-and-or-ops-overloaded |
| M5-2-12 | An identifier with array type passed as a function argument shall not decay to a pointer. | array-decays-to-pointer |
| M5-3-1 | Each operand of the ! operator, the logical && or the logical \|\| operators shall have type bool. | not-and-or-ops-bool-operands |
| A5-3-2 | Null pointers shall not be dereferenced. | null-dereference |
| M5-3-2 | The unary minus operator shall not be applied to an expression whose underlying type is unsigned. | unary-minus-on-unsigned-type |
| A5-3-3 | Pointers to incomplete class types shall not be deleted. | delete-incomplete |
| M5-3-3 | The unary & operator shall not be overloaded. | unary-amp-op-overloaded |
| M5-3-4 | Evaluation of the operand to the sizeof operator shall not contain side effects. | sizeof-side-effect |
| A5-6-1 | The right hand operand of the integer division or remainder operators shall not be equal to zero. | div-by-zero |
| A5-1-7 | A lambda shall not be an operand to decltype or typeid. | lambda-decltype-typeid |
| M5-14-1 | The right hand operand of a logical &&, \|\| operators shall not contain side effects. | rhs-operand-and-or-side-effect |
| A5-16-1 | The ternary conditional operator shall not be used as a sub-expression. | ternary-op-sub-expr |
| M5-18-1 | The comma operator shall not be used. | comma-operator-used |
| M6-2-1 | Assignment operators shall not be used in sub-expressions. | assignment-op-sub-expr |
| M6-2-3 | Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character. | null-stmt-not-alone |
| M6-3-1 | The statement forming the body of a switch, while, do ... while or for statement shall be a compound statement. | not-compound-statement |
| M6-4-1 | An if ( condition ) construct shall be followed by a compound statement. The else keyword shall be followed by either a compound statement, or another if statement. | if-followed-by-compound-stmt |
| M6-4-2 | All if ... else if constructs shall be terminated with an else clause. | if-else-if-terminated |
| M6-4-5 | An unconditional throw or break statement shall terminate every non-empty switch-clause. | break-switch-case |
| M6-4-6 | The final clause of a switch statement shall be the default-clause. | switch-stmt-default-clause |
| M6-4-7 | The condition of a switch statement shall not have bool type. | switch-bool |
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
| M6-6-2 | The goto statement shall jump to a label declared later in the same function body. | goto-back-jump |
| M6-6-3 | The continue statement shall only be used within a well-formed for loop. | continue-stmt-well-formed-loop |
| A7-1-1 | Constexpr or const specifiers shall be used for immutable data declaration. | const-unused-for-immutable-data |
| A7-1-3 | CV-qualifiers shall be placed on the right hand side of the type that is a typedef or a using name. | cv-qualifiers-placed-left |
| A7-1-4 | The register keyword shall not be used. | register-keyword-used |
| A7-1-6 | The typedef specifier shall not be used. | typedef-used |
| A7-1-7 | Each expression statement and identifier declaration shall be placed on a separate line. | separate-line-expression-statement |
| A7-2-2 | Enumeration underlying base type shall be explicitly defined. | enum-type-not-defined |
| A7-2-3 | Enumerations shall be declared as scoped enum classes. | enum-not-scoped-enum-class |
| A7-2-4 | In an enumeration, either (1) none, (2) the first or (3) all enumerators shall be initialized. | enumerator-initialization |
| M7-3-1 | The global namespace shall only contain main, namespace declarations and extern "C" declarations. | global-namespace-nodes |
| M7-3-2 | The identifier main shall not be used for a function other than the global function main. | identifier-main-reused |
| M7-3-3 | There shall be no unnamed namespaces in header files. | unnamed-namespace-in-header |
| M7-3-4 | Using-directives shall not be used. | using-directive-used |
| M7-3-6 | Using-directives and using-declarations (excluding class scope or function scope using-declarations) shall not be used in header files. | using-inside-header |
| A7-4-1 | The asm declaration shall not be used. | asm-declaration-used |
| M7-5-1 | A function shall not return a reference or a pointer to an automatic variable (including parameters), defined within the function. | return-stack-address |
| A7-5-2 | Functions shall not call themselves, either directly or indirectly. | recursion-used |
| A7-6-1 | Functions declared with the [[noreturn]] attribute shall not return. | invalid-noreturn |
| M8-3-1 | Parameters in an overriding virtual function shall either use the same default arguments as the function they override, or else shall not specify any default arguments. | changed-default-arguments |
| A8-4-1 | Functions shall not be defined using the ellipsis notation. | variadic-function-used |
| A8-4-2 | All exit paths from a function with non-void return type shall have an explicit return statement with an expression. | return-nonvoid-function  |
| M8-4-2 | The identifiers used for the parameters in a re-declaration of a function shall be identical to those in the declaration. | function-redecl-params |
| A8-4-7 | "in" parameters for "cheap to copy" types shall be passed by value. | in-param-passed-by-value |
| A8-5-0 | All memory shall be initialized before it is read. | uninitialized-memory |
| A8-5-2 | Braced-initialization {}, without equals sign, shall be used for variable initialization. | non-braced-init-used |
| M8-5-2 | Braces shall be used to indicate and match the structure in the non-zero initialization of arrays and structures. | missing-braces-or-elements |
| A8-5-3 | A variable of type auto shall not be initialized using {} or ={} braced-initialization. | auto-var-braced-init |
| M9-3-1 | Const member functions shall not return non-const pointers or references to class-data. | non-const-returned-from-const |
| A9-5-1 | Unions shall not be used. | unions-used |
| A9-6-1 | Bit-fields shall be either unsigned integral, or enumeration (with underlying type of unsigned integral type). | bit-field-unsigned-integral |
| M10-1-1 | Classes should not be derived from virtual bases. | derived-from-virtual |
| A10-3-1 | Virtual function declaration shall contain exactly one of the three specifiers: (1) virtual, (2) override, (3) final. | virtual-func-specifier |
| A10-3-2 | Each overriding virtual function shall be declared with the override or final specifier. | override-func-specifier |
| A10-3-3 | Virtual functions shall not be introduced in a final class. | virtual-method-final-class |
| M10-3-3 | A virtual function shall only be overridden by a pure virtual function if it is itself declared as pure virtual. | pure-func-override |
| A10-3-5 | A user-defined assignment operator shall not be virtual. | assignment-op-virtual |
| A11-0-1 | A non-POD type should be defined as class. | non-pod-as-class |
| M11-0-1 | Member data in non-POD class types shall be private. | member-data-private |
| A11-0-2 | A type defined as struct shall: (1) provide only public data members, (2) not provide any special member functions or methods, (3) not be a base of another struct or class, (4) not inherit from another struct or class. | proper-structure |
| A11-3-1 | Friend declarations shall not be used. | friend-used |
| A12-1-2 | Both NSDMI and a non-static member initializer in a constructor shall not be used in the same type. | nsdmi-and-ctor-init-used |
| A12-1-4 | All constructors that are callable with a single argument of fundamental type shall be declared explicit. | explicit-ctors |
| A12-4-1 | Destructor of a base class shall be public virtual, public override or protected non-virtual. | base-destructor |
| A12-4-2 | If a public destructor of a class is non-virtual, then the class should be declared final. | final-class-destructor |
| A12-8-2 | User-defined copy and move assignment operators should use user-defined no-throw swap function. | swap-not-in-copy-and-move-assignment |
| A12-8-5 | A copy assignment and a move assignment operators shall handle self-assignment. | self-assignment-unhandled |
| A12-8-7 | Assignment operators should be declared with the ref-qualifier &. | assignment-op-ref-qual |
| A13-2-1 | An assignment operator shall return a reference to "this". | assignment-operator-return-type |
| A13-2-3 | A relational operator shall return a boolean value. | rel-op-bool-value |
| A13-5-1 | If “operator[]” is to be overloaded with a non-const version, const version shall also be implemented. | subscript-op-overload |
| A13-5-2 | All user-defined conversion operators shall be defined explicit. | implicit-conversion-op |
| A13-5-3 | User-defined conversion operators should not be used. | conversion-op-used |
| A13-6-1 | Digit sequences separators ' shall only be used as follows: (1) for decimal, every 3 digits, (2) for hexadecimal, every 2 digits, (3) for binary, every 4 digits. | digit-sequence-separator |
| A15-1-2 | An exception object shall not be a pointer. | exception-object-is-pointer |
| M15-1-2 | NULL shall not be thrown explicitly. | null-thrown-explicitly |
| M15-3-4 | Each exception explicitly thrown in the code shall have a handler of a compatible type in all call paths that could lead to that point. | handler-of-compatible-type |
| M15-3-6 | Where multiple handlers are provided in a single try-catch statement or function-try-block for a derived class and some or all of its bases, the handlers shall be ordered most-derived to base class. | exception-handler-inversion |
| A15-4-1 | Dynamic exception-specification shall not be used. | deprecated-dynamic-exception-spec |
| A15-4-2 | If a function is declared to be noexcept, noexcept(true) or noexcept(\<true condition\>), then it shall not exit with an exception. | throw-in-noexcept-func |
| A15-5-1 | All user-provided class destructors, deallocation functions, move constructors, move assignment operators and swap functions shall not exit with an exception. A noexcept exception specification shall be added to these functions as appropriate. | ctors-dtors-deallocation-move-swap-noexcept |
| M16-0-5 | Arguments to a function-like macro shall not contain tokens that look like pre-processing directives. | embedded-directive |
| M16-0-7 | Undefined macro identifiers shall not be used in #if or #elif pre-processor directives, except as operands to the defined operator. | undef-macro-used |
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
| A18-1-3 | The std::auto_ptr type shall not be used. | type-auto-ptr-used |
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

## Notes

- Rule A16-2-2 (There shall be no unused include directives) has three implementations activated with the following flags: 
  1. `headers-unused` - Basic version that follows the rule as stated. This is the default implementation.
  2. `headers-unused-system-off` - Ignores checking of system headers.
  3. `headers-unused-system-smart` - Checks system headers but does not report a warning if header itself is not used but one or more of it’s own #include directives is.
  Only one of these can be active at a time. If multiple flags are specified, the last one will be used.
- Some rules are implemented using static analysis.
  These are more expensive to run so they are not included by default or under the `-Wall` flag.
  Instead they can all be using the `-Wanalysis` flag.
  Those rules are:
  - [M0-1-1] A project shall not contain unreachable code.
  - [M5-0-16] A pointer operand and any pointer resulting from pointer arithmetic using that operand shall both address elements of the same array.
  - [A5-3-2] Null pointers shall not be dereferenced.
  - [A5-6-1] The right hand operand of the integer division or remainder operators shall not be equal to zero.
  - [A7-5-2] Functions shall not call themselves, either directly or indirectly.
