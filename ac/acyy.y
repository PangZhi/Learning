// add user with attr1=xx, attr2=xxx,attr3=xxx
// allow read/write on Table("xxx").col("xxx") when xx and or yy 

%{
  #include "attrval.h"
  #include "db_worker.h"
  
  #include <iostream>
  #include <string>
  #include <unordered_map>
  #include <vector>

  using namespace std;
  int yylex();
  // This is mandatory.
  int yyerror(const char*);
  
  std::string username;
  std::vector<AttrItem> attrList;
  

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

%type <av> attrval
%union {  
  int iv;  
  double dv;
  util::AttrVal av;
  char *sv;  
}

%%

main :  useradmin main
    | ruleadmin main
    |
    ;

useradmin : userop TUSER TWITH attrlist {cout << "success"<< "\n"; cout << attrList.size();}
ruleadmin : ruleop permissionval TON obj TWHEN logicexp 

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

attr: TIDENTIFIER TEQUAL attrval 
{
  if (strcmp($1, "username") == 0) {
    username = std::string($3);
  } else {
    AttrItem attr(std::string($1), $3);
    attrList.add(attr);
  }

}
;

attrval : TINT {$$ = $1.iv;}
    | TDBL {$$ = $1.dv;}
    | TSTR {$$ = $1.sv;}
    ;

permissionval : TREAD
              | TDENY
              ;

obj : tableobj
    | tableobj TDOT rowobj
    | tableobj TDOT colobj
    | tableobj TDOT cellobj

tableobj : TTABLE TLBRACKET TSTR TRBRACKET;
rowobj : TROW TLBRACKET TINT TRBRACKET;
colobj : TCOL TLBRACKET TIDENTIFIER TRBRACKET;
cellobj : TCELL TLBRACKET TINT TDOT TIDENTIFIER TRBRACKET;

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
//        | logicexp TAND logicexp
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


