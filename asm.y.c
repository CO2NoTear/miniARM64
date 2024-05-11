/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "asm.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"

#define LABNUM 100

extern int yylineno;

int pass, ip;

struct label
{
	int addr;
	char *name;
} label[LABNUM];

int yylex();
void yyerror(char* msg);
void byte1(int  n);
void byte2(int  n);
void byte4(int n);

int number(char * name)
{
	int index=0;

	while(label[index].name != NULL)
	{
		if(!strcmp(label[index].name, name)) 
			return index;
		index++;
	}

	if(index>=LABNUM)
	{
		fprintf(stderr, "error: too many label");
		exit(0);
	}

	label[index].name=name;
	return index;
}

void byte1(int  n)
{
	if(pass==2)
		putchar(n);
	ip++;
}

void byte2(int  n)
{
	if(pass==2)
	{
		putchar(n>>8);
		putchar(n);
	}
	ip+=2;
}	

void byte4(int n)
{
	if(pass==2)
	{
		putchar(n>>24);
		putchar(n>>16);
		putchar(n>>8);
		putchar(n);
	}
	ip+=4;
}


#line 147 "asm.y.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "asm.y.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ADD = 3,                        /* ADD  */
  YYSYMBOL_SUB = 4,                        /* SUB  */
  YYSYMBOL_MUL = 5,                        /* MUL  */
  YYSYMBOL_DIV = 6,                        /* DIV  */
  YYSYMBOL_TST = 7,                        /* TST  */
  YYSYMBOL_STO = 8,                        /* STO  */
  YYSYMBOL_LOD = 9,                        /* LOD  */
  YYSYMBOL_JMP = 10,                       /* JMP  */
  YYSYMBOL_JEZ = 11,                       /* JEZ  */
  YYSYMBOL_JLZ = 12,                       /* JLZ  */
  YYSYMBOL_JGZ = 13,                       /* JGZ  */
  YYSYMBOL_DBN = 14,                       /* DBN  */
  YYSYMBOL_DBS = 15,                       /* DBS  */
  YYSYMBOL_OUT = 16,                       /* OUT  */
  YYSYMBOL_NOP = 17,                       /* NOP  */
  YYSYMBOL_END = 18,                       /* END  */
  YYSYMBOL_INTEGER = 19,                   /* INTEGER  */
  YYSYMBOL_REG = 20,                       /* REG  */
  YYSYMBOL_LABEL = 21,                     /* LABEL  */
  YYSYMBOL_22_ = 22,                       /* ','  */
  YYSYMBOL_23_ = 23,                       /* ':'  */
  YYSYMBOL_24_ = 24,                       /* '+'  */
  YYSYMBOL_25_ = 25,                       /* '-'  */
  YYSYMBOL_26_ = 26,                       /* '('  */
  YYSYMBOL_27_ = 27,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 28,                  /* $accept  */
  YYSYMBOL_start = 29,                     /* start  */
  YYSYMBOL_program = 30,                   /* program  */
  YYSYMBOL_statement = 31,                 /* statement  */
  YYSYMBOL_nop_stmt = 32,                  /* nop_stmt  */
  YYSYMBOL_add_stmt = 33,                  /* add_stmt  */
  YYSYMBOL_sub_stmt = 34,                  /* sub_stmt  */
  YYSYMBOL_mul_stmt = 35,                  /* mul_stmt  */
  YYSYMBOL_div_stmt = 36,                  /* div_stmt  */
  YYSYMBOL_tst_stmt = 37,                  /* tst_stmt  */
  YYSYMBOL_lab_stmt = 38,                  /* lab_stmt  */
  YYSYMBOL_jmp_stmt = 39,                  /* jmp_stmt  */
  YYSYMBOL_jez_stmt = 40,                  /* jez_stmt  */
  YYSYMBOL_jlz_stmt = 41,                  /* jlz_stmt  */
  YYSYMBOL_jgz_stmt = 42,                  /* jgz_stmt  */
  YYSYMBOL_lod_stmt = 43,                  /* lod_stmt  */
  YYSYMBOL_sto_stmt = 44,                  /* sto_stmt  */
  YYSYMBOL_out_stmt = 45,                  /* out_stmt  */
  YYSYMBOL_end_stmt = 46,                  /* end_stmt  */
  YYSYMBOL_dbn_stmt = 47,                  /* dbn_stmt  */
  YYSYMBOL_dbs_stmt = 48                   /* dbs_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   103

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  118

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   276


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,    27,     2,    24,    22,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    23,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    89,    89,    92,    93,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   115,   118,   125,   132,   141,   148,   155,   164,
     171,   178,   187,   194,   201,   210,   219,   236,   243,   252,
     259,   268,   275,   284,   291,   300,   307,   314,   321,   328,
     335,   342,   349,   356,   363,   372,   379,   386,   393,   400,
     407,   414,   423,   426,   429,   437,   438
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ADD", "SUB", "MUL",
  "DIV", "TST", "STO", "LOD", "JMP", "JEZ", "JLZ", "JGZ", "DBN", "DBS",
  "OUT", "NOP", "END", "INTEGER", "REG", "LABEL", "','", "':'", "'+'",
  "'-'", "'('", "')'", "$accept", "start", "program", "statement",
  "nop_stmt", "add_stmt", "sub_stmt", "mul_stmt", "div_stmt", "tst_stmt",
  "lab_stmt", "jmp_stmt", "jez_stmt", "jlz_stmt", "jgz_stmt", "lod_stmt",
  "sto_stmt", "out_stmt", "end_stmt", "dbn_stmt", "dbs_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    44,    58,    43,    45,    40,    41
};
#endif

#define YYPACT_NINF (-5)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -5,    55,    -3,    -5,     4,     9,    40,    41,    42,    37,
      44,    -4,    29,    31,    33,    46,    47,    -5,    -5,    -5,
      45,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,
      -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    48,    49,
      50,    51,    52,    -5,    56,    53,    -5,    -5,    -5,    -5,
      -5,    -5,    -5,    -5,    57,    -5,    -5,    58,    12,    15,
      18,    21,    -2,     0,    59,    -5,    -5,    -5,    -5,    -5,
      -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    61,    62,
      60,    -5,    32,    -5,    24,    -5,    63,    64,    27,    65,
      66,    67,     3,    68,    70,    71,    -5,    34,    -5,    -5,
      -5,    -5,    69,    77,    -5,    -5,    78,    79,    81,    82,
      75,    76,    -5,    -5,    -5,    -5,    -5,    -5
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,     2,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    22,    63,
       0,     3,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,    35,     0,     0,    38,    37,    40,    39,
      42,    41,    44,    43,     0,    66,    36,     0,     0,     0,
       0,     0,     0,     0,     0,    65,    23,    25,    24,    26,
      28,    27,    29,    31,    30,    32,    34,    33,     0,     0,
       0,    45,    47,    46,     0,    64,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,    57,    56,    48,
      49,    50,     0,     0,    52,    51,     0,     0,     0,     0,
       0,     0,    60,    61,    58,    59,    53,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
      -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,
      -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,    -5,
      -5
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    46,    47,    20,    81,
      82,    83,    78,    79,    39,    80,    84,   102,   103,    40,
     104,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    91,    92,    93,    96,    97,    98,    48,
      49,    50,    51,    52,    53,     3,    89,    90,   108,   109,
      41,    42,    43,    44,    45,    54,    55,     0,    56,     0,
      57,    58,    59,    60,    61,    63,    62,    65,    85,    64,
      86,    87,    88,     0,    99,   100,     0,     0,   110,     0,
      94,    95,   106,   107,   101,   105,   111,     0,   112,   113,
     114,   115,   116,   117
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    20,    21,    21,    19,
      20,    21,    24,    25,    20,    27,    26,    24,    25,    20,
      27,    19,    20,    21,    19,    20,    21,    19,    20,    21,
      19,    20,    21,    19,    20,    21,    19,    20,    21,    20,
      21,    20,    21,    20,    21,     0,    24,    25,    24,    25,
      20,    20,    20,    26,    20,    19,    19,    -1,    23,    -1,
      22,    22,    22,    22,    22,    22,    20,    19,    19,    22,
      19,    19,    22,    -1,    19,    19,    -1,    -1,    19,    -1,
      27,    27,    22,    22,    27,    27,    19,    -1,    20,    20,
      19,    19,    27,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    30,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      21,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    20,
      20,    20,    20,    20,    26,    20,    20,    21,    20,    21,
      20,    21,    20,    21,    19,    19,    23,    22,    22,    22,
      22,    22,    20,    22,    22,    19,    19,    20,    21,    19,
      20,    21,    19,    20,    21,    19,    20,    21,    24,    25,
      27,    19,    20,    21,    26,    19,    19,    19,    22,    24,
      25,    19,    20,    21,    27,    27,    19,    20,    21,    19,
      19,    27,    24,    25,    27,    27,    22,    22,    24,    25,
      19,    19,    20,    20,    19,    19,    27,    27
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    28,    29,    30,    30,    31,    31,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,    31,    31,    31,
      31,    31,    32,    33,    33,    33,    34,    34,    34,    35,
      35,    35,    36,    36,    36,    37,    38,    39,    39,    40,
      40,    41,    41,    42,    42,    43,    43,    43,    43,    43,
      43,    43,    43,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    45,    46,    47,    48,    48
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     4,     4,     4,     6,     6,
       6,     6,     6,     8,     8,     6,     6,     6,     8,     8,
       8,     8,     1,     1,     4,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 22: /* nop_stmt: NOP  */
