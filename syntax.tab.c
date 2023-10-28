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
  YYSYMBOL_STRUCT = 6,                     /* STRUCT  */
  YYSYMBOL_ID = 7,                         /* ID  */
  YYSYMBOL_TYPE = 8,                       /* TYPE  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_THEN = 10,                      /* THEN  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_FOR = 13,                       /* FOR  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_SEMI = 15,                      /* SEMI  */
  YYSYMBOL_COMMA = 16,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 17,                    /* ASSIGN  */
  YYSYMBOL_AND = 18,                       /* AND  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_BITWISE_AND = 20,               /* BITWISE_AND  */
  YYSYMBOL_BITWISE_OR = 21,                /* BITWISE_OR  */
  YYSYMBOL_LT = 22,                        /* LT  */
  YYSYMBOL_LE = 23,                        /* LE  */
  YYSYMBOL_GT = 24,                        /* GT  */
  YYSYMBOL_GE = 25,                        /* GE  */
  YYSYMBOL_NE = 26,                        /* NE  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_PLUS = 28,                      /* PLUS  */
  YYSYMBOL_MINUS = 29,                     /* MINUS  */
  YYSYMBOL_MUL = 30,                       /* MUL  */
  YYSYMBOL_DIV = 31,                       /* DIV  */
  YYSYMBOL_LC = 32,                        /* LC  */
  YYSYMBOL_RC = 33,                        /* RC  */
  YYSYMBOL_LP = 34,                        /* LP  */
  YYSYMBOL_RP = 35,                        /* RP  */
  YYSYMBOL_NOT = 36,                       /* NOT  */
  YYSYMBOL_DOT = 37,                       /* DOT  */
  YYSYMBOL_LSB = 38,                       /* LSB  */
  YYSYMBOL_RSB = 39,                       /* RSB  */
  YYSYMBOL_UPLUS = 40,                     /* UPLUS  */
  YYSYMBOL_UMINUS = 41,                    /* UMINUS  */
  YYSYMBOL_PRE_PLUS = 42,                  /* PRE_PLUS  */
  YYSYMBOL_PRE_MINUS = 43,                 /* PRE_MINUS  */
  YYSYMBOL_POST_PLUS = 44,                 /* POST_PLUS  */
  YYSYMBOL_POST_MINUS = 45,                /* POST_MINUS  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_Program = 47,                   /* Program  */
  YYSYMBOL_ExtDefList = 48,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 49,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 50,                /* ExtDecList  */
  YYSYMBOL_Specifier = 51,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 52,           /* StructSpecifier  */
  YYSYMBOL_VarDec = 53,                    /* VarDec  */
  YYSYMBOL_FuncDec = 54,                   /* FuncDec  */
  YYSYMBOL_VarList = 55,                   /* VarList  */
  YYSYMBOL_ParamDec = 56,                  /* ParamDec  */
  YYSYMBOL_CompStmt = 57,                  /* CompStmt  */
  YYSYMBOL_StmtList = 58,                  /* StmtList  */
  YYSYMBOL_Stmt = 59,                      /* Stmt  */
  YYSYMBOL_DefList = 60,                   /* DefList  */
  YYSYMBOL_Def = 61,                       /* Def  */
  YYSYMBOL_DecList = 62,                   /* DecList  */
  YYSYMBOL_Dec = 63,                       /* Dec  */
  YYSYMBOL_Exp = 64,                       /* Exp  */
  YYSYMBOL_Args = 65                       /* Args  */
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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   673

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  96
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  173

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


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
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    43,    43,    51,    52,    56,    57,    58,    59,    63,
      64,    67,    68,    72,    73,    77,    78,    79,    80,    81,
      85,    86,    87,    88,    89,    93,    94,    98,   102,   103,
     104,   105,   106,   107,   108,   112,   113,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   134,   135,   139,   140,   144,   145,   149,   150,   151,
     152,   153,   157,   159,   160,   161,   162,   164,   165,   166,
     167,   168,   169,   171,   172,   173,   174,   176,   177,   178,
     180,   182,   183,   185,   186,   187,   188,   189,   190,   192,
     193,   195,   196,   197,   198,   202,   203
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
  "end of file", "error", "invalid token", "INT", "FLOAT", "CHAR",
  "STRUCT", "ID", "TYPE", "IF", "THEN", "ELSE", "WHILE", "FOR", "RETURN",
  "SEMI", "COMMA", "ASSIGN", "AND", "OR", "BITWISE_AND", "BITWISE_OR",
  "LT", "LE", "GT", "GE", "NE", "EQ", "PLUS", "MINUS", "MUL", "DIV", "LC",
  "RC", "LP", "RP", "NOT", "DOT", "LSB", "RSB", "UPLUS", "UMINUS",
  "PRE_PLUS", "PRE_MINUS", "POST_PLUS", "POST_MINUS", "$accept", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "VarDec", "FuncDec", "VarList", "ParamDec", "CompStmt", "StmtList",
  "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-46)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-58)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       5,    14,   -46,    26,   -46,     5,   307,   -46,   -10,   -46,
     -46,   -46,    -4,   -46,    54,   -46,     1,    38,     5,    56,
     -46,   -46,    12,    87,   324,   -46,    12,     8,     5,    46,
     -46,    12,    31,    69,   -46,   -46,   -46,    50,   -46,   -46,
     -46,    59,    67,   392,    70,    71,    79,   -46,    79,    79,
     -46,   213,    79,   -46,    89,   392,   358,   113,    62,    22,
      93,   -46,   -46,   -46,    64,   -46,   -46,     5,   -46,   151,
      79,   -46,    79,     5,   144,    37,    37,    68,   175,    37,
     -46,   392,   -46,   -46,    28,   -46,   -46,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,   439,   448,
      79,    79,   103,    79,   290,   405,   -46,   -46,    12,   -46,
      81,   -46,   469,    32,   514,   536,    79,   -46,   -46,   -46,
     -46,   -46,   120,   -46,   -46,   123,   580,   434,   635,   618,
     601,   312,   312,   312,   312,   312,   312,   266,    88,   266,
      88,    37,    37,   -46,   491,   -46,   580,   -46,   558,   -46,
     -46,    79,   -46,   -46,   392,   392,   206,   -46,   -46,   -46,
     -46,   -46,   118,   -46,   -46,    79,   392,   237,   -46,   -46,
     275,   -46,   -46
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,    11,     0,     2,     4,     0,    12,    14,     1,
       3,     8,    15,     6,     0,    16,     0,     0,     0,     0,
       5,    19,     0,     0,     0,     7,     0,     0,    52,    24,
      23,     0,     0,    26,    15,    10,    18,     0,    92,    93,
      94,    91,     0,     0,     0,     0,     0,    37,     0,     0,
      28,     0,     0,    40,     0,    36,     0,     0,     0,     0,
      55,    13,    51,    21,     0,    22,    20,     0,    17,     0,
       0,    45,     0,     0,     0,    81,    83,     0,     0,    84,
      31,     0,    35,    30,     0,    39,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    53,     0,    25,
       0,    85,    96,     0,     0,     0,     0,    42,    41,    79,
      78,    77,     0,    32,    29,     0,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    80,    73,    82,
      74,    75,    76,    90,     0,    61,    60,    58,     0,    56,
      88,     0,    87,    86,     0,     0,     0,    34,    33,    89,
      59,    95,    43,    46,    50,     0,     0,     0,    44,    49,
       0,    48,    47
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -46,   -46,   141,   -46,   125,     9,   -46,   -21,   -46,    82,
     -46,   131,    17,   -43,   -16,    80,    49,   -46,   -45,    27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,    14,    26,     7,    16,    17,    32,
      33,    53,    54,    55,    27,    28,    59,    60,    57,   113
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,    74,    21,    75,    76,    58,    78,    79,    56,     6,
      64,     1,    62,     2,     6,    15,    -9,    22,     1,    34,
       2,     8,    18,   106,   112,   114,     9,   115,    31,   123,
      19,    15,    65,   152,     1,    15,     2,   107,    81,    23,
      15,    61,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   138,   140,   141,   142,    29,   144,   146,
     148,   124,     1,    21,     2,    21,    66,   153,   125,    20,
      24,   156,    82,    84,   102,   103,    31,   -57,   -57,   104,
     -27,    63,    38,    39,    40,    67,    41,    58,    36,    68,
      37,    30,    75,    69,    76,     1,   105,     2,   122,   -27,
      23,    70,    23,   119,    72,    73,   112,    48,    49,   108,
     143,   162,   163,    51,    85,    52,   150,    15,   100,   101,
     167,   157,    80,   168,   158,   102,   103,   172,    86,   166,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   117,    10,    35,    25,   109,
     102,   103,   110,   116,    38,    39,    40,   149,    41,   118,
       0,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   120,     0,   161,    48,
      49,   102,   103,     0,     0,    51,   111,    52,     0,     0,
       0,     0,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   164,     0,     0,
     121,     0,   102,   103,    77,     0,    38,    39,    40,     0,
      41,   165,     0,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   169,     0,
       0,    48,    49,   102,   103,     0,     0,    51,     0,    52,
       0,     0,     0,     0,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    38,
      39,    40,   170,    41,   102,   103,   171,     0,    38,    39,
      40,     0,    41,     0,    42,     0,    43,    44,    45,    46,
      47,   145,     0,    38,    39,    40,     0,    41,     0,     0,
      51,     0,    52,    48,    49,     0,     0,    24,    11,    51,
       0,    52,     0,     1,    12,     2,     0,     0,    48,    49,
       0,     0,    13,     0,    51,     0,    52,    38,    39,    40,
       1,    41,     2,    42,     0,    43,    44,    45,    46,    47,
      98,    99,   100,   101,     0,     0,     0,     0,     0,   102,
     103,     0,    48,    49,     0,     0,    24,    50,    51,     0,
      52,    38,    39,    40,     0,    41,     0,    42,     0,    43,
      44,    45,    46,    47,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,    49,     0,     0,
      24,    83,    51,     0,    52,    38,    39,    40,     0,    41,
       0,    42,     0,    43,    44,    45,    46,    47,    38,    39,
      40,     0,    41,     0,     0,     0,     0,     0,     0,     0,
      48,    49,     0,     0,    24,     0,    51,     0,    52,     0,
       0,     0,     0,    48,    49,     0,     0,     0,     0,    51,
     147,    52,    38,    39,    40,     0,    41,     0,     0,     0,
       0,    38,    39,    40,     0,    41,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,     0,   137,    49,     0,
       0,   102,   103,    51,     0,    52,    48,   139,     0,     0,
       0,     0,    51,     0,    52,   151,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,     0,     0,     0,     0,     0,   102,   103,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,     0,     0,     0,     0,     0,   102,   103,
     159,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,     0,     0,     0,   154,
       0,   102,   103,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,     0,     0,
       0,   155,     0,   102,   103,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
       0,     0,     0,   160,     0,   102,   103,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,     0,     0,     0,     0,     0,   102,   103,    88,
      89,    90,     0,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,     0,     0,     0,    88,    89,   102,   103,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
       0,     0,     0,    88,     0,   102,   103,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,     0,     0,     0,
       0,     0,   102,   103
};

