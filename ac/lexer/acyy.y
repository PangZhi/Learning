// add user with attr1=xx, attr2=xxx,attr3=xxx
// allow read/write on Table("xxx").col("xxx") when xx and or yy 

%{
  #include "attrval.h"
  #include "permission.h"
  #include "db/db_worker.h"
  #include "access_control.h"
  #include "obj.h"

  #include <cstring>
  #include <iostream>
  #include <string>
  #include <unordered_map>
  #include <vector>

  using namespace std;
  //using namespace ac;
  int yylex();
  // This is mandatory.
  int yyerror(const char*);

  // Used for adding user.
  std::string username;
  std::vector<ac::AttrItem> attrList;
  
  ac::AccessControl *ac_ptr = new ac::AccessControl();
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
%type <sv> userop
%type <sv> ruleop
%type <pv> permissionval
%type <objv> obj

%left TAND TOR
%union {  
  int iv;  
  double dv;
  util::AttrVal* av;
  char *sv;  
  ac::Permission pv; 
  ac::Obj *objv;
};

%%

main :  useradmin main
    | ruleadmin main
    |
    ;

useradmin : userop TUSER TWITH attrlist 
{
  cout << "success"<< "\n"; 
  cout << attrList.size();
  if (strcmp($1, "add") == 0) { 
      ac_ptr->addUser(username, attrList);
  } else if (strcmp($1, "rm")) {
  } else if (strcmp($1, "set")) {
  }
  attrList.clear();  
}

ruleadmin : ruleop permissionval TON obj TWHEN logic 
          {}
userop : TADD {$$="add";} 
      |  TRM {$$="rm";}
      | TSET {$$="set";}
      ;

ruleop : TALLOW {$$ = "allow";}
      | TDENY {$$ = "deny";}
      ;


// modify the attrlist value here.
attrlist : attrlist TCOMMA attr
          | attr
          ;

attr: TIDENTIFIER TEQUAL attrval 
{
  if (strcmp($1, "username") == 0) {
    std::cout << "LOG : Find username" << std::endl;
    std::cout << *($3->value_.asString) << std::endl;
    username = std::string($3->value_.asString->c_str());
  } 
  else {
    ac::AttrItem attr(std::string($1), *($3));
    attrList.push_back(attr);
  }
}
;

attrval : TINT {$$ = new util::AttrVal($1);}
    | TDBL {$$ = new util::AttrVal($1);}
    | TSTR {$$ = new util::AttrVal($1);}
    | TIDENTIFIER {$$ = new util::AttrVal($1);}
    ;

permissionval : TREAD {$$ = ac::PRead;}
              | TWRITE {$$ = ac::PWrite;}
              ;

//obj : tableobj
//    | tableobj TDOT rowobj
//    | tableobj TDOT colobj
//    | tableobj TDOT cellobj

//tableobj : TTABLE TLBRACKET TSTR TRBRACKET;
//rowobj : TROW TLBRACKET TINT TRBRACKET;
//colobj : TCOL TLBRACKET TIDENTIFIER TRBRACKET;
//cellobj : TCELL TLBRACKET TINT TDOT TIDENTIFIER TRBRACKET;

obj : TTABLE TLBRACKET TSTR TRBRACKET 
      {$$ = new ac::Obj(std::string($3));}
    | TTABLE TLBRACKET TSTR TRBRACKET TDOT TROW TLBRACKET TINT TRBRACKET 
      {$$ = new ac::Obj(std::string($3),$8);}
    | TTABLE TLBRACKET TSTR TRBRACKET TDOT TCOL TLBRACKET TIDENTIFIER TRBRACKET 
      {$$ = new ac::Obj(std::string($3), std::string($8));}
    | TTABLE TLBRACKET TSTR TRBRACKET TDOT TCELL TLBRACKET TINT TDOT TIDENTIFIER TRBRACKET
      {$$ = new ac::Obj(std::string($3), $8, std::string($10));}
    ;

logicop : TEQUAL TGREATER TGE TLESS TLE;

userval : TUSER TDOT TIDENTIFIER;
objval : TOBJ TDOT TIDENTIFIER; 
logic : userval logicop objval 
      | objval logicop userval
      | userval logicop attrval
      | attrval logicop userval
      | objval logicop attrval
      | attrval logicop objval
      | TLBRACKET logic TRBRACKET
      | logic TAND logic
      | logic TOR logic
      ;

//logicexp : logic 
//        | TLBRACKET logic TRBRACKET
//        | logicexp TAND logicexp
//        | logicexp TOR logicexp
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


