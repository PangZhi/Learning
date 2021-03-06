%{
  #include <iostream>
  using namespace std;
  int yylex();
  // This is mandatory.
  int yyerror(const char*);
%}

%token <iv> Number  
%token <sv> Word  
%union {  
  int iv;  
  char *sv;  
}  
%%

main : main Number Word Word Number '\n'   { cout << $2 + $5 << $3 << $4 << endl ; free($3) ; free($4) ; }  
       |    //空  
       ;  
    
%%  
  
  
int yyerror(const char *emseg)  
{  
  cout << "Error: " << emseg << endl;  
}  
  
  
int main()  
{  
  yyparse();  
}  