static const yytype_int16 yycheck[] =
{
      43,    46,     1,    48,    49,    26,    51,    52,    24,     0,
      31,     6,    28,     8,     5,     6,    15,    16,     6,     7,
       8,     7,    32,     1,    69,    70,     0,    72,    19,     1,
      34,    22,     1,     1,     6,    26,     8,    15,    54,    38,
      31,    33,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,     1,   103,   104,
     105,    33,     6,     1,     8,     1,    35,    35,    84,    15,
      32,   116,    55,    56,    37,    38,    67,    15,    16,    17,
      16,    35,     3,     4,     5,    16,     7,   108,     1,    39,
       3,    35,   137,    34,   139,     6,    34,     8,    81,    35,
      38,    34,    38,    35,    34,    34,   151,    28,    29,    16,
       7,   154,   155,    34,     1,    36,    35,   108,    30,    31,
     165,     1,    33,   166,     1,    37,    38,   170,    15,    11,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     1,     5,    22,    17,    67,
      37,    38,     1,    73,     3,     4,     5,   108,     7,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     1,    -1,   151,    28,
      29,    37,    38,    -1,    -1,    34,    35,    36,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     1,    -1,    -1,
      35,    -1,    37,    38,     1,    -1,     3,     4,     5,    -1,
       7,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     1,    -1,
      -1,    28,    29,    37,    38,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     3,
       4,     5,    35,     7,    37,    38,     1,    -1,     3,     4,
       5,    -1,     7,    -1,     9,    -1,    11,    12,    13,    14,
      15,     1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      34,    -1,    36,    28,    29,    -1,    -1,    32,     1,    34,
      -1,    36,    -1,     6,     7,     8,    -1,    -1,    28,    29,
      -1,    -1,    15,    -1,    34,    -1,    36,     3,     4,     5,
       6,     7,     8,     9,    -1,    11,    12,    13,    14,    15,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,
      38,    -1,    28,    29,    -1,    -1,    32,    33,    34,    -1,
      36,     3,     4,     5,    -1,     7,    -1,     9,    -1,    11,
      12,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
      32,    33,    34,    -1,    36,     3,     4,     5,    -1,     7,
      -1,     9,    -1,    11,    12,    13,    14,    15,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    -1,    32,    -1,    34,    -1,    36,    -1,
      -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    34,
      35,    36,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    28,    29,    -1,
      -1,    37,    38,    34,    -1,    36,    28,    29,    -1,    -1,
      -1,    -1,    34,    -1,    36,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    35,
      -1,    37,    38,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      -1,    35,    -1,    37,    38,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    35,    -1,    37,    38,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    18,    19,    37,    38,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    18,    -1,    37,    38,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      -1,    -1,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     8,    47,    48,    49,    51,    52,     7,     0,
      48,     1,     7,    15,    50,    51,    53,    54,    32,    34,
      15,     1,    16,    38,    32,    57,    51,    60,    61,     1,
      35,    51,    55,    56,     7,    50,     1,     3,     3,     4,
       5,     7,     9,    11,    12,    13,    14,    15,    28,    29,
      33,    34,    36,    57,    58,    59,    60,    64,    53,    62,
      63,    33,    60,    35,    53,     1,    35,    16,    39,    34,
      34,    59,    34,    34,    64,    64,    64,     1,    64,    64,
      33,    60,    58,    33,    58,     1,    15,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    37,    38,    17,    34,     1,    15,    16,    55,
       1,    35,    64,    65,    64,    64,    61,     1,    15,    35,
       1,    35,    58,     1,    33,    60,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    28,    64,    29,
      64,    64,    64,     7,    64,     1,    64,    35,    64,    62,
      35,    16,     1,    35,    35,    35,    64,     1,     1,    39,
      35,    65,    59,    59,     1,    15,    11,    64,    59,     1,
      35,     1,    59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    49,    49,    50,
      50,    51,    51,    52,    52,    53,    53,    53,    53,    53,
      54,    54,    54,    54,    54,    55,    55,    56,    57,    57,
      57,    57,    57,    57,    57,    58,    58,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    63,
      63,    63,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     1,
       3,     1,     1,     5,     2,     1,     1,     4,     3,     2,
       4,     4,     4,     3,     3,     3,     1,     2,     2,     4,
       3,     3,     4,     5,     5,     2,     1,     1,     2,     2,
       1,     3,     3,     5,     7,     2,     5,     8,     8,     7,
       5,     2,     1,     3,     3,     1,     3,     1,     3,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     3,     2,     2,     3,     4,     4,     4,     4,
       3,     1,     1,     1,     1,     3,     1
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
#line 43 "src/parser/syntax.y"
                    {
        yyval = create_parent_node(AST_PROGRAM, (yyloc).first_line, 1, yyvsp[0]);
        if (!err_flag)
            print_ast(yyval);
      }
#line 1672 "syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 51 "src/parser/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1678 "syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 52 "src/parser/syntax.y"
      { yyval = NULL; }
#line 1684 "syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 56 "src/parser/syntax.y"
                                  { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1690 "syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 57 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1696 "syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FuncDec CompStmt  */
#line 58 "src/parser/syntax.y"
                                 { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1702 "syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier error  */
#line 59 "src/parser/syntax.y"
                      { splerror(SPLERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "missing valid identifier"); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1708 "syntax.tab.c"
    break;

  case 9: /* ExtDecList: VarDec  */
#line 63 "src/parser/syntax.y"
                   { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1714 "syntax.tab.c"
    break;

  case 10: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 64 "src/parser/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1720 "syntax.tab.c"
    break;

  case 11: /* Specifier: TYPE  */
#line 67 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1726 "syntax.tab.c"
    break;

  case 12: /* Specifier: StructSpecifier  */
#line 68 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1732 "syntax.tab.c"
    break;

  case 13: /* StructSpecifier: STRUCT ID LC DefList RC  */
#line 72 "src/parser/syntax.y"
                                         { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1738 "syntax.tab.c"
    break;

  case 14: /* StructSpecifier: STRUCT ID  */
#line 73 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1744 "syntax.tab.c"
    break;

  case 15: /* VarDec: ID  */
#line 77 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1750 "syntax.tab.c"
    break;

  case 16: /* VarDec: Specifier  */
#line 78 "src/parser/syntax.y"
                { splerror(SPLERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "two or more data types in declaration specifiers"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1756 "syntax.tab.c"
    break;

  case 17: /* VarDec: VarDec LSB INT RSB  */
#line 79 "src/parser/syntax.y"
                         { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1762 "syntax.tab.c"
    break;

  case 18: /* VarDec: VarDec LSB error  */
#line 80 "src/parser/syntax.y"
                       { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing bracket ']'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1768 "syntax.tab.c"
    break;

  case 19: /* VarDec: VarDec error  */
#line 81 "src/parser/syntax.y"
                   { splerror(SPLERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "unrecognized following content"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1774 "syntax.tab.c"
    break;

  case 20: /* FuncDec: ID LP VarList RP  */
#line 85 "src/parser/syntax.y"
                          { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1780 "syntax.tab.c"
    break;

  case 21: /* FuncDec: ID LP error RP  */
#line 86 "src/parser/syntax.y"
                     { splerror(SPLERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid parameter declaration ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1786 "syntax.tab.c"
    break;

  case 22: /* FuncDec: ID LP VarList error  */
#line 87 "src/parser/syntax.y"
                          { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1792 "syntax.tab.c"
    break;

  case 23: /* FuncDec: ID LP RP  */
#line 88 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1798 "syntax.tab.c"
    break;

  case 24: /* FuncDec: ID LP error  */
#line 89 "src/parser/syntax.y"
                  { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1804 "syntax.tab.c"
    break;

  case 25: /* VarList: ParamDec COMMA VarList  */
#line 93 "src/parser/syntax.y"
                                { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1810 "syntax.tab.c"
    break;

  case 26: /* VarList: ParamDec  */
#line 94 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1816 "syntax.tab.c"
    break;

  case 27: /* ParamDec: Specifier VarDec  */
#line 98 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_PARAM_DEC, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1822 "syntax.tab.c"
    break;

  case 28: /* CompStmt: LC RC  */
#line 102 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1828 "syntax.tab.c"
    break;

  case 29: /* CompStmt: LC DefList StmtList RC  */
#line 103 "src/parser/syntax.y"
                             { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1834 "syntax.tab.c"
    break;

  case 30: /* CompStmt: LC DefList RC  */
#line 104 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1840 "syntax.tab.c"
    break;

  case 31: /* CompStmt: LC StmtList RC  */
#line 105 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1846 "syntax.tab.c"
    break;

  case 32: /* CompStmt: LC DefList StmtList error  */
#line 106 "src/parser/syntax.y"
                                { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing brace '}'"); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1852 "syntax.tab.c"
    break;

  case 33: /* CompStmt: LC DefList StmtList DefList error  */
#line 107 "src/parser/syntax.y"
                                        { splerror(SPLERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-2]).last_line, (yylsp[-2]).last_column, "cannot interleave definitions with statements. "); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1858 "syntax.tab.c"
    break;

  case 34: /* CompStmt: LC StmtList DefList StmtList error  */
#line 108 "src/parser/syntax.y"
                                         { splerror(SPLERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[-2]).last_line, (yylsp[-2]).last_column, "cannot interleave definitions with statements. "); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1864 "syntax.tab.c"
    break;

  case 35: /* StmtList: Stmt StmtList  */
#line 112 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1870 "syntax.tab.c"
    break;

  case 36: /* StmtList: Stmt  */
#line 113 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1876 "syntax.tab.c"
    break;

  case 37: /* Stmt: SEMI  */
#line 117 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 1882 "syntax.tab.c"
    break;

  case 38: /* Stmt: Exp SEMI  */
#line 118 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1888 "syntax.tab.c"
    break;

  case 39: /* Stmt: Exp error  */
#line 119 "src/parser/syntax.y"
                { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1894 "syntax.tab.c"
    break;

  case 40: /* Stmt: CompStmt  */
#line 120 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 1900 "syntax.tab.c"
    break;

  case 41: /* Stmt: RETURN Exp SEMI  */
#line 121 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1906 "syntax.tab.c"
    break;

  case 42: /* Stmt: RETURN Exp error  */
#line 122 "src/parser/syntax.y"
                       { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 1912 "syntax.tab.c"
    break;

  case 43: /* Stmt: IF LP Exp RP Stmt  */
#line 123 "src/parser/syntax.y"
                                   { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1918 "syntax.tab.c"
    break;

  case 44: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 124 "src/parser/syntax.y"
                                  { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1924 "syntax.tab.c"
    break;

  case 45: /* Stmt: ELSE Stmt  */
#line 125 "src/parser/syntax.y"
                { splerror(SPLERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "hanging else is not allowed."); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); yyerrok; }
#line 1930 "syntax.tab.c"
    break;

  case 46: /* Stmt: WHILE LP Exp RP Stmt  */
#line 126 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1936 "syntax.tab.c"
    break;

  case 47: /* Stmt: FOR LP Def Exp SEMI Exp RP Stmt  */
#line 127 "src/parser/syntax.y"
                                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 8, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1942 "syntax.tab.c"
    break;

  case 48: /* Stmt: FOR LP Def Exp SEMI Exp RP error  */
#line 128 "src/parser/syntax.y"
                                       { splerror(SPLERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "for loop requires at one statement to be executed"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 1948 "syntax.tab.c"
    break;

  case 49: /* Stmt: FOR LP Def Exp SEMI Exp error  */
#line 129 "src/parser/syntax.y"
                                    { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing right parenthesis ')'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1954 "syntax.tab.c"
    break;

  case 50: /* Stmt: FOR LP Def Exp error  */
#line 130 "src/parser/syntax.y"
                           { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1960 "syntax.tab.c"
    break;

  case 51: /* DefList: Def DefList  */
#line 134 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1966 "syntax.tab.c"
    break;

  case 52: /* DefList: Def  */
#line 135 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1972 "syntax.tab.c"
    break;

  case 53: /* Def: Specifier DecList SEMI  */
#line 139 "src/parser/syntax.y"
                            { yyval = create_parent_node(AST_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1978 "syntax.tab.c"
    break;

  case 54: /* Def: Specifier DecList error  */
#line 140 "src/parser/syntax.y"
                              { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing semicolon ';'"); yyval = create_parent_node(AST_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1984 "syntax.tab.c"
    break;

  case 55: /* DecList: Dec  */
#line 144 "src/parser/syntax.y"
             { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1990 "syntax.tab.c"
    break;

  case 56: /* DecList: Dec COMMA DecList  */
#line 145 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1996 "syntax.tab.c"
    break;

  case 57: /* Dec: VarDec  */
#line 149 "src/parser/syntax.y"
            { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 2002 "syntax.tab.c"
    break;

  case 58: /* Dec: VarDec LP RP  */
#line 150 "src/parser/syntax.y"
                   { splerror(SPLERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "function definition not allowed here."); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2008 "syntax.tab.c"
    break;

  case 59: /* Dec: VarDec LP Exp RP  */
#line 151 "src/parser/syntax.y"
                       { splerror(SPLERR_B, (yylsp[-3]).first_line, (yylsp[-3]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "function definition not allowed here."); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2014 "syntax.tab.c"
    break;

  case 60: /* Dec: VarDec ASSIGN Exp  */
#line 152 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2020 "syntax.tab.c"
    break;

  case 61: /* Dec: VarDec ASSIGN error  */
#line 153 "src/parser/syntax.y"
                          { splerror(SPLERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, (yylsp[0]).last_line, (yylsp[0]).last_column, "invalid initialization"); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2026 "syntax.tab.c"
    break;

  case 62: /* Exp: Exp ASSIGN Exp  */
#line 157 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2032 "syntax.tab.c"
    break;

  case 63: /* Exp: Exp AND Exp  */
#line 159 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2038 "syntax.tab.c"
    break;

  case 64: /* Exp: Exp OR Exp  */
#line 160 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2044 "syntax.tab.c"
    break;

  case 65: /* Exp: Exp BITWISE_AND Exp  */
#line 161 "src/parser/syntax.y"
                          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2050 "syntax.tab.c"
    break;

  case 66: /* Exp: Exp BITWISE_OR Exp  */
#line 162 "src/parser/syntax.y"
                         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2056 "syntax.tab.c"
    break;

  case 67: /* Exp: Exp LT Exp  */
#line 164 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2062 "syntax.tab.c"
    break;

  case 68: /* Exp: Exp LE Exp  */
#line 165 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2068 "syntax.tab.c"
    break;

  case 69: /* Exp: Exp GT Exp  */
#line 166 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2074 "syntax.tab.c"
    break;

  case 70: /* Exp: Exp GE Exp  */
#line 167 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2080 "syntax.tab.c"
    break;

  case 71: /* Exp: Exp NE Exp  */
#line 168 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2086 "syntax.tab.c"
    break;

  case 72: /* Exp: Exp EQ Exp  */
#line 169 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2092 "syntax.tab.c"
    break;

  case 73: /* Exp: Exp PLUS Exp  */
#line 171 "src/parser/syntax.y"
                   { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2098 "syntax.tab.c"
    break;

  case 74: /* Exp: Exp MINUS Exp  */
#line 172 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2104 "syntax.tab.c"
    break;

  case 75: /* Exp: Exp MUL Exp  */
#line 173 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2110 "syntax.tab.c"
    break;

  case 76: /* Exp: Exp DIV Exp  */
#line 174 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2116 "syntax.tab.c"
    break;

  case 77: /* Exp: LP Exp RP  */
#line 176 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2122 "syntax.tab.c"
    break;

  case 78: /* Exp: LP Exp error  */
#line 177 "src/parser/syntax.y"
                   { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing closing parenthesis ')'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2128 "syntax.tab.c"
    break;

  case 79: /* Exp: LP error RP  */
#line 178 "src/parser/syntax.y"
                  { splerror(SPLERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid expression: "); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2134 "syntax.tab.c"
    break;

  case 80: /* Exp: Exp PLUS PLUS  */
#line 180 "src/parser/syntax.y"
                                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2140 "syntax.tab.c"
    break;

  case 81: /* Exp: PLUS Exp  */
#line 182 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2146 "syntax.tab.c"
    break;

  case 82: /* Exp: Exp MINUS MINUS  */
#line 183 "src/parser/syntax.y"
                                       { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2152 "syntax.tab.c"
    break;

  case 83: /* Exp: MINUS Exp  */
#line 185 "src/parser/syntax.y"
                             { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2158 "syntax.tab.c"
    break;

  case 84: /* Exp: NOT Exp  */
#line 186 "src/parser/syntax.y"
              { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2164 "syntax.tab.c"
    break;

  case 85: /* Exp: ID LP RP  */
#line 187 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2170 "syntax.tab.c"
    break;

  case 86: /* Exp: ID LP Args RP  */
#line 188 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2176 "syntax.tab.c"
    break;

  case 87: /* Exp: ID LP Args error  */
#line 189 "src/parser/syntax.y"
                       { splerror(SPLERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "missing right parenthesis ')'"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2182 "syntax.tab.c"
    break;

  case 88: /* Exp: ID LP error RP  */
#line 190 "src/parser/syntax.y"
                     { splerror(SPLERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "invalid argument list"); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2188 "syntax.tab.c"
    break;

  case 89: /* Exp: Exp LSB Exp RSB  */
#line 192 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2194 "syntax.tab.c"
    break;

  case 90: /* Exp: Exp DOT ID  */
#line 193 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2200 "syntax.tab.c"
    break;

  case 91: /* Exp: ID  */
#line 195 "src/parser/syntax.y"
         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2206 "syntax.tab.c"
    break;

  case 92: /* Exp: INT  */
#line 196 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2212 "syntax.tab.c"
    break;

  case 93: /* Exp: FLOAT  */
#line 197 "src/parser/syntax.y"
            { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2218 "syntax.tab.c"
    break;

  case 94: /* Exp: CHAR  */
#line 198 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2224 "syntax.tab.c"
    break;

  case 95: /* Args: Exp COMMA Args  */
#line 202 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2230 "syntax.tab.c"
    break;

  case 96: /* Args: Exp  */
#line 203 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 1, yyvsp[0]); }
#line 2236 "syntax.tab.c"
    break;


#line 2240 "syntax.tab.c"

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

#line 206 "src/parser/syntax.y"


void yyerror(const char *s) {
    // suppressed
    fprintf(stderr, "%s at line %d\n", s, yylloc.first_line);
}
