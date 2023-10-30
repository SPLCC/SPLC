/* A Bison parser, made by GNU Bison 3.8.2.  */

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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





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

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_CHAR = 5,                       /* CHAR  */
  YYSYMBOL_STR = 6,                        /* STR  */
  YYSYMBOL_STRUCT = 7,                     /* STRUCT  */
  YYSYMBOL_ID = 8,                         /* ID  */
  YYSYMBOL_TYPE = 9,                       /* TYPE  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_THEN = 11,                      /* THEN  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_FOR = 14,                       /* FOR  */
  YYSYMBOL_RETURN = 15,                    /* RETURN  */
  YYSYMBOL_SEMI = 16,                      /* SEMI  */
  YYSYMBOL_COMMA = 17,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 18,                    /* ASSIGN  */
  YYSYMBOL_AND = 19,                       /* AND  */
  YYSYMBOL_OR = 20,                        /* OR  */
  YYSYMBOL_BITWISE_AND = 21,               /* BITWISE_AND  */
  YYSYMBOL_BITWISE_OR = 22,                /* BITWISE_OR  */
  YYSYMBOL_LT = 23,                        /* LT  */
  YYSYMBOL_LE = 24,                        /* LE  */
  YYSYMBOL_GT = 25,                        /* GT  */
  YYSYMBOL_GE = 26,                        /* GE  */
  YYSYMBOL_NE = 27,                        /* NE  */
  YYSYMBOL_EQ = 28,                        /* EQ  */
  YYSYMBOL_PLUS = 29,                      /* PLUS  */
  YYSYMBOL_MINUS = 30,                     /* MINUS  */
  YYSYMBOL_MUL = 31,                       /* MUL  */
  YYSYMBOL_DIV = 32,                       /* DIV  */
  YYSYMBOL_LC = 33,                        /* LC  */
  YYSYMBOL_RC = 34,                        /* RC  */
  YYSYMBOL_LP = 35,                        /* LP  */
  YYSYMBOL_RP = 36,                        /* RP  */
  YYSYMBOL_NOT = 37,                       /* NOT  */
  YYSYMBOL_DOT = 38,                       /* DOT  */
  YYSYMBOL_LSB = 39,                       /* LSB  */
  YYSYMBOL_RSB = 40,                       /* RSB  */
  YYSYMBOL_UPLUS = 41,                     /* UPLUS  */
  YYSYMBOL_UMINUS = 42,                    /* UMINUS  */
  YYSYMBOL_PRE_PLUS = 43,                  /* PRE_PLUS  */
  YYSYMBOL_PRE_MINUS = 44,                 /* PRE_MINUS  */
  YYSYMBOL_POST_PLUS = 45,                 /* POST_PLUS  */
  YYSYMBOL_POST_MINUS = 46,                /* POST_MINUS  */
  YYSYMBOL_YYACCEPT = 47,                  /* $accept  */
  YYSYMBOL_Program = 48,                   /* Program  */
  YYSYMBOL_ExtDefList = 49,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 50,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 51,                /* ExtDecList  */
  YYSYMBOL_Specifier = 52,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 53,           /* StructSpecifier  */
  YYSYMBOL_VarDec = 54,                    /* VarDec  */
  YYSYMBOL_FuncDec = 55,                   /* FuncDec  */
  YYSYMBOL_VarList = 56,                   /* VarList  */
  YYSYMBOL_ParamDec = 57,                  /* ParamDec  */
  YYSYMBOL_CompStmt = 58,                  /* CompStmt  */
  YYSYMBOL_StmtList = 59,                  /* StmtList  */
  YYSYMBOL_Stmt = 60,                      /* Stmt  */
  YYSYMBOL_ForLoopBody = 61,               /* ForLoopBody  */
  YYSYMBOL_DefList = 62,                   /* DefList  */
  YYSYMBOL_Def = 63,                       /* Def  */
  YYSYMBOL_DecList = 64,                   /* DecList  */
  YYSYMBOL_Dec = 65,                       /* Dec  */
  YYSYMBOL_Exp = 66,                       /* Exp  */
  YYSYMBOL_StrExp = 67,                    /* StrExp  */
  YYSYMBOL_Args = 68                       /* Args  */
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
typedef yytype_uint8 yy_state_t;

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

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  80
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1433

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  234

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   301


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    43,    43,    51,    52,    53,    57,    58,    59,    60,
      62,    63,    64,    65,    69,    70,    73,    74,    78,    79,
      83,    84,    85,    86,    91,    92,    94,    95,    96,   100,
     101,   105,   109,   110,   111,   112,   113,   114,   115,   119,
     120,   124,   125,   126,   128,   130,   131,   133,   134,   135,
     136,   137,   138,   140,   141,   143,   145,   146,   147,   150,
     152,   153,   154,   156,   157,   158,   160,   164,   165,   169,
     170,   174,   175,   179,   180,   181,   182,   183,   187,   188,
     189,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   227,   228,   229,   230,   232,   233,   234,   235,   236,
     237,   239,   240,   241,   243,   245,   246,   248,   249,   250,
     251,   252,   253,   255,   256,   258,   259,   260,   261,   262,
     266,   267,   271,   272
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "INT", "FLOAT", "CHAR", "STR",
  "STRUCT", "ID", "TYPE", "IF", "THEN", "ELSE", "WHILE", "FOR", "RETURN",
  "SEMI", "COMMA", "ASSIGN", "AND", "OR", "BITWISE_AND", "BITWISE_OR",
  "LT", "LE", "GT", "GE", "NE", "EQ", "PLUS", "MINUS", "MUL", "DIV", "LC",
  "RC", "LP", "RP", "NOT", "DOT", "LSB", "RSB", "UPLUS", "UMINUS",
  "PRE_PLUS", "PRE_MINUS", "POST_PLUS", "POST_MINUS", "$accept", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "VarDec", "FuncDec", "VarList", "ParamDec", "CompStmt", "StmtList",
  "Stmt", "ForLoopBody", "DefList", "Def", "DecList", "Dec", "Exp",
  "StrExp", "Args", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-71)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-28)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     838,   -71,   -71,   -71,     5,    24,   140,   -71,   -29,   908,
      15,    34,  1048,   -71,   558,  1188,  1153,  1118,  1083,   162,
     162,   162,   162,   162,   162,  1048,  1048,   593,   628,   803,
     663,  1048,    40,   -71,   838,    45,    50,   -71,    14,    31,
     -71,   838,   109,   -71,   -71,    39,   484,  1048,    42,   -71,
    1048,    67,   209,   -71,  1322,  1394,  1377,  1360,  1343,   174,
     174,   174,   174,   174,   174,   125,   125,   -71,   125,   -71,
     125,   -71,   164,   111,   908,   873,   108,    56,   241,   125,
     -71,   -71,   -71,   -71,    51,   -71,    92,   -71,    38,   164,
     121,   -71,   -71,   -71,   -71,  1048,  1048,  1048,  1048,  1048,
    1048,  1048,  1048,  1048,  1048,  1048,   698,   733,  1048,  1048,
     106,  1048,   108,   107,   113,   164,     7,   132,  1255,    26,
     273,   521,   305,   943,    32,   978,   -71,   -71,   -71,    52,
      59,   134,   -71,   908,   -71,   -71,   143,   -71,   -71,   -71,
     -71,    29,   -71,   -71,   -71,   -71,   -71,   -71,   115,  1322,
    1394,  1377,  1360,  1343,   174,   174,   174,   174,   174,   174,
     -71,    76,   122,   -71,    76,   122,   125,   125,   -71,  1277,
     124,   136,   139,   -71,   -71,   108,  1048,   -71,   -71,   -71,
     344,   145,   -71,   -71,   379,  1048,  1208,   -71,   414,  1048,
    1232,   768,  1013,   -71,   -71,   164,   181,   -71,   -71,   184,
     150,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   183,   -71,
     -71,   -71,  1322,  1048,   -71,   -71,  1322,  1048,   -71,  1322,
     -71,  1300,   -71,   166,   167,   -71,   449,  1322,  1322,   -71,
     -71,   -71,   -71,   -71
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,   136,   137,   138,   140,     0,   135,    16,     0,     0,
       0,     0,     0,    41,     0,    89,    90,    91,    92,   105,
     106,   107,   108,   109,   110,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     5,     0,     0,    17,    14,     0,
      44,     5,     0,   139,   141,    19,     0,     0,   135,    52,
       0,     0,     0,    80,    79,    85,    86,    87,    88,    99,
     100,   101,   102,   103,   104,   125,   127,   119,   117,   120,
     118,    32,     0,     0,    40,     0,    68,     0,     0,   128,
       1,     3,     7,    13,    20,     8,     0,    21,     0,     0,
       0,     9,     4,    43,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    28,   129,     0,     0,    30,   143,     0,
       0,     0,     0,     0,     0,     0,    46,    45,    20,    73,
       0,    71,    35,     0,    39,    34,     0,    67,   123,   122,
     121,     0,     6,    12,    11,    10,    15,    23,     0,    78,
      81,    82,    83,    84,    93,    94,    95,    96,    97,    98,
     113,   124,   111,   114,   126,   112,   115,   116,   134,     0,
       0,   132,    31,    26,    24,     0,     0,   131,   130,    51,
       0,     0,   129,    55,     0,    66,     0,    58,     0,    65,
       0,     0,     0,    70,    69,     0,     0,    36,    33,     0,
      28,    27,    22,   133,    18,    29,   142,    48,    47,   132,
      54,    53,    64,    63,    57,    56,    62,    61,    77,    76,
      74,     0,    72,     0,     0,    25,     0,    60,    59,    75,
      38,    37,    50,    49
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,    44,   -71,   -34,     1,   -71,   -69,   171,    23,
     -71,   -32,   -70,     0,   -71,   -24,   151,    13,   -71,    -2,
     205,    41
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    32,    33,    34,    35,    72,    37,    38,    39,   116,
     117,    40,    73,    74,   124,    75,    76,   130,   131,    42,
      43,   119
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      41,    36,    86,   129,   134,   136,    47,    91,   173,    49,
      52,     4,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    68,    70,   177,    78,    79,
     200,    89,    45,   187,    41,    36,     5,    87,     7,   143,
      80,    41,    36,   174,   118,   120,   172,   115,   122,   133,
      50,    83,   137,    90,   144,   146,   145,     5,    84,     7,
     193,    82,   178,   196,    29,   201,    85,   -21,   188,    51,
     191,    29,   112,    87,     5,   194,     7,   121,    81,     1,
       2,     3,     4,   123,    48,    92,   141,   192,   170,   -21,
      87,    90,   138,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   162,   165,   166,   167,   142,   169,
      93,    30,   199,    31,   168,     5,    87,     7,     5,   118,
       7,   186,   147,   190,   148,    94,   129,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   115,   171,   197,   132,   -27,   110,   111,   175,
       5,   195,     7,   108,   109,   202,   -20,   -20,   204,    65,
     110,   111,    66,   110,   111,     1,     2,     3,     4,   -25,
      48,     5,   128,     7,   118,    46,   115,   198,    90,   -20,
     208,   209,   223,   212,   211,   224,   225,   216,   215,   219,
     221,    25,    26,    27,    28,   226,    87,    30,   205,    31,
     230,   231,   125,   106,   107,   108,   109,    88,   222,    44,
     126,   227,   110,   111,     0,   228,     0,   206,     0,     0,
       0,     0,     0,     0,     0,   127,   233,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   139,     0,     0,     0,     0,   110,   111,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   179,     0,     0,   140,     0,   110,
     111,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   183,     0,     0,   180,
       0,   110,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,   184,     0,   110,   111,   207,     0,     1,     2,     3,
       4,     0,    48,     0,     8,     0,     9,    10,    11,    12,
      13,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,    30,
     210,    31,     1,     2,     3,     4,     0,    48,     0,     8,
       0,     9,    10,    11,    12,    13,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     0,    30,   214,    31,     1,     2,     3,
       4,     0,    48,     0,     8,     0,     9,    10,    11,    12,
      13,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,    30,
     232,    31,     1,     2,     3,     4,     0,    48,     0,     8,
       0,     9,    10,    11,    12,    13,     0,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     0,    30,   113,    31,     1,     2,     3,
       4,     5,    48,     7,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,     0,    30,
     114,    31,   181,     0,     1,     2,     3,     4,     0,    48,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     0,     0,    30,   182,    31,    53,
       0,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,    67,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,    69,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,    77,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,   160,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,   161,    26,    27,
      28,     0,     0,    30,   163,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,   164,    27,    28,     0,     0,    30,   218,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,     0,    31,     1,     2,     3,     4,
       5,    48,     7,     8,     0,     9,    10,    11,    12,    13,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    71,    30,     0,
      31,     1,     2,     3,     4,     5,     6,     7,     8,     0,
       9,    10,    11,    12,    13,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     0,    30,     0,    31,     1,     2,     3,     4,
       0,    48,     0,     8,     0,     9,    10,    11,    12,    13,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,   135,    30,     0,
      31,     1,     2,     3,     4,     0,    48,     0,     8,     0,
       9,    10,    11,    12,    13,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     0,    30,     0,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,   185,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,     0,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,   189,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,     0,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,   220,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,     0,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    15,    16,    17,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,     0,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,    16,     0,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,     0,    31,     1,     2,     3,     4,
       0,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    15,     0,     0,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,     0,    30,     0,
      31,     1,     2,     3,     4,     0,    48,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,     0,    30,   213,    31,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,     0,   110,   111,   217,     0,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
     110,   111,   176,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,     0,   110,   111,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,   110,   111,   203,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,     0,   229,     0,   110,   111,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
     110,   111,    96,    97,    98,     0,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,    96,
      97,   110,   111,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,     0,    96,     0,   110,   111,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,   110,   111,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,     0,   110,   111
};

