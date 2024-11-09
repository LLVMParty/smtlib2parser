/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BINCONSTANT = 258,
     HEXCONSTANT = 259,
     BVCONSTANT = 260,
     RATCONSTANT = 261,
     NUMERAL = 262,
     STRING = 263,
     SYMBOL = 264,
     KEYWORD = 265,
     TK_EOF = 266,
     TK_AS = 267,
     TK_UNDERSCORE = 268,
     TK_LET = 269,
     TK_BANG = 270,
     TK_FORALL = 271,
     TK_EXISTS = 272,
     TK_SET_LOGIC = 273,
     TK_DECLARE_SORT = 274,
     TK_DEFINE_SORT = 275,
     TK_DECLARE_FUN = 276,
     TK_DEFINE_FUN = 277,
     TK_PUSH = 278,
     TK_POP = 279,
     TK_ASSERT = 280,
     TK_CHECK_SAT = 281,
     TK_GET_ASSERTIONS = 282,
     TK_GET_UNSAT_CORE = 283,
     TK_GET_PROOF = 284,
     TK_SET_OPTION = 285,
     TK_GET_INFO = 286,
     TK_SET_INFO = 287,
     TK_GET_ASSIGNMENT = 288,
     TK_GET_MODEL = 289,
     TK_GET_VALUE = 290,
     TK_EXIT = 291,
     TK_INTERNAL_PARSE_TERMS = 292
   };
#endif
/* Tokens.  */
#define BINCONSTANT 258
#define HEXCONSTANT 259
#define BVCONSTANT 260
#define RATCONSTANT 261
#define NUMERAL 262
#define STRING 263
#define SYMBOL 264
#define KEYWORD 265
#define TK_EOF 266
#define TK_AS 267
#define TK_UNDERSCORE 268
#define TK_LET 269
#define TK_BANG 270
#define TK_FORALL 271
#define TK_EXISTS 272
#define TK_SET_LOGIC 273
#define TK_DECLARE_SORT 274
#define TK_DEFINE_SORT 275
#define TK_DECLARE_FUN 276
#define TK_DEFINE_FUN 277
#define TK_PUSH 278
#define TK_POP 279
#define TK_ASSERT 280
#define TK_CHECK_SAT 281
#define TK_GET_ASSERTIONS 282
#define TK_GET_UNSAT_CORE 283
#define TK_GET_PROOF 284
#define TK_SET_OPTION 285
#define TK_GET_INFO 286
#define TK_SET_INFO 287
#define TK_GET_ASSIGNMENT 288
#define TK_GET_MODEL 289
#define TK_GET_VALUE 290
#define TK_EXIT 291
#define TK_INTERNAL_PARSE_TERMS 292




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 73 "smtlib2bisonparser.y"
{
    char *string;
    smtlib2_vector *termlist;
    smtlib2_sort sort;
    smtlib2_vector *sortlist;
    smtlib2_vector *numlist;
    smtlib2_term term;
    void *identifier;
    smtlib2_charbuf *buf;
    char **attribute;
    smtlib2_vector *attributelist;
    smtlib2_vector *stringlist;
    smtlib2_vector *intlist;
}
/* Line 1529 of yacc.c.  */
#line 138 "smtlib2bisonparser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


