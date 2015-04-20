%{  
  #include <iostream>  
  #include "attrval.h"
  #include "db/db_worker.h"
  #include "access_control.h"
  #include "permission.h"

  #include "expression/comparison.h"
  #include "expression/comparison_predicate.h"
  #include "expression/predicate.h"
  #include "expression/predicate_tree.h"
  #include "expression/disjunction_converter.h" 
  #include "acyy.hpp"
  using namespace std;

%}  
%%
[-+]?[0-9]+        { yylval.iv = strtol(yytext,0,10); cout << "int" ;return TINT; }  
[-+]?(\d+(\.\d*)?|\.\d+)[dD]? {yylval.dv=strtol(yytext, 0, 10); return TDBL;}
[ \t\r\n]         ; // ignore them
"add" {cout << "add"; return TADD;}
"rm" {return TRM;}
"test" {return TSET;}
"user" {cout << "user\n"; return TUSER;}
"obj" {cout << "obj\n";return TOBJ;}
"with" {cout << "with";return TWITH;}
"when" {cout << "when\n"; return TWHEN;}
"on" {cout << "on\n"; return TON;}
"table" {return TTABLE;}
"col" {return TCOL;}
"row" {return TROW;}
"cell" {return TCELL;}
"and" {return TAND;}
"or" {return TOR;}
"," {return TCOMMA;}
"(" {return TLBRACKET;}
")" {return TRBRACKET;}
"=" {cout << "=\n";return TEQUAL;}
">" {return TGREATER;}
"<" {return TLESS;}
">=" {return TGE;}
"<=" {return TLE;}
"." {return TDOT;}
"read" {cout << "read\n"; return TREAD;}
"write" {return TWRITE;}
"allow" {cout << "allow\n"; return TALLOW;}
"deny" {return TDENY;}
[a-zA-Z]+     { cout << "identifier\n"; yylval.sv = strdup(yytext); return TIDENTIFIER; }  
\"[a-zA-Z0-9]+\" {cout << "str\n"; yylval.sv = strdup(yytext); return TSTR;}

%%
int yywrap() {
  return 1;
}

