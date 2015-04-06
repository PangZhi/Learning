/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_ACYY_HPP_INCLUDED
# define YY_YY_ACYY_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TINT = 258,
    TDBL = 259,
    TIDENTIFIER = 260,
    TSTR = 261,
    TADD = 262,
    TRM = 263,
    TSET = 264,
    TUSER = 265,
    TOBJ = 266,
    TWITH = 267,
    TWHEN = 268,
    TON = 269,
    TTABLE = 270,
    TCOL = 271,
    TROW = 272,
    TCELL = 273,
    TAND = 274,
    TOR = 275,
    TCOMMA = 276,
    TLBRACKET = 277,
    TRBRACKET = 278,
    TEQUAL = 279,
    TGREATER = 280,
    TGE = 281,
    TLESS = 282,
    TLE = 283,
    TDOT = 284,
    TREAD = 285,
    TWRITE = 286,
    TALLOW = 287,
    TDENY = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 49 "acyy.y" /* yacc.c:1909  */
  
  int iv;  
  double dv;
  util::AttrVal* av;
  char *sv;  
  ac::Permission pv; 

#line 96 "acyy.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_ACYY_HPP_INCLUDED  */
