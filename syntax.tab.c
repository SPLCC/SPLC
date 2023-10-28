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
#define YYLAST   637

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  171

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
      64,    67,    68,    72,    73,    77,    78,    79,    84,    85,
      86,    87,    88,    92,    93,    97,   101,   102,   103,   104,
     105,   106,   107,   111,   112,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   133,
     134,   138,   139,   143,   144,   148,   149,   150,   151,   152,
     156,   158,   159,   160,   161,   163,   164,   165,   166,   167,
     168,   170,   171,   172,   173,   175,   176,   177,   179,   181,
     182,   184,   185,   186,   187,   188,   189,   191,   192,   194,
     195,   196,   197,   201,   202
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

#define YYPACT_NINF (-44)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      27,    15,   -44,    26,   -44,    27,   325,   -44,   -16,   -44,
     -44,   -44,    -5,   -44,    17,   -44,     2,    29,    27,   118,
     -44,    13,    38,   309,   -44,    13,    36,    27,    28,   -44,
      13,     1,    60,   -44,   -44,    41,   -44,   -44,   -44,    48,
      51,   377,    54,    65,   266,   -44,   266,   266,   -44,    74,
     266,   -44,     5,   377,   343,   113,    53,    16,    85,   -44,
     -44,   -44,    66,   -44,   -44,    27,   -44,    61,   266,   -44,
     266,    27,   144,    35,    35,    70,   175,    35,   -44,   377,
     -44,   -44,    92,   -44,   -44,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   390,   418,   266,   266,
     100,   266,   151,   211,   -44,   -44,    13,   -44,    78,   -44,
     411,    22,   461,   483,   266,   -44,   -44,   -44,   -44,   -44,
     115,   -44,   -44,   116,   527,   599,   582,   565,   548,   331,
     331,   331,   331,   331,   331,   294,   153,   294,   153,    35,
      35,   -44,   438,   -44,   527,   -44,   505,   -44,   -44,   266,
     -44,   -44,   377,   377,   206,   -44,   -44,   -44,   -44,   -44,
     107,   -44,   -44,   266,   377,   237,   -44,   -44,   275,   -44,
     -44
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,    11,     0,     2,     4,     0,    12,    14,     1,
       3,     8,    15,     6,     0,    16,     9,     0,     0,     0,
       5,     0,     0,     0,     7,     0,     0,    50,    22,    21,
       0,     0,    24,    15,    10,     0,    90,    91,    92,    89,
       0,     0,     0,     0,     0,    35,     0,     0,    26,     0,
       0,    38,     0,    34,     0,     0,    55,     0,    53,    13,
      49,    19,    25,    20,    18,     0,    17,     0,     0,    43,
       0,     0,     0,    79,    81,     0,     0,    82,    29,     0,
      33,    28,     0,    37,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    51,     0,    23,     0,    83,
      94,     0,     0,     0,     0,    40,    39,    77,    76,    75,
       0,    30,    27,     0,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    78,    71,    80,    72,    73,
      74,    88,     0,    59,    58,    56,     0,    54,    86,     0,
      85,    84,     0,     0,     0,    32,    31,    87,    57,    93,
      41,    44,    48,     0,     0,     0,    42,    47,     0,    46,
      45
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -44,   -44,   117,   -44,   108,     9,   -44,   -20,   -44,    56,
     -44,   129,    30,   -41,   -15,    76,    42,   -44,   -43,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,    14,    25,     7,    16,    17,    31,
      32,    51,    52,    53,    26,    27,    57,    58,    55,   111
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      69,    72,    63,    73,    74,    56,    76,    77,    54,     6,
      62,     1,    60,     2,     6,    15,    18,   104,    21,     1,
      33,     2,     8,   150,   110,   112,     9,   113,    30,    19,
      15,   105,    20,     1,    15,     2,    64,    79,    78,    15,
      22,    35,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   136,   138,   139,   140,   151,   142,   144,
     146,    23,   108,    61,    36,    37,    38,   123,    39,    59,
     102,   154,   100,   101,    30,    75,    65,    36,    37,    38,
      66,    39,    67,    80,    82,    68,    56,   103,    70,    46,
      47,    22,    73,   121,    74,    49,   109,    50,     1,    71,
       2,   106,    46,    47,    22,   117,   110,   141,    49,   120,
      50,   160,   161,   148,    83,    15,   155,   156,   164,    28,
     165,   107,    10,   166,     1,   122,     2,   170,    84,    34,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   115,    24,   114,   147,   159,
     100,   101,   143,    29,    36,    37,    38,     0,    39,   116,
       0,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   118,     0,     0,    46,
      47,   100,   101,    98,    99,    49,     0,    50,     0,     0,
     100,   101,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   162,     0,     0,
     119,     0,   100,   101,    36,    37,    38,     0,    39,     0,
       0,   163,     0,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   167,    46,
      47,     0,     0,   100,   101,    49,   145,    50,     0,     0,
       0,     0,     0,     0,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,    36,
      37,    38,   168,    39,   100,   101,   169,     0,    36,    37,
      38,     0,    39,     0,    40,     0,    41,    42,    43,    44,
      45,     0,     0,     0,    46,    47,     0,    36,    37,    38,
      49,    39,    50,    46,    47,     0,     0,    23,     0,    49,
       0,    50,    36,    37,    38,     1,    39,     2,    40,     0,
      41,    42,    43,    44,    45,     0,    11,     0,    49,     0,
      50,     1,    12,     2,     0,     0,     0,    46,    47,     0,
      13,    23,    48,    49,     0,    50,    36,    37,    38,     0,
      39,     0,    40,     0,    41,    42,    43,    44,    45,    96,
      97,    98,    99,     0,     0,     0,     0,     0,   100,   101,
       0,    46,    47,     0,     0,    23,    81,    49,     0,    50,
      36,    37,    38,     0,    39,     0,    40,     0,    41,    42,
      43,    44,    45,    36,    37,    38,     0,    39,     0,     0,
       0,     0,     0,     0,     0,    46,    47,     0,     0,    23,
       0,    49,     0,    50,     0,     0,     0,     0,   135,    47,
       0,    36,    37,    38,    49,    39,    50,   149,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,     0,     0,     0,    46,   137,   100,   101,
       0,     0,    49,     0,    50,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
       0,     0,     0,     0,     0,   100,   101,   157,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,     0,     0,     0,   152,     0,   100,   101,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,     0,     0,     0,   153,     0,
     100,   101,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,     0,     0,     0,
     158,     0,   100,   101,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,     0,   100,   101,    86,    87,    88,     0,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
       0,     0,     0,    86,    87,   100,   101,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,     0,     0,     0,
      86,     0,   100,   101,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,     0,     0,     0,     0,     0,   100,
     101,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,     0,     0,     0,     0,     0,   100,   101
};