static const yytype_int16 yycheck[] =
{
       0,     0,    36,    72,    74,    75,    35,    39,     1,     9,
      12,     6,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     1,    30,    31,
       1,    17,     8,     1,    34,    34,     7,    36,     9,     1,
       0,    41,    41,    36,    46,    47,   115,    46,    50,    73,
      35,     1,    76,    39,    16,    89,    88,     7,     8,     9,
       1,    16,    36,   133,    33,    36,    16,    17,    36,    35,
      18,    33,    33,    72,     7,    16,     9,    35,    34,     3,
       4,     5,     6,    16,     8,    41,    35,    35,   112,    39,
      89,    39,    36,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    16,   111,
       1,    35,   136,    37,     8,     7,   115,     9,     7,   121,
       9,   123,     1,   125,     3,    16,   195,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,   141,    36,     1,    34,    33,    38,    39,    17,
       7,    17,     9,    31,    32,    40,    16,    17,    34,   161,
      38,    39,   164,    38,    39,     3,     4,     5,     6,    33,
       8,     7,     8,     9,   176,    35,   175,    34,    39,    39,
     180,    36,     1,   185,   184,     1,    36,   189,   188,   191,
     192,    29,    30,    31,    32,    12,   195,    35,   175,    37,
      34,    34,    51,    29,    30,    31,    32,    36,   195,     4,
       1,   213,    38,    39,    -1,   217,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,   226,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,     1,    -1,    -1,    -1,    -1,    38,    39,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,     1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,     1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    -1,    38,    39,     1,    -1,     3,     4,     5,
       6,    -1,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
       1,    37,     3,     4,     5,     6,    -1,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,     1,    37,     3,     4,     5,
       6,    -1,     8,    -1,    10,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    -1,    35,
       1,    37,     3,     4,     5,     6,    -1,     8,    -1,    10,
      -1,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,     1,    37,     3,     4,     5,
       6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    35,
      36,    37,     1,    -1,     3,     4,     5,     6,    -1,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    35,    36,    37,     1,
      -1,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,     1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,     1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,     1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,     1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,     1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,     1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    -1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    10,    -1,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    -1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    10,    -1,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    -1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    -1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    36,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    -1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    19,    20,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,    -1,    37,     3,     4,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    35,    -1,
      37,     3,     4,     5,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    35,    16,    37,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    38,    39,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    38,    39,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    40,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    36,    -1,    38,    39,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      38,    39,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    19,
      20,    38,    39,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    19,    -1,    38,    39,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    38,    39,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    14,    15,    16,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      35,    37,    48,    49,    50,    51,    52,    53,    54,    55,
      58,    60,    66,    67,    67,     8,    35,    35,     8,    60,
      35,    35,    66,     1,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,     1,    66,     1,
      66,    34,    52,    59,    60,    62,    63,     1,    66,    66,
       0,    49,    16,     1,     8,    16,    51,    52,    55,    17,
      39,    58,    49,     1,    16,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      38,    39,    33,     1,    36,    52,    56,    57,    66,    68,
      66,    35,    66,    16,    61,    63,     1,    16,     8,    54,
      64,    65,    34,    62,    59,    34,    59,    62,    36,     1,
      36,    35,    16,     1,    16,    58,    51,     1,     3,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
       1,    29,    66,     1,    30,    66,    66,    66,     8,    66,
      62,    36,    54,     1,    36,    17,    17,     1,    36,     1,
      36,     1,    36,     1,    36,    16,    66,     1,    36,    16,
      66,    18,    35,     1,    16,    17,    59,     1,    34,    62,
       1,    36,    40,    40,    34,    56,    68,     1,    60,    36,
       1,    60,    66,    16,     1,    60,    66,    16,     1,    66,
      36,    66,    64,     1,     1,    36,    12,    66,    66,    36,
      34,    34,     1,    60
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    50,    50,    50,    50,
      50,    50,    50,    50,    51,    51,    52,    52,    53,    53,
      54,    54,    54,    54,    55,    55,    55,    55,    55,    56,
      56,    57,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    61,
      61,    61,    61,    61,    61,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    68,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     3,     2,     2,     2,
       3,     3,     3,     2,     1,     3,     1,     1,     5,     2,
       1,     1,     4,     3,     4,     4,     4,     3,     3,     3,
       1,     2,     2,     4,     3,     3,     4,     6,     6,     2,
       1,     1,     2,     2,     1,     3,     3,     5,     5,     7,
       7,     4,     2,     5,     5,     4,     5,     5,     4,     4,
       4,     3,     3,     3,     3,     2,     2,     2,     1,     3,
       3,     1,     3,     1,     3,     4,     3,     3,     3,     2,
       2,     3,     3,     3,     3,     2,     2,     2,     2,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     3,     3,     3,     3,     2,     3,     2,     2,     3,
       4,     4,     4,     4,     3,     1,     1,     1,     1,     1,
       1,     2,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
    YYNOMEM;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 43 "src/syntax.y"
                    {
        root = yyval = create_parent_node(AST_PROGRAM, (yyloc).first_line, 1, yyvsp[0]);
        if (!err_flag)
            print_ast(yyval);
      }
#line 1863 "syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 51 "src/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1869 "syntax.tab.c"
    break;

  case 4: /* ExtDefList: Stmt ExtDefList  */
#line 52 "src/syntax.y"
                     { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "statement is not allowed at global scope"); yyval = create_parent_node(AST_EXT_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); yyerrok; }
