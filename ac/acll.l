%{  
  #include <iostream>  
  #include "attrval.h"
  #include "db_worker.h"
  #include "acyy.hpp"
  #include "access_control.h"
  using namespace std;

%}  
%%
[-+]?[0-9]+        { yylval.iv = strtol(yytext,0,10); cout << "int" ;return TINT; }  
[-+]?(\d+(\.\d*)?|\.\d+)[dD]? {yylval.dv=strtol(yytext, 0, 10); return TDBL;}
[ \t\r\n]         ; // ignore them
"add" {cout << "add"; return TADD;}
"rm" {return TRM;}
"test" {return TSET;}
"user" {cout << "user"; return TUSER;}
"obj" {return TOBJ;}
"with" {cout << "with";return TWITH;}
"when" {return TWHEN;}
"on" {return TON;}
"table" {return TTABLE;}
"col" {return TCOL;}
"row" {return TROW;}
"cell" {return TCELL;}
"and" {return TAND;}
"or" {return TOR;}
"," {return TCOMMA;}
"(" {return TLBRACKET;}
")" {return TRBRACKET;}
"=" {return TEQUAL;}
">" {return TGREATER;}
"<" {return TLESS;}
">=" {return TGE;}
"<=" {return TLE;}
"." {return TDOT;}
"read" {return TREAD;}
"write" {return TWRITE;}
"allow" {return TALLOW;}
"deny" {return TDENY;}
[a-zA-Z]+     { cout << "identifier";yylval.sv = strdup(yytext); return TIDENTIFIER; }  
\"[a-zA-z0-9]+\" {yylval.sv = strdup(yytext); return TIDENTIFIER;}

%%
int yywrap() {
  return 1;
}

