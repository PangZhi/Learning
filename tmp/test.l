%{  
  #include <iostream>  
  #include "testyy.h"
  using namespace std;

%}  
%%
[0-9]+        { yylval.iv = strtol(yytext,0,10); return Number; }  
[a-zA-Z]+     { yylval.sv = strdup(yytext); return Word;   }  
[ \t]         ; //忽略空格    
\n            { return *yytext; } //直接返回换行符作为符号给bison  
%%
int yywrap() {
  return 1;
}