#line 1875 "syntax.tab.c"
    break;

  case 5: /* ExtDefList: %empty  */
#line 53 "src/syntax.y"
      { yyval = NULL; }
#line 1881 "syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier ExtDecList SEMI  */
#line 57 "src/syntax.y"
                                  { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1887 "syntax.tab.c"
    break;

  case 7: /* ExtDef: ExtDecList SEMI  */
#line 58 "src/syntax.y"
                      { splwarn((yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "declaration is missing a specifier and will default to int"); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1893 "syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier SEMI  */
#line 59 "src/syntax.y"
                     { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1899 "syntax.tab.c"
    break;

  case 9: /* ExtDef: FuncDec CompStmt  */
#line 60 "src/syntax.y"
                       { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "function is missing a specifier"); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1905 "syntax.tab.c"
    break;

  case 10: /* ExtDef: Specifier FuncDec CompStmt  */
#line 62 "src/syntax.y"
                                 { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1911 "syntax.tab.c"
    break;

  case 11: /* ExtDef: Specifier FuncDec SEMI  */
#line 63 "src/syntax.y"
                             { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1917 "syntax.tab.c"
    break;

  case 12: /* ExtDef: Specifier FuncDec error  */
#line 64 "src/syntax.y"
                              { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid function body"); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1923 "syntax.tab.c"
    break;

  case 13: /* ExtDef: Specifier error  */
#line 65 "src/syntax.y"
                      { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "missing valid identifier"); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1929 "syntax.tab.c"
    break;

  case 14: /* ExtDecList: VarDec  */
#line 69 "src/syntax.y"
                   { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1935 "syntax.tab.c"
    break;

  case 15: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 70 "src/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1941 "syntax.tab.c"
    break;

  case 16: /* Specifier: TYPE  */
#line 73 "src/syntax.y"
                { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1947 "syntax.tab.c"
    break;

  case 17: /* Specifier: StructSpecifier  */
#line 74 "src/syntax.y"
                      { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1953 "syntax.tab.c"
    break;

  case 18: /* StructSpecifier: STRUCT ID LC DefList RC  */
#line 78 "src/syntax.y"
                                         { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1959 "syntax.tab.c"
    break;

  case 19: /* StructSpecifier: STRUCT ID  */
#line 79 "src/syntax.y"
                { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1965 "syntax.tab.c"
    break;

  case 20: /* VarDec: ID  */
#line 83 "src/syntax.y"
           { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1971 "syntax.tab.c"
    break;

  case 21: /* VarDec: Specifier  */
#line 84 "src/syntax.y"
                { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "two or more data types in declaration specifiers"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1977 "syntax.tab.c"
    break;

  case 22: /* VarDec: VarDec LSB INT RSB  */
#line 85 "src/syntax.y"
                         { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1983 "syntax.tab.c"
    break;

  case 23: /* VarDec: VarDec LSB error  */
#line 86 "src/syntax.y"
                       { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing bracket ']'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1989 "syntax.tab.c"
    break;

  case 24: /* FuncDec: ID LP VarList RP  */
#line 91 "src/syntax.y"
                          { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1995 "syntax.tab.c"
    break;

  case 25: /* FuncDec: ID LP error RP  */
#line 92 "src/syntax.y"
                     { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid parameter declaration ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2001 "syntax.tab.c"
    break;

  case 26: /* FuncDec: ID LP VarList error  */
#line 94 "src/syntax.y"
                          { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2007 "syntax.tab.c"
    break;

  case 27: /* FuncDec: ID LP RP  */
#line 95 "src/syntax.y"
               { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2013 "syntax.tab.c"
    break;

  case 28: /* FuncDec: ID LP error  */
#line 96 "src/syntax.y"
                  { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2019 "syntax.tab.c"
    break;

  case 29: /* VarList: ParamDec COMMA VarList  */
#line 100 "src/syntax.y"
                                { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2025 "syntax.tab.c"
    break;

  case 30: /* VarList: ParamDec  */
#line 101 "src/syntax.y"
               { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 2031 "syntax.tab.c"
    break;

  case 31: /* ParamDec: Specifier VarDec  */
#line 105 "src/syntax.y"
                           { yyval = create_parent_node(AST_PARAM_DEC, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2037 "syntax.tab.c"
    break;

  case 32: /* CompStmt: LC RC  */
#line 109 "src/syntax.y"
                { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 2043 "syntax.tab.c"
    break;

  case 33: /* CompStmt: LC DefList StmtList RC  */
#line 110 "src/syntax.y"
                             { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2049 "syntax.tab.c"
    break;

  case 34: /* CompStmt: LC DefList RC  */
#line 111 "src/syntax.y"
                    { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2055 "syntax.tab.c"
    break;

  case 35: /* CompStmt: LC StmtList RC  */
#line 112 "src/syntax.y"
                     { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2061 "syntax.tab.c"
    break;

  case 36: /* CompStmt: LC DefList StmtList error  */
#line 113 "src/syntax.y"
                                { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing brace '}'"); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2067 "syntax.tab.c"
    break;

  case 37: /* CompStmt: LC DefList StmtList DefList error RC  */
#line 114 "src/syntax.y"
                                           { splerror(SPLC_ERR_B, (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yylsp[-3]).last_line, (yylsp[-3]).last_column, "cannot interleave definitions with statements. "); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 2073 "syntax.tab.c"
    break;

  case 38: /* CompStmt: LC StmtList DefList StmtList error RC  */
#line 115 "src/syntax.y"
                                            { splerror(SPLC_ERR_B, (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yylsp[-3]).last_line, (yylsp[-3]).last_column, "cannot interleave definitions with statements. "); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 2079 "syntax.tab.c"
    break;

  case 39: /* StmtList: Stmt StmtList  */
#line 119 "src/syntax.y"
                        { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2085 "syntax.tab.c"
    break;

  case 40: /* StmtList: Stmt  */
#line 120 "src/syntax.y"
           { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 2091 "syntax.tab.c"
    break;

  case 41: /* Stmt: SEMI  */
#line 124 "src/syntax.y"
           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 2097 "syntax.tab.c"
    break;

  case 42: /* Stmt: Exp SEMI  */
#line 125 "src/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2103 "syntax.tab.c"
    break;

  case 43: /* Stmt: Exp error  */
#line 126 "src/syntax.y"
                { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2109 "syntax.tab.c"
    break;

  case 44: /* Stmt: CompStmt  */
#line 128 "src/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 2115 "syntax.tab.c"
    break;

  case 45: /* Stmt: RETURN Exp SEMI  */
#line 130 "src/syntax.y"
                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2121 "syntax.tab.c"
    break;

  case 46: /* Stmt: RETURN Exp error  */
#line 131 "src/syntax.y"
                       { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 2127 "syntax.tab.c"
    break;

  case 47: /* Stmt: IF LP Exp RP Stmt  */
#line 133 "src/syntax.y"
                                   { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2133 "syntax.tab.c"
    break;

  case 48: /* Stmt: IF LP Exp RP error  */
#line 134 "src/syntax.y"
                         { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "if requires at least one statement to be executed"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2139 "syntax.tab.c"
    break;

  case 49: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 135 "src/syntax.y"
                                  { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2145 "syntax.tab.c"
    break;

  case 50: /* Stmt: IF LP Exp RP Stmt ELSE error  */
#line 136 "src/syntax.y"
                                   { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2151 "syntax.tab.c"
    break;

  case 51: /* Stmt: IF LP Exp error  */
#line 137 "src/syntax.y"
                      { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2157 "syntax.tab.c"
    break;

  case 52: /* Stmt: ELSE Stmt  */
#line 138 "src/syntax.y"
                { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "hanging else is not allowed."); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); yyerrok; }
#line 2163 "syntax.tab.c"
    break;

  case 53: /* Stmt: WHILE LP Exp RP Stmt  */
#line 140 "src/syntax.y"
                           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2169 "syntax.tab.c"
    break;

  case 54: /* Stmt: WHILE LP Exp RP error  */
#line 141 "src/syntax.y"
                            { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "while loop requires at least one statement to be executed"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok;  }
#line 2175 "syntax.tab.c"
    break;

  case 55: /* Stmt: WHILE LP Exp error  */
#line 143 "src/syntax.y"
                         { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok;  }
#line 2181 "syntax.tab.c"
    break;

  case 56: /* Stmt: FOR LP ForLoopBody RP Stmt  */
#line 145 "src/syntax.y"
                                 { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2187 "syntax.tab.c"
    break;

  case 57: /* Stmt: FOR LP ForLoopBody RP error  */
#line 146 "src/syntax.y"
                                  { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "for loop requires at least one statement to be executed"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2193 "syntax.tab.c"
    break;

  case 58: /* Stmt: FOR LP ForLoopBody error  */
#line 147 "src/syntax.y"
                               { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 2199 "syntax.tab.c"
    break;

  case 59: /* ForLoopBody: Def Exp SEMI Exp  */
#line 150 "src/syntax.y"
                              { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2205 "syntax.tab.c"
    break;

  case 60: /* ForLoopBody: SEMI Exp SEMI Exp  */
#line 152 "src/syntax.y"
                        { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2211 "syntax.tab.c"
    break;

  case 61: /* ForLoopBody: Def Exp SEMI  */
#line 153 "src/syntax.y"
                   { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2217 "syntax.tab.c"
    break;

  case 62: /* ForLoopBody: Def SEMI Exp  */
#line 154 "src/syntax.y"
                   { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2223 "syntax.tab.c"
    break;

  case 63: /* ForLoopBody: SEMI Exp SEMI  */
#line 156 "src/syntax.y"
                    { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2229 "syntax.tab.c"
    break;

  case 64: /* ForLoopBody: SEMI SEMI Exp  */
#line 157 "src/syntax.y"
                    { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2235 "syntax.tab.c"
    break;

  case 65: /* ForLoopBody: Def SEMI  */
#line 158 "src/syntax.y"
               { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2241 "syntax.tab.c"
    break;

  case 66: /* ForLoopBody: SEMI SEMI  */
#line 160 "src/syntax.y"
                { yyval = create_parent_node(AST_FOR_LOOP_BODY, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2247 "syntax.tab.c"
    break;

  case 67: /* DefList: Def DefList  */
#line 164 "src/syntax.y"
                     { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2253 "syntax.tab.c"
    break;

  case 68: /* DefList: Def  */
#line 165 "src/syntax.y"
          { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 2259 "syntax.tab.c"
    break;

  case 69: /* Def: Specifier DecList SEMI  */
#line 169 "src/syntax.y"
                            { yyval = create_parent_node(AST_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2265 "syntax.tab.c"
    break;

  case 70: /* Def: Specifier DecList error  */
#line 170 "src/syntax.y"
                              { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_DEF, (yyloc).first_line, 0); yyerrok; }
#line 2271 "syntax.tab.c"
    break;

  case 71: /* DecList: Dec  */
#line 174 "src/syntax.y"
             { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 2277 "syntax.tab.c"
    break;

  case 72: /* DecList: Dec COMMA DecList  */
#line 175 "src/syntax.y"
                        { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2283 "syntax.tab.c"
    break;

  case 73: /* Dec: VarDec  */
#line 179 "src/syntax.y"
            { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 2289 "syntax.tab.c"
    break;

  case 74: /* Dec: VarDec LP RP  */
#line 180 "src/syntax.y"
                   { splerror(SPLC_ERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "function definition not allowed here."); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2295 "syntax.tab.c"
    break;

  case 75: /* Dec: VarDec LP Exp RP  */
#line 181 "src/syntax.y"
                       { splerror(SPLC_ERR_B, (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "function definition not allowed here."); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2301 "syntax.tab.c"
    break;

  case 76: /* Dec: VarDec ASSIGN Exp  */
#line 182 "src/syntax.y"
                        { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2307 "syntax.tab.c"
    break;

  case 77: /* Dec: VarDec ASSIGN error  */
#line 183 "src/syntax.y"
                          { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "invalid initialization"); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2313 "syntax.tab.c"
    break;

  case 78: /* Exp: Exp ASSIGN Exp  */
#line 187 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2319 "syntax.tab.c"
    break;

  case 79: /* Exp: ASSIGN Exp  */
#line 188 "src/syntax.y"
                 {  splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2325 "syntax.tab.c"
    break;

  case 80: /* Exp: ASSIGN error  */
#line 189 "src/syntax.y"
                   {  splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression after '='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2331 "syntax.tab.c"
    break;

  case 81: /* Exp: Exp AND Exp  */
#line 191 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2337 "syntax.tab.c"
    break;

  case 82: /* Exp: Exp OR Exp  */
#line 192 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2343 "syntax.tab.c"
    break;

  case 83: /* Exp: Exp BITWISE_AND Exp  */
#line 193 "src/syntax.y"
                          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2349 "syntax.tab.c"
    break;

  case 84: /* Exp: Exp BITWISE_OR Exp  */
#line 194 "src/syntax.y"
                         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2355 "syntax.tab.c"
    break;

  case 85: /* Exp: AND Exp  */
#line 195 "src/syntax.y"
              { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '&&'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2361 "syntax.tab.c"
    break;

  case 86: /* Exp: OR Exp  */
#line 196 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '||'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2367 "syntax.tab.c"
    break;

  case 87: /* Exp: BITWISE_AND Exp  */
#line 197 "src/syntax.y"
                      { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '&'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2373 "syntax.tab.c"
    break;

  case 88: /* Exp: BITWISE_OR Exp  */
#line 198 "src/syntax.y"
                     { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '|'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2379 "syntax.tab.c"
    break;

  case 89: /* Exp: AND  */
#line 199 "src/syntax.y"
          { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '&&'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2385 "syntax.tab.c"
    break;

  case 90: /* Exp: OR  */
#line 200 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '||'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2391 "syntax.tab.c"
    break;

  case 91: /* Exp: BITWISE_AND  */
#line 201 "src/syntax.y"
                  { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '&'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2397 "syntax.tab.c"
    break;

  case 92: /* Exp: BITWISE_OR  */
#line 202 "src/syntax.y"
                 { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '|'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2403 "syntax.tab.c"
    break;

  case 93: /* Exp: Exp LT Exp  */
#line 208 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2409 "syntax.tab.c"
    break;

  case 94: /* Exp: Exp LE Exp  */
#line 209 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2415 "syntax.tab.c"
    break;

  case 95: /* Exp: Exp GT Exp  */
#line 210 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2421 "syntax.tab.c"
    break;

  case 96: /* Exp: Exp GE Exp  */
#line 211 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2427 "syntax.tab.c"
    break;

  case 97: /* Exp: Exp NE Exp  */
#line 212 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2433 "syntax.tab.c"
    break;

  case 98: /* Exp: Exp EQ Exp  */
#line 213 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2439 "syntax.tab.c"
    break;

  case 99: /* Exp: LT Exp  */
#line 214 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '<'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2445 "syntax.tab.c"
    break;

  case 100: /* Exp: LE Exp  */
#line 215 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '<='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2451 "syntax.tab.c"
    break;

  case 101: /* Exp: GT Exp  */
#line 216 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '>'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2457 "syntax.tab.c"
    break;

  case 102: /* Exp: GE Exp  */
#line 217 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '>='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2463 "syntax.tab.c"
    break;

  case 103: /* Exp: NE Exp  */
#line 218 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '!='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2469 "syntax.tab.c"
    break;

  case 104: /* Exp: EQ Exp  */
#line 219 "src/syntax.y"
             { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '=='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2475 "syntax.tab.c"
    break;

  case 105: /* Exp: LT  */
#line 220 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '<'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2481 "syntax.tab.c"
    break;

  case 106: /* Exp: LE  */
#line 221 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '<='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2487 "syntax.tab.c"
    break;

  case 107: /* Exp: GT  */
#line 222 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '>'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2493 "syntax.tab.c"
    break;

  case 108: /* Exp: GE  */
#line 223 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '>='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2499 "syntax.tab.c"
    break;

  case 109: /* Exp: NE  */
#line 224 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '!='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2505 "syntax.tab.c"
    break;

  case 110: /* Exp: EQ  */
#line 225 "src/syntax.y"
         { splerror(SPLC_ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "expected expression after '=='"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2511 "syntax.tab.c"
    break;

  case 111: /* Exp: Exp PLUS Exp  */
#line 227 "src/syntax.y"
                   { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2517 "syntax.tab.c"
    break;

  case 112: /* Exp: Exp MINUS Exp  */
#line 228 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2523 "syntax.tab.c"
    break;

  case 113: /* Exp: Exp PLUS error  */
#line 229 "src/syntax.y"
                     { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression after '+'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2529 "syntax.tab.c"
    break;

  case 114: /* Exp: Exp MINUS error  */
#line 230 "src/syntax.y"
                      { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression after '-'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2535 "syntax.tab.c"
    break;

  case 115: /* Exp: Exp MUL Exp  */
#line 232 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2541 "syntax.tab.c"
    break;

  case 116: /* Exp: Exp DIV Exp  */
#line 233 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2547 "syntax.tab.c"
    break;

  case 117: /* Exp: MUL Exp  */
#line 234 "src/syntax.y"
              { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '*'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2553 "syntax.tab.c"
    break;

  case 118: /* Exp: DIV Exp  */
#line 235 "src/syntax.y"
              { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression before '/'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2559 "syntax.tab.c"
    break;

  case 119: /* Exp: MUL error  */
#line 236 "src/syntax.y"
                { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression after '*'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2565 "syntax.tab.c"
    break;

  case 120: /* Exp: DIV error  */
#line 237 "src/syntax.y"
                { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "expected expression after '/'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2571 "syntax.tab.c"
    break;

  case 121: /* Exp: LP Exp RP  */
#line 239 "src/syntax.y"
                { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2577 "syntax.tab.c"
    break;

  case 122: /* Exp: LP Exp error  */
#line 240 "src/syntax.y"
                   { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2583 "syntax.tab.c"
    break;

  case 123: /* Exp: LP error RP  */
#line 241 "src/syntax.y"
                  { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid expression"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2589 "syntax.tab.c"
    break;

  case 124: /* Exp: Exp PLUS PLUS  */
#line 243 "src/syntax.y"
                                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2595 "syntax.tab.c"
    break;

  case 125: /* Exp: PLUS Exp  */
#line 245 "src/syntax.y"
                           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2601 "syntax.tab.c"
    break;

  case 126: /* Exp: Exp MINUS MINUS  */
#line 246 "src/syntax.y"
                                       { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2607 "syntax.tab.c"
    break;

  case 127: /* Exp: MINUS Exp  */
#line 248 "src/syntax.y"
                             { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2613 "syntax.tab.c"
    break;

  case 128: /* Exp: NOT Exp  */
#line 249 "src/syntax.y"
              { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2619 "syntax.tab.c"
    break;

  case 129: /* Exp: ID LP RP  */
#line 250 "src/syntax.y"
               { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2625 "syntax.tab.c"
    break;

  case 130: /* Exp: ID LP Args RP  */
#line 251 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2631 "syntax.tab.c"
    break;

  case 131: /* Exp: ID LP Args error  */
#line 252 "src/syntax.y"
                       { splerror(SPLC_ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2637 "syntax.tab.c"
    break;

  case 132: /* Exp: ID LP error RP  */
#line 253 "src/syntax.y"
                     { splerror(SPLC_ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid argument list"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2643 "syntax.tab.c"
    break;

  case 133: /* Exp: Exp LSB Exp RSB  */
#line 255 "src/syntax.y"
                      { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2649 "syntax.tab.c"
    break;

  case 134: /* Exp: Exp DOT ID  */
#line 256 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2655 "syntax.tab.c"
    break;

  case 135: /* Exp: ID  */
#line 258 "src/syntax.y"
         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2661 "syntax.tab.c"
    break;

  case 136: /* Exp: INT  */
#line 259 "src/syntax.y"
          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2667 "syntax.tab.c"
    break;

  case 137: /* Exp: FLOAT  */
#line 260 "src/syntax.y"
            { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2673 "syntax.tab.c"
    break;

  case 138: /* Exp: CHAR  */
#line 261 "src/syntax.y"
           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2679 "syntax.tab.c"
    break;

  case 139: /* Exp: StrExp  */
#line 262 "src/syntax.y"
             { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2685 "syntax.tab.c"
    break;

  case 140: /* StrExp: STR  */
#line 266 "src/syntax.y"
            { yyval = create_parent_node(AST_STREXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2691 "syntax.tab.c"
    break;

  case 141: /* StrExp: STR StrExp  */
#line 267 "src/syntax.y"
                 { yyval = create_parent_node(AST_STREXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2697 "syntax.tab.c"
    break;

  case 142: /* Args: Exp COMMA Args  */
#line 271 "src/syntax.y"
                     { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2703 "syntax.tab.c"
    break;

  case 143: /* Args: Exp  */
#line 272 "src/syntax.y"
          { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 1, yyvsp[0]); }
#line 2709 "syntax.tab.c"
    break;


#line 2713 "syntax.tab.c"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  ++yynerrs;

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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 275 "src/syntax.y"


void yyerror(const char *s) {
    // suppressed
    /* fprintf(stderr, "%s at line %d\n", s, yylloc.first_line); */
}