static const yytype_int16 yycheck[] =
{
      41,    44,     1,    46,    47,    25,    49,    50,    23,     0,
      30,     6,    27,     8,     5,     6,    32,     1,    16,     6,
       7,     8,     7,     1,    67,    68,     0,    70,    19,    34,
      21,    15,    15,     6,    25,     8,    35,    52,    33,    30,
      38,     3,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,    35,   101,   102,
     103,    32,     1,    35,     3,     4,     5,    82,     7,    33,
      17,   114,    37,    38,    65,     1,    16,     3,     4,     5,
      39,     7,    34,    53,    54,    34,   106,    34,    34,    28,
      29,    38,   135,     1,   137,    34,    35,    36,     6,    34,
       8,    16,    28,    29,    38,    35,   149,     7,    34,    79,
      36,   152,   153,    35,     1,   106,     1,     1,    11,     1,
     163,    65,     5,   164,     6,    33,     8,   168,    15,    21,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,     1,    17,    71,   106,   149,
      37,    38,     1,    35,     3,     4,     5,    -1,     7,    15,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,     1,    -1,    -1,    28,
      29,    37,    38,    30,    31,    34,    -1,    36,    -1,    -1,
      37,    38,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     1,    -1,    -1,
      35,    -1,    37,    38,     3,     4,     5,    -1,     7,    -1,
      -1,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     1,    28,
      29,    -1,    -1,    37,    38,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     3,
       4,     5,    35,     7,    37,    38,     1,    -1,     3,     4,
       5,    -1,     7,    -1,     9,    -1,    11,    12,    13,    14,
      15,    -1,    -1,    -1,    28,    29,    -1,     3,     4,     5,
      34,     7,    36,    28,    29,    -1,    -1,    32,    -1,    34,
      -1,    36,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    -1,     1,    -1,    34,    -1,
      36,     6,     7,     8,    -1,    -1,    -1,    28,    29,    -1,
      15,    32,    33,    34,    -1,    36,     3,     4,     5,    -1,
       7,    -1,     9,    -1,    11,    12,    13,    14,    15,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,    38,
      -1,    28,    29,    -1,    -1,    32,    33,    34,    -1,    36,
       3,     4,     5,    -1,     7,    -1,     9,    -1,    11,    12,
      13,    14,    15,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,    32,
      -1,    34,    -1,    36,    -1,    -1,    -1,    -1,    28,    29,
      -1,     3,     4,     5,    34,     7,    36,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    28,    29,    37,    38,
      -1,    -1,    34,    -1,    36,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,    -1,    37,    38,    39,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    35,    -1,    37,    38,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    35,    -1,
      37,    38,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      35,    -1,    37,    38,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    37,    38,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    18,    19,    37,    38,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
      18,    -1,    37,    38,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    37,
      38,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    37,    38
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     8,    47,    48,    49,    51,    52,     7,     0,
      48,     1,     7,    15,    50,    51,    53,    54,    32,    34,
      15,    16,    38,    32,    57,    51,    60,    61,     1,    35,
      51,    55,    56,     7,    50,     3,     3,     4,     5,     7,
       9,    11,    12,    13,    14,    15,    28,    29,    33,    34,
      36,    57,    58,    59,    60,    64,    53,    62,    63,    33,
      60,    35,    53,     1,    35,    16,    39,    34,    34,    59,
      34,    34,    64,    64,    64,     1,    64,    64,    33,    60,
      58,    33,    58,     1,    15,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      37,    38,    17,    34,     1,    15,    16,    55,     1,    35,
      64,    65,    64,    64,    61,     1,    15,    35,     1,    35,
      58,     1,    33,    60,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    28,    64,    29,    64,    64,
      64,     7,    64,     1,    64,    35,    64,    62,    35,    16,
       1,    35,    35,    35,    64,     1,     1,    39,    35,    65,
      59,    59,     1,    15,    11,    64,    59,     1,    35,     1,
      59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    49,    49,    50,
      50,    51,    51,    52,    52,    53,    53,    53,    54,    54,
      54,    54,    54,    55,    55,    56,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    60,
      60,    61,    61,    62,    62,    63,    63,    63,    63,    63,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    65,    65
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     1,
       3,     1,     1,     5,     2,     1,     1,     4,     4,     4,
       4,     3,     3,     3,     1,     2,     2,     4,     3,     3,
       4,     5,     5,     2,     1,     1,     2,     2,     1,     3,
       3,     5,     7,     2,     5,     8,     8,     7,     5,     2,
       1,     3,     3,     1,     3,     1,     3,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     2,     2,     3,     4,     4,     4,     4,     3,     1,
       1,     1,     1,     3,     1
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
#line 1664 "syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 51 "src/parser/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1670 "syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 52 "src/parser/syntax.y"
      { yyval = NULL; }
#line 1676 "syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 56 "src/parser/syntax.y"
                                  { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1682 "syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 57 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1688 "syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FuncDec CompStmt  */
#line 58 "src/parser/syntax.y"
                                 { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1694 "syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier error  */
#line 59 "src/parser/syntax.y"
                      { splerror(ERR_B, (yylsp[0]).last_line, (yylsp[0]).last_column, "Missing valid identifier", NULL); yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1700 "syntax.tab.c"
    break;

  case 9: /* ExtDecList: VarDec  */
#line 63 "src/parser/syntax.y"
                   { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1706 "syntax.tab.c"
    break;

  case 10: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 64 "src/parser/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1712 "syntax.tab.c"
    break;

  case 11: /* Specifier: TYPE  */
#line 67 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1718 "syntax.tab.c"
    break;

  case 12: /* Specifier: StructSpecifier  */
#line 68 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1724 "syntax.tab.c"
    break;

  case 13: /* StructSpecifier: STRUCT ID LC DefList RC  */
#line 72 "src/parser/syntax.y"
                                         { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1730 "syntax.tab.c"
    break;

  case 14: /* StructSpecifier: STRUCT ID  */
#line 73 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1736 "syntax.tab.c"
    break;

  case 15: /* VarDec: ID  */
#line 77 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1742 "syntax.tab.c"
    break;

  case 16: /* VarDec: Specifier  */
#line 78 "src/parser/syntax.y"
                {splerror(ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, "Two or more data types in declaration specifiers", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1748 "syntax.tab.c"
    break;

  case 17: /* VarDec: VarDec LSB INT RSB  */
#line 79 "src/parser/syntax.y"
                         { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1754 "syntax.tab.c"
    break;

  case 18: /* FuncDec: ID LP VarList RP  */
#line 84 "src/parser/syntax.y"
                          { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1760 "syntax.tab.c"
    break;

  case 19: /* FuncDec: ID LP error RP  */
#line 85 "src/parser/syntax.y"
                     { splerror(ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "Invalid parameter declaration ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1766 "syntax.tab.c"
    break;

  case 20: /* FuncDec: ID LP VarList error  */
#line 86 "src/parser/syntax.y"
                          { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1772 "syntax.tab.c"
    break;

  case 21: /* FuncDec: ID LP RP  */
#line 87 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1778 "syntax.tab.c"
    break;

  case 22: /* FuncDec: ID LP error  */
#line 88 "src/parser/syntax.y"
                  { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1784 "syntax.tab.c"
    break;

  case 23: /* VarList: ParamDec COMMA VarList  */
#line 92 "src/parser/syntax.y"
                                { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1790 "syntax.tab.c"
    break;

  case 24: /* VarList: ParamDec  */
#line 93 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1796 "syntax.tab.c"
    break;

  case 25: /* ParamDec: Specifier VarDec  */
#line 97 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_PARAM_DEC, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1802 "syntax.tab.c"
    break;

  case 26: /* CompStmt: LC RC  */
#line 101 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1808 "syntax.tab.c"
    break;

  case 27: /* CompStmt: LC DefList StmtList RC  */
#line 102 "src/parser/syntax.y"
                             { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1814 "syntax.tab.c"
    break;

  case 28: /* CompStmt: LC DefList RC  */
#line 103 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1820 "syntax.tab.c"
    break;

  case 29: /* CompStmt: LC StmtList RC  */
#line 104 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1826 "syntax.tab.c"
    break;

  case 30: /* CompStmt: LC DefList StmtList error  */
#line 105 "src/parser/syntax.y"
                                { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing closing brace '}'", NULL); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1832 "syntax.tab.c"
    break;

  case 31: /* CompStmt: LC DefList StmtList DefList error  */
#line 106 "src/parser/syntax.y"
                                        { splerror(ERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, "Cannot interleave definitions with statements. ", NULL); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1838 "syntax.tab.c"
    break;

  case 32: /* CompStmt: LC StmtList DefList StmtList error  */
#line 107 "src/parser/syntax.y"
                                         { splerror(ERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, "Cannot interleave definitions with statements. ", NULL); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1844 "syntax.tab.c"
    break;

  case 33: /* StmtList: Stmt StmtList  */
#line 111 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1850 "syntax.tab.c"
    break;

  case 34: /* StmtList: Stmt  */
#line 112 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1856 "syntax.tab.c"
    break;

  case 35: /* Stmt: SEMI  */
#line 116 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 1862 "syntax.tab.c"
    break;

  case 36: /* Stmt: Exp SEMI  */
#line 117 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1868 "syntax.tab.c"
    break;

  case 37: /* Stmt: Exp error  */
#line 118 "src/parser/syntax.y"
                { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1874 "syntax.tab.c"
    break;

  case 38: /* Stmt: CompStmt  */
#line 119 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 1880 "syntax.tab.c"
    break;

  case 39: /* Stmt: RETURN Exp SEMI  */
#line 120 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1886 "syntax.tab.c"
    break;

  case 40: /* Stmt: RETURN Exp error  */
#line 121 "src/parser/syntax.y"
                       { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 1892 "syntax.tab.c"
    break;

  case 41: /* Stmt: IF LP Exp RP Stmt  */
#line 122 "src/parser/syntax.y"
                                   { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1898 "syntax.tab.c"
    break;

  case 42: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 123 "src/parser/syntax.y"
                                  { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1904 "syntax.tab.c"
    break;

  case 43: /* Stmt: ELSE Stmt  */
#line 124 "src/parser/syntax.y"
                { splerror(ERR_B, (yylsp[-1]).first_line, (yylsp[0]).first_column, "Hanging else is not allowed.", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); yyerrok; }
#line 1910 "syntax.tab.c"
    break;

  case 44: /* Stmt: WHILE LP Exp RP Stmt  */
#line 125 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1916 "syntax.tab.c"
    break;

  case 45: /* Stmt: FOR LP Def Exp SEMI Exp RP Stmt  */
#line 126 "src/parser/syntax.y"
                                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]); }
#line 1922 "syntax.tab.c"
    break;

  case 46: /* Stmt: FOR LP Def Exp SEMI Exp RP error  */
#line 127 "src/parser/syntax.y"
                                       { splerror(ERR_B, (yylsp[0]).first_line, (yylsp[-6]).first_column, "for loop requires at one statement to be executed", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-7], yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 1928 "syntax.tab.c"
    break;

  case 47: /* Stmt: FOR LP Def Exp SEMI Exp error  */
#line 128 "src/parser/syntax.y"
                                    { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing right parenthesis ')'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1934 "syntax.tab.c"
    break;

  case 48: /* Stmt: FOR LP Def Exp error  */
#line 129 "src/parser/syntax.y"
                           { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1940 "syntax.tab.c"
    break;

  case 49: /* DefList: Def DefList  */
#line 133 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1946 "syntax.tab.c"
    break;

  case 50: /* DefList: Def  */
#line 134 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1952 "syntax.tab.c"
    break;

  case 51: /* Def: Specifier DecList SEMI  */
#line 138 "src/parser/syntax.y"
                            { yyval = create_parent_node(AST_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1958 "syntax.tab.c"
    break;

  case 52: /* Def: Specifier DecList error  */
#line 139 "src/parser/syntax.y"
                              { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1964 "syntax.tab.c"
    break;

  case 53: /* DecList: Dec  */
#line 143 "src/parser/syntax.y"
             { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1970 "syntax.tab.c"
    break;

  case 54: /* DecList: Dec COMMA DecList  */
#line 144 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1976 "syntax.tab.c"
    break;

  case 55: /* Dec: VarDec  */
#line 148 "src/parser/syntax.y"
            { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1982 "syntax.tab.c"
    break;

  case 56: /* Dec: VarDec LP RP  */
#line 149 "src/parser/syntax.y"
                   { splerror(ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "Function definition not allowed here.", NULL); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1988 "syntax.tab.c"
    break;

  case 57: /* Dec: VarDec LP Exp RP  */
#line 150 "src/parser/syntax.y"
                       { splerror(ERR_B, (yylsp[-2]).first_line, (yylsp[-2]).first_column, "Function definition not allowed here.", NULL); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1994 "syntax.tab.c"
    break;

  case 58: /* Dec: VarDec ASSIGN Exp  */
#line 151 "src/parser/syntax.y"
                        { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2000 "syntax.tab.c"
    break;

  case 59: /* Dec: VarDec ASSIGN error  */
#line 152 "src/parser/syntax.y"
                          { splerror(ERR_B, (yylsp[0]).first_line, (yylsp[0]).first_column, "Invalid initialization", NULL); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 2006 "syntax.tab.c"
    break;

  case 60: /* Exp: Exp ASSIGN Exp  */
#line 156 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2012 "syntax.tab.c"
    break;

  case 61: /* Exp: Exp AND Exp  */
#line 158 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2018 "syntax.tab.c"
    break;

  case 62: /* Exp: Exp OR Exp  */
#line 159 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2024 "syntax.tab.c"
    break;

  case 63: /* Exp: Exp BITWISE_AND Exp  */
#line 160 "src/parser/syntax.y"
                          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2030 "syntax.tab.c"
    break;

  case 64: /* Exp: Exp BITWISE_OR Exp  */
#line 161 "src/parser/syntax.y"
                         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2036 "syntax.tab.c"
    break;

  case 65: /* Exp: Exp LT Exp  */
#line 163 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2042 "syntax.tab.c"
    break;

  case 66: /* Exp: Exp LE Exp  */
#line 164 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2048 "syntax.tab.c"
    break;

  case 67: /* Exp: Exp GT Exp  */
#line 165 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2054 "syntax.tab.c"
    break;

  case 68: /* Exp: Exp GE Exp  */
#line 166 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2060 "syntax.tab.c"
    break;

  case 69: /* Exp: Exp NE Exp  */
#line 167 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2066 "syntax.tab.c"
    break;

  case 70: /* Exp: Exp EQ Exp  */
#line 168 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2072 "syntax.tab.c"
    break;

  case 71: /* Exp: Exp PLUS Exp  */
#line 170 "src/parser/syntax.y"
                   { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2078 "syntax.tab.c"
    break;

  case 72: /* Exp: Exp MINUS Exp  */
#line 171 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2084 "syntax.tab.c"
    break;

  case 73: /* Exp: Exp MUL Exp  */
#line 172 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2090 "syntax.tab.c"
    break;

  case 74: /* Exp: Exp DIV Exp  */
#line 173 "src/parser/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2096 "syntax.tab.c"
    break;

  case 75: /* Exp: LP Exp RP  */
#line 175 "src/parser/syntax.y"
                { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2102 "syntax.tab.c"
    break;

  case 76: /* Exp: LP Exp error  */
#line 176 "src/parser/syntax.y"
                   { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2108 "syntax.tab.c"
    break;

  case 77: /* Exp: LP error RP  */
#line 177 "src/parser/syntax.y"
                  { splerror(ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "Invalid expression: ", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2114 "syntax.tab.c"
    break;

  case 78: /* Exp: Exp PLUS PLUS  */
#line 179 "src/parser/syntax.y"
                                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2120 "syntax.tab.c"
    break;

  case 79: /* Exp: PLUS Exp  */
#line 181 "src/parser/syntax.y"
                           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2126 "syntax.tab.c"
    break;

  case 80: /* Exp: Exp MINUS MINUS  */
#line 182 "src/parser/syntax.y"
                                       { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2132 "syntax.tab.c"
    break;

  case 81: /* Exp: MINUS Exp  */
#line 184 "src/parser/syntax.y"
                             { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2138 "syntax.tab.c"
    break;

  case 82: /* Exp: NOT Exp  */
#line 185 "src/parser/syntax.y"
              { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 2144 "syntax.tab.c"
    break;

  case 83: /* Exp: ID LP RP  */
#line 186 "src/parser/syntax.y"
               { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2150 "syntax.tab.c"
    break;

  case 84: /* Exp: ID LP Args RP  */
#line 187 "src/parser/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2156 "syntax.tab.c"
    break;

  case 85: /* Exp: ID LP Args error  */
#line 188 "src/parser/syntax.y"
                       { splerror(ERR_B, (yylsp[-1]).last_line, (yylsp[-1]).last_column, "Missing right parenthesis ')'", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2162 "syntax.tab.c"
    break;

  case 86: /* Exp: ID LP error RP  */
#line 189 "src/parser/syntax.y"
                     { splerror(ERR_B, (yylsp[-1]).first_line, (yylsp[-1]).first_column, "Invalid argument: ", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2168 "syntax.tab.c"
    break;

  case 87: /* Exp: Exp LSB Exp RSB  */
#line 191 "src/parser/syntax.y"
                      { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2174 "syntax.tab.c"
    break;

  case 88: /* Exp: Exp DOT ID  */
#line 192 "src/parser/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2180 "syntax.tab.c"
    break;

  case 89: /* Exp: ID  */
#line 194 "src/parser/syntax.y"
         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2186 "syntax.tab.c"
    break;

  case 90: /* Exp: INT  */
#line 195 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2192 "syntax.tab.c"
    break;

  case 91: /* Exp: FLOAT  */
#line 196 "src/parser/syntax.y"
            { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2198 "syntax.tab.c"
    break;

  case 92: /* Exp: CHAR  */
#line 197 "src/parser/syntax.y"
           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2204 "syntax.tab.c"
    break;

  case 93: /* Args: Exp COMMA Args  */
#line 201 "src/parser/syntax.y"
                     { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2210 "syntax.tab.c"
    break;

  case 94: /* Args: Exp  */
#line 202 "src/parser/syntax.y"
          { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 1, yyvsp[0]); }
#line 2216 "syntax.tab.c"
    break;


#line 2220 "syntax.tab.c"

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

#line 205 "src/parser/syntax.y"


void yyerror(const char *s) {
    // suppressed
    /* fprintf(stderr, "%s at line %d\n", s, yylloc.first_line); */
}
