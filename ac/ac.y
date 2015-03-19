// add user with attr1=xx, attr2=xxx,attr3=xxx
// allow read/write on Table("xxx").col("xxx") when xx and or yy 

%{
  #include <iostream>
  using namespace std;
  int yylex();
  // This is mandatory.
  int yyerror(const char*);
%}

%token <iv> TINT
%token <dv> TDBL
%token <sv> TIDENTIFIER
%token <sv> TSTR
%token TADD TRM TSET
%token TUSER TOBJ
%token TWITH TWHEN TON
%token TTABLE TCOL TROW TCELL
%token TAND TOR
%token TCOMMA TLBRACKET TRBRACKET TEQUAL TGREATER TGE TLESS TLE TDOT
%token TREAD TWRITE
%token TALLOW TDENY

%union {  
  int iv;  
  double dv;
  char *sv;  
}

%%

main : userop TUSER TWITH attrlist 
    | ruleop permissionval TON obj TWHEN logicexp

userop : TADD 
      |  TRM
      | TSET
      ;

ruleop : TALLOW
      | TDENY
      ;


attrlist : attrlist TCOMMA attr
          | attr
          ;

attr: TIDENTIFIER TEQUAL attrval;

attrval : TINT
    | TDBL
    | TSTR
    ;

permissionval : TREAD
              | TDENY
              ;

obj : tableobj
    | tableobj TDOT rowobj
    | tableobj TDOT colobj
    | tableobj TDOT cellobj

tableobj : TABLE TLBRACKET TSTR TRBRACKET;
rowobj : TROW TLBRACKET TINT TRBRACEKT;
colobj : TCOL TLBRACKET TIDENTIFIER TBRACKET;
cellobj : TCELL TLBRACKET TINT TDOT TIDENTIFIER TBRACKET;

logicop : TEQUAL TGREATER TGE TLESS TLE;

userval : TUSER TDOT TIDENTIFIER;
objval : TOBJ TDOT TIDENTIFIER; 
logic : userval logicop objval 
      | objval logicop userval
      | userval logicop attrval
      | attrval logicop userval
      | objval logicop attrval
      | attrval logicop objval
      ;

logicexp : logic 
        | TLBRACKET logic TRBRACKET
        | logicexp AND logicexp
        | logicexp OR logicexp

%%  
  
  
int yyerror(const char *emseg)  
{  
  cout << "Error: " << emseg << endl;  
}  
  
  
int main()  
{ 
  yyparse();  
}  


