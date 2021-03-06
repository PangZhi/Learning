// add user with attr1=xx, attr2=xxx,attr3=xxx
// allow read/write on Table("xxx").col("xxx") when xx and or yy 
// username read/write Table("xxx").col("xxx")
// allow read on col1, col2 from tablename where predicate
// username read/write col1, coln from tablename where predicate
// add tag with logic 
%{
  #include "attrval.h"
  #include "permission.h"
  #include "db/db_worker.h"
  #include "access_control.h"
  #include "obj.h"
  #include "table_obj.h"
  #include "attrval_msg.pb.h"
  
  #include "expression/predicate.h"
  #include "expression/comparison_predicate.h"
  #include "expression/predicate_tree.h"
  #include "expression/disjunction_converter.h"
  #include "expression/auth_predicate.pb.h"

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

  std::unordered_map<std::string, util::AttrVal> valueMap;
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
%token TENTER
%token TFROM TWHERE 
%token TTAG

%type <av> attrval
%type <sv> userval
%type <sv> objval
%type <sv> userop
%type <sv> ruleop
%type <pv> permissionval
%type <objv> obj
%type <predv> logic
%type <compv> logicop
%type <string_listv> columnlist

%left TAND TOR
%union {  
  int iv;  
  double dv;
  util::AttrVal* av;
  char *sv;  
  // std::string *strv;
  ac::Permission pv; 
  ac::Obj *objv;
  ac::Predicate *predv;
  ac::Comparison compv;
  std::vector<std::string>* string_listv;
};

%%

main :  useradmin TENTER main 
    | ruleadmin TENTER main 
    | query TENTER main
    | tagadmin TENTER main
    |
    | TENTER
    ;

tagadmin :: TADD TTAG TIDENTIFIER TWITH logic 
            {
              std::string protostr;
              $5->GetProto().SerializeToString(&protostr);
              ac_ptr->AddTag($3, protostr);  
            }
          | TRM TTAG TIDENTIFIER
          {
            ac_ptr->RemoveTag($3); 
          };
            

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
          {
            // Convert the logic into disjunction form, so that it can be
            // stored in a relational table.
            std::vector<std::vector<ac::ComparisonPredicate> > disjunctions = 
              ac::DisjunctionConverter::convert2Disjunction($6);
                
              bool is_allow = (strcmp($1, "allow") == 0);
              ac_ptr->addRule(is_allow,// Whether it is allow (or deny).
              $2,// Permission type.
              *($4),// Object.
              // $6// Logic.
              disjunctions
              );

          }
          | ruleop permissionval TON columnlist TFROM TIDENTIFIER TWHERE logic
          {
            std::cout << "LOG: New syntax here\n";
            std::vector<std::vector<ac::ComparisonPredicate> > disjunctions = 
                ac::DisjunctionConverter::convert2Disjunction($8);
                
              bool is_allow = (strcmp($1, "allow") == 0);
              ac_ptr->addRule(is_allow,// Whether it is allow (or deny).
              $2,// Permission type.
              //*($4),// Object.
              ac::TableObj($6, *($4)),
              // $6// Logic.
              disjunctions
              );

          }
          ;

columnlist : TIDENTIFIER 
           {
            $$ = new std::vector<std::string>();
            $$->push_back($1);
           }
           | columnlist TCOMMA TIDENTIFIER
           {
            $$ = $1;
            $1->push_back($3);
           }
           ;
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

logicop : TEQUAL {$$ = ac::kEqual;}
        | TGREATER {$$ = ac::kGreater;}
        | TGE {$$ = ac::kGE;}
        | TLESS {$$ = ac::kLess;}
        | TLE {$$ = ac::kLE;}
        ;

userval : TUSER TDOT TIDENTIFIER 
        {
          // int len = strlen($3) + 6;
          // $$ = new char[len];
          // strcpy($$, "user.");
          // strcat($$ + 5, $3);
          // $$[len-1]='\0';
          $$ = $3;
        }

objval :TTABLE TDOT TIDENTIFIER 
       {
          /*
          int len = strlen($3) + 5;
          $$ = new char[len];
          strcpy($$, "user.");
          strcat($$ + 4, $3);
          $$[len-1]='\0';
          */
          $$ = $3;
       }

logic : userval logicop objval 
        {
          $$ = new ac::ComparisonPredicate($2, std::string($1),
          std::string($3));
        } 
      | objval logicop userval 
        {
          $$ = new ac::ComparisonPredicate($2, std::string($3),
          std::string($1));
        }
      | userval logicop attrval
        {
          std::cout << "find a logic\n";
        $$ = new ac::ComparisonPredicate($2, std::string($1), *($3),
        ac::ComparisonPredicate::kUser2Val);}
      | attrval logicop userval
        {$$ = new ac::ComparisonPredicate($2, std::string($3), *($1),
        ac::ComparisonPredicate::kUser2Val);}
      | objval logicop attrval
        {$$ = new ac::ComparisonPredicate($2, std::string($1), *($3),
        ac::ComparisonPredicate::kCol2Val);} 
      | attrval logicop objval
        {$$ = new ac::ComparisonPredicate($2, std::string($3), *($1),
        ac::ComparisonPredicate::kCol2Val);} 
      | TLBRACKET logic TRBRACKET
        {$$ = $2;}
      | logic TAND logic 
        {
          $$ = new ac::ConjunctionPredicate();
          dynamic_cast<ac::ConjunctionPredicate*>($$)->addPredicate($1);
          dynamic_cast<ac::ConjunctionPredicate*>($$)->addPredicate($3);
        }
      | logic TOR logic
        {
          $$ = new ac::DisjunctionPredicate();
          dynamic_cast<ac::DisjunctionPredicate*>($$)->addPredicate($1);
          dynamic_cast<ac::DisjunctionPredicate*>($$)->addPredicate($3);
        }
      ;

query: TIDENTIFIER permissionval obj  
      {
        if (ac_ptr->allow($1, *($3), $2 )) {
          std::cout << "LOG: The operation is allowd\n";
        } else {
          std::cout << "LOG: The operation is not allowed\n";
        }
      }
      | TIDENTIFIER permissionval columnlist TFROM TIDENTIFIER
      {
        if (ac_ptr->allow($1, ac::TableObj($5, *($3)), $2)) {
          std::cout << "LOG: New syntax, the operation is allowed\n";
        } else {
          std::cout << "LOG: The operation is not allowed or undetermined\n";
        }
      }
      ;

//logicexp : logic
//        | TLBRACKET logic TRBRACKET
//        | logicexp TAND logicexp
//        | logicexp TOR logicexp

%%  
  
  
int yyerror(const char *emseg)  
{  
  cout << "Error: " << emseg << endl;  
}  
  
  
int main()  
{ 
  yyparse();  
}  


