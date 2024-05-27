/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MINI_Y_H_INCLUDED
# define YY_YY_MINI_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INT = 258,                     /* INT  */
    CHAR = 259,                    /* CHAR  */
    SHORT = 260,                   /* SHORT  */
    LONG = 261,                    /* LONG  */
    FLOAT = 262,                   /* FLOAT  */
    EQ = 263,                      /* EQ  */
    NE = 264,                      /* NE  */
    LT = 265,                      /* LT  */
    LE = 266,                      /* LE  */
    GT = 267,                      /* GT  */
    GE = 268,                      /* GE  */
    UMINUS = 269,                  /* UMINUS  */
    IF = 270,                      /* IF  */
    THEN = 271,                    /* THEN  */
    ELSE = 272,                    /* ELSE  */
    FI = 273,                      /* FI  */
    WHILE = 274,                   /* WHILE  */
    DO = 275,                      /* DO  */
    DONE = 276,                    /* DONE  */
    CONTINUE = 277,                /* CONTINUE  */
    FUNC = 278,                    /* FUNC  */
    PRINT = 279,                   /* PRINT  */
    RETURN = 280,                  /* RETURN  */
    FOR = 281,                     /* FOR  */
    STRUCT = 282,                  /* STRUCT  */
    CHARACTER = 283,               /* CHARACTER  */
    INTEGER = 284,                 /* INTEGER  */
    IDENTIFIER = 285,              /* IDENTIFIER  */
    TEXT = 286                     /* TEXT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define INT 258
#define CHAR 259
#define SHORT 260
#define LONG 261
#define FLOAT 262
#define EQ 263
#define NE 264
#define LT 265
#define LE 266
#define GT 267
#define GE 268
#define UMINUS 269
#define IF 270
#define THEN 271
#define ELSE 272
#define FI 273
#define WHILE 274
#define DO 275
#define DONE 276
#define CONTINUE 277
#define FUNC 278
#define PRINT 279
#define RETURN 280
#define FOR 281
#define STRUCT 282
#define CHARACTER 283
#define INTEGER 284
#define IDENTIFIER 285
#define TEXT 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "mini.y"

	char character;
	char *string;
	SYM *sym;
	TAC *tac;
	EXP	*exp;

#line 137 "mini.y.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MINI_Y_H_INCLUDED  */
