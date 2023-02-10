// RUN: autocheck -verify -Wnull-stmt-not-alone %s

#define MACRO_DEFINE

namespace {
void test() {
  ;
  ; // Comment
  ; /* Comment */
  ; // Comment
  ; /* Comment */

  ;;
  ; ;
  ;// Comment
  ;/* Comment */
  /* Comment */;
  /* Comment */ ;// Comment
  /* Comment */ ;
  MACRO_DEFINE ;
  ; MACRO_DEFINE
  ; 
  ;  
}
}

// expected-warning@13 2 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@14 2 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@15 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@16 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@17 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@18 2 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@19 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@20 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@21 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@22 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
// expected-warning@23 1 {{Before preprocessing, a null statement shall only occur on a line by itself; it may be followed by a comment, provided that the first character following the null statement is a white-space character}}