#line 115 "asm.y"
                { byte2(I_NOP); byte1(0); byte1(0); byte4(0);}
#line 1259 "asm.y.c"
    break;

  case 23: /* add_stmt: ADD REG ',' INTEGER  */
#line 119 "asm.y"
{
	byte2(I_ADD_0) ;
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1270 "asm.y.c"
    break;

  case 24: /* add_stmt: ADD REG ',' LABEL  */
#line 126 "asm.y"
{
	byte2(I_ADD_0);
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1281 "asm.y.c"
    break;

  case 25: /* add_stmt: ADD REG ',' REG  */
#line 133 "asm.y"
{
	byte2(I_ADD_1);
	byte1((yyvsp[-2].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1292 "asm.y.c"
    break;

  case 26: /* sub_stmt: SUB REG ',' INTEGER  */
#line 142 "asm.y"
{
	byte2(I_SUB_0) ;
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1303 "asm.y.c"
    break;

  case 27: /* sub_stmt: SUB REG ',' LABEL  */
#line 149 "asm.y"
{
	byte2(I_SUB_0);
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1314 "asm.y.c"
    break;

  case 28: /* sub_stmt: SUB REG ',' REG  */
#line 156 "asm.y"
{
	byte2(I_SUB_1);
	byte1((yyvsp[-2].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1325 "asm.y.c"
    break;

  case 29: /* mul_stmt: MUL REG ',' INTEGER  */
#line 165 "asm.y"
{
	byte2(I_MUL_0) ;
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1336 "asm.y.c"
    break;

  case 30: /* mul_stmt: MUL REG ',' LABEL  */
#line 172 "asm.y"
{
	byte2(I_MUL_0);
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1347 "asm.y.c"
    break;

  case 31: /* mul_stmt: MUL REG ',' REG  */
#line 179 "asm.y"
{
	byte2(I_MUL_1);
	byte1((yyvsp[-2].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1358 "asm.y.c"
    break;

  case 32: /* div_stmt: DIV REG ',' INTEGER  */
#line 188 "asm.y"
{
	byte2(I_DIV_0) ;
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1369 "asm.y.c"
    break;

  case 33: /* div_stmt: DIV REG ',' LABEL  */
#line 195 "asm.y"
{
	byte2(I_DIV_0);
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1380 "asm.y.c"
    break;

  case 34: /* div_stmt: DIV REG ',' REG  */
#line 202 "asm.y"
{
	byte2(I_DIV_1);
	byte1((yyvsp[-2].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1391 "asm.y.c"
    break;

  case 35: /* tst_stmt: TST REG  */
#line 211 "asm.y"
{
	byte2(I_TST_0) ;
	byte1((yyvsp[0].number));
	byte1(0);
	byte4(0);
}
#line 1402 "asm.y.c"
    break;

  case 36: /* lab_stmt: LABEL ':'  */
#line 220 "asm.y"
{
	if(pass==1)
	{
		if(label[number((yyvsp[-1].string))].addr==0)
		{
			label[number((yyvsp[-1].string))].addr=ip;
		}
		else
		{
			fprintf(stderr, "error: label %s already exist\n", label[number((yyvsp[-1].string))].name);
			exit(0);
		}
	}
}
#line 1421 "asm.y.c"
    break;

  case 37: /* jmp_stmt: JMP LABEL  */
#line 237 "asm.y"
{
	byte2(I_JMP_0);
	byte1(0);
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1432 "asm.y.c"
    break;

  case 38: /* jmp_stmt: JMP REG  */
#line 244 "asm.y"
{
	byte2(I_JMP_1);
	byte1((yyvsp[0].number));		
	byte1(0);	
	byte4(0);				
}
#line 1443 "asm.y.c"
    break;

  case 39: /* jez_stmt: JEZ LABEL  */
#line 253 "asm.y"
{
	byte2(I_JEZ_0);
	byte1(0);
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1454 "asm.y.c"
    break;

  case 40: /* jez_stmt: JEZ REG  */
#line 260 "asm.y"
{
	byte2(I_JEZ_1) ;
	byte1((yyvsp[0].number));		
	byte1(0);	
	byte4(0);	
}
#line 1465 "asm.y.c"
    break;

  case 41: /* jlz_stmt: JLZ LABEL  */
#line 269 "asm.y"
{
	byte2(I_JLZ_0);
	byte1(0);
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1476 "asm.y.c"
    break;

  case 42: /* jlz_stmt: JLZ REG  */
#line 276 "asm.y"
{
	byte2(I_JLZ_1) ;
	byte1( (yyvsp[0].number) ) ;		
	byte1(0);	
	byte4(0);	
}
#line 1487 "asm.y.c"
    break;

  case 43: /* jgz_stmt: JGZ LABEL  */
#line 285 "asm.y"
{
	byte2(I_JGZ_0);
	byte1(0);
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1498 "asm.y.c"
    break;

  case 44: /* jgz_stmt: JGZ REG  */
#line 292 "asm.y"
{
	byte2(I_JGZ_1) ;
	byte1( (yyvsp[0].number) ) ;		
	byte1(0);	
	byte4(0);	
}
#line 1509 "asm.y.c"
    break;

  case 45: /* lod_stmt: LOD REG ',' INTEGER  */
#line 301 "asm.y"
{
	byte2(I_LOD_0) ;
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1520 "asm.y.c"
    break;

  case 46: /* lod_stmt: LOD REG ',' LABEL  */
#line 308 "asm.y"
{
	byte2(I_LOD_0);
	byte1((yyvsp[-2].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1531 "asm.y.c"
    break;

  case 47: /* lod_stmt: LOD REG ',' REG  */
#line 315 "asm.y"
{
	byte2(I_LOD_1);
	byte1((yyvsp[-2].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1542 "asm.y.c"
    break;

  case 48: /* lod_stmt: LOD REG ',' REG '+' INTEGER  */
#line 322 "asm.y"
{
	byte2(I_LOD_2);
	byte1((yyvsp[-4].number));
	byte1((yyvsp[-2].number));
	byte4((yyvsp[0].number));
}
#line 1553 "asm.y.c"
    break;

  case 49: /* lod_stmt: LOD REG ',' REG '-' INTEGER  */
#line 329 "asm.y"
{
	byte2(I_LOD_2);
	byte1((yyvsp[-4].number));
	byte1((yyvsp[-2].number));
	byte4(-((yyvsp[0].number)));
}
#line 1564 "asm.y.c"
    break;

  case 50: /* lod_stmt: LOD REG ',' '(' INTEGER ')'  */
#line 336 "asm.y"
{
	byte2(I_LOD_3);
	byte1((yyvsp[-4].number));
	byte1(0);
	byte4((yyvsp[-1].number));
}
#line 1575 "asm.y.c"
    break;

  case 51: /* lod_stmt: LOD REG ',' '(' LABEL ')'  */
#line 343 "asm.y"
{
	byte2(I_LOD_3);
	byte1((yyvsp[-4].number));
	byte1(0);
	byte4(label[number((yyvsp[-1].string))].addr);
}
#line 1586 "asm.y.c"
    break;

  case 52: /* lod_stmt: LOD REG ',' '(' REG ')'  */
#line 350 "asm.y"
{
	byte2(I_LOD_4);
	byte1((yyvsp[-4].number));
	byte1((yyvsp[-1].number));
	byte4(0);
}
#line 1597 "asm.y.c"
    break;

  case 53: /* lod_stmt: LOD REG ',' '(' REG '+' INTEGER ')'  */
#line 357 "asm.y"
{
	byte2(I_LOD_5);
	byte1((yyvsp[-6].number));
	byte1((yyvsp[-3].number));
	byte4((yyvsp[-1].number));
}
#line 1608 "asm.y.c"
    break;

  case 54: /* lod_stmt: LOD REG ',' '(' REG '-' INTEGER ')'  */
#line 364 "asm.y"
{
	byte2(I_LOD_5);
	byte1((yyvsp[-6].number));
	byte1((yyvsp[-3].number));
	byte4(-((yyvsp[-1].number)));
}
#line 1619 "asm.y.c"
    break;

  case 55: /* sto_stmt: STO '(' REG ')' ',' INTEGER  */
#line 373 "asm.y"
{
	byte2(I_STO_0);
	byte1((yyvsp[-3].number));
	byte1(0);
	byte4((yyvsp[0].number));
}
#line 1630 "asm.y.c"
    break;

  case 56: /* sto_stmt: STO '(' REG ')' ',' LABEL  */
#line 380 "asm.y"
{
	byte2(I_STO_0);
	byte1((yyvsp[-3].number));
	byte1(0);
	byte4(label[number((yyvsp[0].string))].addr);
}
#line 1641 "asm.y.c"
    break;

  case 57: /* sto_stmt: STO '(' REG ')' ',' REG  */
#line 387 "asm.y"
{
	byte2( I_STO_1 ) ;
	byte1((yyvsp[-3].number));
	byte1((yyvsp[0].number));
	byte4(0);
}
#line 1652 "asm.y.c"
    break;

  case 58: /* sto_stmt: STO '(' REG ')' ',' REG '+' INTEGER  */
#line 394 "asm.y"
{
	byte2( I_STO_2 ) ;
	byte1((yyvsp[-5].number));
	byte1((yyvsp[-2].number));
	byte4((yyvsp[0].number));
}
#line 1663 "asm.y.c"
    break;

  case 59: /* sto_stmt: STO '(' REG ')' ',' REG '-' INTEGER  */
#line 401 "asm.y"
{
	byte2( I_STO_2 ) ;
	byte1((yyvsp[-5].number));
	byte1((yyvsp[-2].number));
	byte4(-((yyvsp[0].number)));
}
#line 1674 "asm.y.c"
    break;

  case 60: /* sto_stmt: STO '(' REG '+' INTEGER ')' ',' REG  */
#line 408 "asm.y"
{
	byte2( I_STO_3 ) ;
	byte1((yyvsp[-5].number));
	byte1((yyvsp[0].number));
	byte4((yyvsp[-3].number));
}
#line 1685 "asm.y.c"
    break;

  case 61: /* sto_stmt: STO '(' REG '-' INTEGER ')' ',' REG  */
#line 415 "asm.y"
{
	byte2( I_STO_3 ) ;
	byte1((yyvsp[-5].number));
	byte1((yyvsp[0].number));
	byte4(-((yyvsp[-3].number)));
}
#line 1696 "asm.y.c"
    break;

  case 62: /* out_stmt: OUT  */
#line 423 "asm.y"
               { byte2(I_OUT); byte1(0); byte1(0); byte4(0);}
#line 1702 "asm.y.c"
    break;

  case 63: /* end_stmt: END  */
#line 426 "asm.y"
               { byte2(I_END); byte1(0); byte1(0); byte4(0);}
#line 1708 "asm.y.c"
    break;

  case 64: /* dbn_stmt: DBN INTEGER ',' INTEGER  */
#line 430 "asm.y"
{
	int n = (yyvsp[0].number);
	while(n-- > 0)
		byte1((yyvsp[-2].number));
}
#line 1718 "asm.y.c"
    break;

  case 65: /* dbs_stmt: dbs_stmt ',' INTEGER  */
#line 437 "asm.y"
                                { byte1((yyvsp[0].number)); }
#line 1724 "asm.y.c"
    break;

  case 66: /* dbs_stmt: DBS INTEGER  */
#line 438 "asm.y"
              { byte1((yyvsp[0].number)); }
#line 1730 "asm.y.c"
    break;


#line 1734 "asm.y.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 441 "asm.y"


void yyerror(char* msg) 
{
	fprintf(stderr, "%s: line %d\n", msg, yylineno);
	exit(0);
}

int main(int argc,   char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(0);
	}
	
	char *input, *output;

	input = argv[1];
	if(freopen(input, "r", stdin)==NULL)
	{
		fprintf(stderr, "error: open %s failed\n", input);
		return 0;
	}

	output=(char *)malloc(strlen(input + 10));
	strcpy(output,input);
	strcat(output,".o");

	if(freopen(output, "w", stdout)==NULL)
	{
		fprintf(stderr, "error: open %s failed\n", output);
		return 0;
	}

	int i=sizeof(label);
	bzero(label, i);

	/* First pass, set up labels */
	pass=1;
	ip=0;
	yyparse();

	/* Second pass, generate code */
	pass=2;
	ip=0;
	rewind(stdin) ;
	yyparse();

	return 0;
}

