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
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_SEMI = 14,                      /* SEMI  */
  YYSYMBOL_COMMA = 15,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 16,                    /* ASSIGN  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_BITWISE_AND = 19,               /* BITWISE_AND  */
  YYSYMBOL_BITWISE_OR = 20,                /* BITWISE_OR  */
  YYSYMBOL_LT = 21,                        /* LT  */
  YYSYMBOL_LE = 22,                        /* LE  */
  YYSYMBOL_GT = 23,                        /* GT  */
  YYSYMBOL_GE = 24,                        /* GE  */
  YYSYMBOL_NE = 25,                        /* NE  */
  YYSYMBOL_EQ = 26,                        /* EQ  */
  YYSYMBOL_PLUS = 27,                      /* PLUS  */
  YYSYMBOL_MINUS = 28,                     /* MINUS  */
  YYSYMBOL_MUL = 29,                       /* MUL  */
  YYSYMBOL_DIV = 30,                       /* DIV  */
  YYSYMBOL_LC = 31,                        /* LC  */
  YYSYMBOL_RC = 32,                        /* RC  */
  YYSYMBOL_LP = 33,                        /* LP  */
  YYSYMBOL_RP = 34,                        /* RP  */
  YYSYMBOL_NOT = 35,                       /* NOT  */
  YYSYMBOL_DOT = 36,                       /* DOT  */
  YYSYMBOL_LSB = 37,                       /* LSB  */
  YYSYMBOL_RSB = 38,                       /* RSB  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_Program = 40,                   /* Program  */
  YYSYMBOL_ExtDefList = 41,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 42,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 43,                /* ExtDecList  */
  YYSYMBOL_Specifier = 44,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 45,           /* StructSpecifier  */
  YYSYMBOL_VarDec = 46,                    /* VarDec  */
  YYSYMBOL_FuncDec = 47,                   /* FuncDec  */
  YYSYMBOL_VarList = 48,                   /* VarList  */
  YYSYMBOL_ParamDec = 49,                  /* ParamDec  */
  YYSYMBOL_CompStmt = 50,                  /* CompStmt  */
  YYSYMBOL_StmtList = 51,                  /* StmtList  */
  YYSYMBOL_Stmt = 52,                      /* Stmt  */
  YYSYMBOL_DefList = 53,                   /* DefList  */
  YYSYMBOL_Def = 54,                       /* Def  */
  YYSYMBOL_DecList = 55,                   /* DecList  */
  YYSYMBOL_Dec = 56,                       /* Dec  */
  YYSYMBOL_Exp = 57,                       /* Exp  */
  YYSYMBOL_Args = 58                       /* Args  */
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
#define YYLAST   432

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


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
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    42,    42,    50,    51,    55,    56,    57,    61,    62,
      65,    66,    70,    71,    75,    76,    81,    82,    83,    84,
      85,    89,    90,    94,    98,    99,   100,   101,   102,   106,
     107,   111,   112,   113,   114,   115,   116,   117,   118,   122,
     123,   127,   128,   132,   133,   137,   138,   139,   143,   145,
     146,   147,   148,   150,   151,   152,   153,   154,   155,   157,
     158,   159,   160,   162,   163,   164,   166,   167,   168,   169,
     170,   171,   173,   174,   176,   177,   178,   179,   183,   184
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
  "STRUCT", "ID", "TYPE", "IF", "THEN", "ELSE", "WHILE", "RETURN", "SEMI",
  "COMMA", "ASSIGN", "AND", "OR", "BITWISE_AND", "BITWISE_OR", "LT", "LE",
  "GT", "GE", "NE", "EQ", "PLUS", "MINUS", "MUL", "DIV", "LC", "RC", "LP",
  "RP", "NOT", "DOT", "LSB", "RSB", "$accept", "Program", "ExtDefList",
  "ExtDef", "ExtDecList", "Specifier", "StructSpecifier", "VarDec",
  "FuncDec", "VarList", "ParamDec", "CompStmt", "StmtList", "Stmt",
  "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-118)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,    12,  -118,    33,  -118,     7,     0,  -118,    22,  -118,
    -118,    27,  -118,    51,    -5,    31,     7,   158,  -118,    64,
      70,   165,  -118,    64,    69,     7,    68,  -118,    64,    15,
      88,  -118,  -118,    66,  -118,  -118,  -118,    73,    90,    92,
     198,   198,    24,   198,  -118,    94,   209,   176,    60,   -11,
      53,   109,  -118,  -118,  -118,    93,  -118,  -118,     7,  -118,
      65,   198,   198,    91,   199,   100,   121,    19,  -118,  -118,
    -118,   159,  -118,  -118,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   129,
     198,   128,  -118,  -118,    64,  -118,   118,  -118,   230,    29,
     275,   297,  -118,  -118,  -118,  -118,  -118,  -118,  -118,   122,
     319,   391,   374,   357,   340,   395,   395,   395,   395,   395,
     395,   199,   199,    19,    19,  -118,   252,  -118,   319,  -118,
    -118,   198,  -118,  -118,   209,   209,  -118,  -118,  -118,   142,
    -118,   209,  -118
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,     0,    10,     0,     2,     4,     0,    11,    13,     1,
       3,    14,     6,     0,     8,     0,     0,     0,     5,     0,
       0,     0,     7,     0,     0,    40,    20,    19,     0,     0,
      22,    14,     9,     0,    75,    76,    77,    74,     0,     0,
       0,     0,     0,     0,    33,     0,    30,     0,     0,    45,
       0,    43,    12,    39,    17,    23,    18,    16,     0,    15,
       0,     0,     0,     0,    66,     0,     0,    67,    26,    29,
      25,     0,    32,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42,    41,     0,    21,     0,    68,    79,     0,
       0,     0,    35,    34,    65,    64,    63,    27,    24,     0,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    73,     0,    47,    46,    44,
      71,     0,    70,    69,     0,     0,    28,    72,    78,    36,
      38,     0,    37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -118,  -118,   157,  -118,   156,     6,  -118,   -19,  -118,   124,
    -118,   161,    48,  -117,   -13,  -118,    96,  -118,   -40,    55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,    13,    23,     7,    14,    15,    29,
      30,    44,    45,    46,    24,    25,    50,    51,    48,    99
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      63,    64,    66,    67,    49,    91,     6,    11,    47,    55,
      19,     6,    53,     1,    12,     2,    56,   139,   140,     8,
      98,   100,   101,    28,   142,    65,    20,    34,    35,    36,
     132,    37,    20,     9,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,    57,
     126,   128,    41,    16,    92,    89,    90,    42,   109,    43,
      17,    72,    21,   133,    28,    18,    96,    93,    34,    35,
      36,    31,    37,    33,    73,    49,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    98,   102,    41,    69,    71,    89,    90,    42,    97,
      43,    52,    54,    58,    59,   103,    60,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,   105,    61,    94,    62,    68,    89,    90,   127,
      20,    34,    35,    36,   104,    37,   125,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,   130,   141,   136,   106,    41,    89,    90,    26,
     107,    42,    10,    43,     1,     1,     2,     2,    34,    35,
      36,     1,    37,     2,    38,    32,    22,    39,    40,    34,
      35,    36,    95,    37,     0,    38,   138,     0,    39,    40,
     129,   108,    27,    41,     0,     0,    21,     0,    42,     0,
      43,    34,    35,    36,    41,    37,     0,    21,    70,    42,
       0,    43,    34,    35,    36,     0,    37,     0,    38,     0,
       0,    39,    40,     0,     0,     0,    41,     0,    87,    88,
       0,    42,     0,    43,     0,    89,    90,    41,     0,     0,
      21,     0,    42,     0,    43,   131,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,     0,     0,     0,    89,    90,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,     0,     0,     0,     0,     0,    89,    90,
     137,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,     0,     0,     0,   134,
       0,    89,    90,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,     0,
       0,   135,     0,    89,    90,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
       0,     0,     0,     0,     0,    89,    90,    75,    76,    77,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,     0,     0,     0,    75,    76,    89,    90,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,     0,     0,
       0,    75,     0,    89,    90,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,     0,     0,     0,     0,     0,
      89,    90,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    85,    86,    87,    88,     0,    89,    90,     0,
       0,    89,    90
};

static const yytype_int16 yycheck[] =
{
      40,    41,    42,    43,    23,    16,     0,     7,    21,    28,
      15,     5,    25,     6,    14,     8,     1,   134,   135,     7,
      60,    61,    62,    17,   141,     1,    37,     3,     4,     5,
       1,     7,    37,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    34,
      90,    91,    28,    31,     1,    36,    37,    33,    71,    35,
      33,     1,    31,    34,    58,    14,     1,    14,     3,     4,
       5,     7,     7,     3,    14,    94,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   131,     1,    28,    46,    47,    36,    37,    33,    34,
      35,    32,    34,    15,    38,    14,    33,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,     1,    33,    15,    33,    32,    36,    37,     1,
      37,     3,     4,     5,    34,     7,     7,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    34,    11,    32,    34,    28,    36,    37,     1,
       1,    33,     5,    35,     6,     6,     8,     8,     3,     4,
       5,     6,     7,     8,     9,    19,    15,    12,    13,     3,
       4,     5,    58,     7,    -1,     9,   131,    -1,    12,    13,
      94,    32,    34,    28,    -1,    -1,    31,    -1,    33,    -1,
      35,     3,     4,     5,    28,     7,    -1,    31,    32,    33,
      -1,    35,     3,     4,     5,    -1,     7,    -1,     9,    -1,
      -1,    12,    13,    -1,    -1,    -1,    28,    -1,    29,    30,
      -1,    33,    -1,    35,    -1,    36,    37,    28,    -1,    -1,
      31,    -1,    33,    -1,    35,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,    -1,    -1,    -1,    36,    37,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    -1,    -1,    -1,    36,    37,
      38,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,    -1,    34,
      -1,    36,    37,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    34,    -1,    36,    37,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    -1,    -1,    -1,    -1,    36,    37,    17,    18,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    -1,    -1,    -1,    17,    18,    36,    37,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    17,    -1,    36,    37,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    -1,    -1,    -1,
      36,    37,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    27,    28,    29,    30,    -1,    36,    37,    -1,
      -1,    36,    37
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     8,    40,    41,    42,    44,    45,     7,     0,
      41,     7,    14,    43,    46,    47,    31,    33,    14,    15,
      37,    31,    50,    44,    53,    54,     1,    34,    44,    48,
      49,     7,    43,     3,     3,     4,     5,     7,     9,    12,
      13,    28,    33,    35,    50,    51,    52,    53,    57,    46,
      55,    56,    32,    53,    34,    46,     1,    34,    15,    38,
      33,    33,    33,    57,    57,     1,    57,    57,    32,    51,
      32,    51,     1,    14,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    36,
      37,    16,     1,    14,    15,    48,     1,    34,    57,    58,
      57,    57,     1,    14,    34,     1,    34,     1,    32,    53,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,     7,    57,     1,    57,    55,
      34,    15,     1,    34,    34,    34,    32,    38,    58,    52,
      52,    11,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    42,    43,    43,
      44,    44,    45,    45,    46,    46,    47,    47,    47,    47,
      47,    48,    48,    49,    50,    50,    50,    50,    50,    51,
      51,    52,    52,    52,    52,    52,    52,    52,    52,    53,
      53,    54,    54,    55,    55,    56,    56,    56,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     1,     3,
       1,     1,     5,     2,     1,     4,     4,     4,     4,     3,
       3,     3,     1,     2,     4,     3,     3,     4,     5,     2,
       1,     2,     2,     1,     3,     3,     5,     7,     5,     2,
       1,     3,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     4,
       4,     4,     4,     3,     1,     1,     1,     1,     3,     1
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
#line 42 "src/syntax.y"
                    {
        yyval = create_parent_node(AST_PROGRAM, (yyloc).first_line, 1, yyvsp[0]);
        if (!err_flag)
            print_ast(yyval);
      }
#line 1600 "syntax.tab.c"
    break;

  case 3: /* ExtDefList: ExtDef ExtDefList  */
#line 50 "src/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1606 "syntax.tab.c"
    break;

  case 4: /* ExtDefList: %empty  */
#line 51 "src/syntax.y"
      { yyval = NULL; }
#line 1612 "syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 55 "src/syntax.y"
                                  { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1618 "syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 56 "src/syntax.y"
                     { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1624 "syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FuncDec CompStmt  */
#line 57 "src/syntax.y"
                                 { yyval = create_parent_node(AST_EXT_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1630 "syntax.tab.c"
    break;

  case 8: /* ExtDecList: VarDec  */
#line 61 "src/syntax.y"
                   { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1636 "syntax.tab.c"
    break;

  case 9: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 62 "src/syntax.y"
                              { yyval = create_parent_node(AST_EXT_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1642 "syntax.tab.c"
    break;

  case 10: /* Specifier: TYPE  */
#line 65 "src/syntax.y"
                { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1648 "syntax.tab.c"
    break;

  case 11: /* Specifier: StructSpecifier  */
#line 66 "src/syntax.y"
                      { yyval = create_parent_node(AST_SPECIFIER, (yyloc).first_line, 1, yyvsp[0]); }
#line 1654 "syntax.tab.c"
    break;

  case 12: /* StructSpecifier: STRUCT ID LC DefList RC  */
#line 70 "src/syntax.y"
                                         { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1660 "syntax.tab.c"
    break;

  case 13: /* StructSpecifier: STRUCT ID  */
#line 71 "src/syntax.y"
                { yyval = create_parent_node(AST_STRUCT_SPECIFIER, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1666 "syntax.tab.c"
    break;

  case 14: /* VarDec: ID  */
#line 75 "src/syntax.y"
           { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1672 "syntax.tab.c"
    break;

  case 15: /* VarDec: VarDec LSB INT RSB  */
#line 76 "src/syntax.y"
                         { yyval = create_parent_node(AST_VAR_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1678 "syntax.tab.c"
    break;

  case 16: /* FuncDec: ID LP VarList RP  */
#line 81 "src/syntax.y"
                          { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1684 "syntax.tab.c"
    break;

  case 17: /* FuncDec: ID LP error RP  */
#line 82 "src/syntax.y"
                     { splerror(ERR_B, (yyloc).first_line, "Invalid parameter declaration ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1690 "syntax.tab.c"
    break;

  case 18: /* FuncDec: ID LP VarList error  */
#line 83 "src/syntax.y"
                          { splerror(ERR_B, (yyloc).first_line, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1696 "syntax.tab.c"
    break;

  case 19: /* FuncDec: ID LP RP  */
#line 84 "src/syntax.y"
               { yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1702 "syntax.tab.c"
    break;

  case 20: /* FuncDec: ID LP error  */
#line 85 "src/syntax.y"
                  { splerror(ERR_B, (yyloc).first_line, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_FUNC_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1708 "syntax.tab.c"
    break;

  case 21: /* VarList: ParamDec COMMA VarList  */
#line 89 "src/syntax.y"
                                { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1714 "syntax.tab.c"
    break;

  case 22: /* VarList: ParamDec  */
#line 90 "src/syntax.y"
               { yyval = create_parent_node(AST_VAR_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1720 "syntax.tab.c"
    break;

  case 23: /* ParamDec: Specifier VarDec  */
#line 94 "src/syntax.y"
                           { yyval = create_parent_node(AST_PARAM_DEC, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1726 "syntax.tab.c"
    break;

  case 24: /* CompStmt: LC DefList StmtList RC  */
#line 98 "src/syntax.y"
                                 { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1732 "syntax.tab.c"
    break;

  case 25: /* CompStmt: LC DefList RC  */
#line 99 "src/syntax.y"
                    { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1738 "syntax.tab.c"
    break;

  case 26: /* CompStmt: LC StmtList RC  */
#line 100 "src/syntax.y"
                     { yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1744 "syntax.tab.c"
    break;

  case 27: /* CompStmt: LC DefList StmtList error  */
#line 101 "src/syntax.y"
                                { splerror(ERR_B, (yyloc).first_line, "Missing closing brace '}'", NULL); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1750 "syntax.tab.c"
    break;

  case 28: /* CompStmt: LC DefList StmtList DefList RC  */
#line 102 "src/syntax.y"
                                     { splerror(ERR_B, (yyloc).first_line, "Invalid def list. ", NULL); yyval = create_parent_node(AST_COMP_STMT, (yyloc).first_line, 0); }
#line 1756 "syntax.tab.c"
    break;

  case 29: /* StmtList: Stmt StmtList  */
#line 106 "src/syntax.y"
                        { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1762 "syntax.tab.c"
    break;

  case 30: /* StmtList: Stmt  */
#line 107 "src/syntax.y"
           { yyval = create_parent_node(AST_STMT_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1768 "syntax.tab.c"
    break;

  case 31: /* Stmt: Exp SEMI  */
#line 111 "src/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1774 "syntax.tab.c"
    break;

  case 32: /* Stmt: Exp error  */
#line 112 "src/syntax.y"
                { splerror(ERR_B, (yyloc).first_line, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 0); yyerrok; }
#line 1780 "syntax.tab.c"
    break;

  case 33: /* Stmt: CompStmt  */
#line 113 "src/syntax.y"
               { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 1, yyvsp[0]); }
#line 1786 "syntax.tab.c"
    break;

  case 34: /* Stmt: RETURN Exp SEMI  */
#line 114 "src/syntax.y"
                      { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1792 "syntax.tab.c"
    break;

  case 35: /* Stmt: RETURN Exp error  */
#line 115 "src/syntax.y"
                       { splerror(ERR_B, (yyloc).first_line, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_STMT, (yyloc).first_line, 2, yyvsp[-2], yyvsp[-1]); yyerrok; }
#line 1798 "syntax.tab.c"
    break;

  case 36: /* Stmt: IF LP Exp RP Stmt  */
#line 116 "src/syntax.y"
                                   { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1804 "syntax.tab.c"
    break;

  case 37: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 117 "src/syntax.y"
                                  { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 7, yyvsp[-6], yyvsp[-5], yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1810 "syntax.tab.c"
    break;

  case 38: /* Stmt: WHILE LP Exp RP Stmt  */
#line 118 "src/syntax.y"
                           { yyval = create_parent_node(AST_STMT, (yyloc).first_line, 5, yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1816 "syntax.tab.c"
    break;

  case 39: /* DefList: Def DefList  */
#line 122 "src/syntax.y"
                     { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1822 "syntax.tab.c"
    break;

  case 40: /* DefList: Def  */
#line 123 "src/syntax.y"
          { yyval = create_parent_node(AST_DEF_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1828 "syntax.tab.c"
    break;

  case 41: /* Def: Specifier DecList SEMI  */
#line 127 "src/syntax.y"
                            { yyval = create_parent_node(AST_DEF, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1834 "syntax.tab.c"
    break;

  case 42: /* Def: Specifier DecList error  */
#line 128 "src/syntax.y"
                              { splerror(ERR_B, (yyloc).first_line, "Missing semicolon ';'", NULL); yyval = create_parent_node(AST_DEF, (yyloc).first_line, 0); yyerrok; }
#line 1840 "syntax.tab.c"
    break;

  case 43: /* DecList: Dec  */
#line 132 "src/syntax.y"
             { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 1, yyvsp[0]); }
#line 1846 "syntax.tab.c"
    break;

  case 44: /* DecList: Dec COMMA DecList  */
#line 133 "src/syntax.y"
                        { yyval = create_parent_node(AST_DEC_LIST, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1852 "syntax.tab.c"
    break;

  case 45: /* Dec: VarDec  */
#line 137 "src/syntax.y"
            { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 1, yyvsp[0]); }
#line 1858 "syntax.tab.c"
    break;

  case 46: /* Dec: VarDec ASSIGN Exp  */
#line 138 "src/syntax.y"
                        { yyval = create_parent_node(AST_DEC, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1864 "syntax.tab.c"
    break;

  case 47: /* Dec: VarDec ASSIGN error  */
#line 139 "src/syntax.y"
                          { splerror(ERR_B, (yyloc).first_line, "Invalid initialization", NULL); yyval = create_parent_node(AST_DEC, (yyloc).first_line, 0); yyerrok; }
#line 1870 "syntax.tab.c"
    break;

  case 48: /* Exp: Exp ASSIGN Exp  */
#line 143 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1876 "syntax.tab.c"
    break;

  case 49: /* Exp: Exp AND Exp  */
#line 145 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1882 "syntax.tab.c"
    break;

  case 50: /* Exp: Exp OR Exp  */
#line 146 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1888 "syntax.tab.c"
    break;

  case 51: /* Exp: Exp BITWISE_AND Exp  */
#line 147 "src/syntax.y"
                          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1894 "syntax.tab.c"
    break;

  case 52: /* Exp: Exp BITWISE_OR Exp  */
#line 148 "src/syntax.y"
                         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1900 "syntax.tab.c"
    break;

  case 53: /* Exp: Exp LT Exp  */
#line 150 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1906 "syntax.tab.c"
    break;

  case 54: /* Exp: Exp LE Exp  */
#line 151 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1912 "syntax.tab.c"
    break;

  case 55: /* Exp: Exp GT Exp  */
#line 152 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1918 "syntax.tab.c"
    break;

  case 56: /* Exp: Exp GE Exp  */
#line 153 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1924 "syntax.tab.c"
    break;

  case 57: /* Exp: Exp NE Exp  */
#line 154 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1930 "syntax.tab.c"
    break;

  case 58: /* Exp: Exp EQ Exp  */
#line 155 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1936 "syntax.tab.c"
    break;

  case 59: /* Exp: Exp PLUS Exp  */
#line 157 "src/syntax.y"
                   { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1942 "syntax.tab.c"
    break;

  case 60: /* Exp: Exp MINUS Exp  */
#line 158 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1948 "syntax.tab.c"
    break;

  case 61: /* Exp: Exp MUL Exp  */
#line 159 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1954 "syntax.tab.c"
    break;

  case 62: /* Exp: Exp DIV Exp  */
#line 160 "src/syntax.y"
                  { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1960 "syntax.tab.c"
    break;

  case 63: /* Exp: LP Exp RP  */
#line 162 "src/syntax.y"
                { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1966 "syntax.tab.c"
    break;

  case 64: /* Exp: LP Exp error  */
#line 163 "src/syntax.y"
                   { splerror(ERR_B, (yyloc).first_line, "Missing closing parenthesis ')'", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 1972 "syntax.tab.c"
    break;

  case 65: /* Exp: LP error RP  */
#line 164 "src/syntax.y"
                  { splerror(ERR_B, (yyloc).first_line, "Invalid expression: ", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 1978 "syntax.tab.c"
    break;

  case 66: /* Exp: MINUS Exp  */
#line 166 "src/syntax.y"
                { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1984 "syntax.tab.c"
    break;

  case 67: /* Exp: NOT Exp  */
#line 167 "src/syntax.y"
              { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 2, yyvsp[-1], yyvsp[0]); }
#line 1990 "syntax.tab.c"
    break;

  case 68: /* Exp: ID LP RP  */
#line 168 "src/syntax.y"
               { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 1996 "syntax.tab.c"
    break;

  case 69: /* Exp: ID LP Args RP  */
#line 169 "src/syntax.y"
                    { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2002 "syntax.tab.c"
    break;

  case 70: /* Exp: ID LP Args error  */
#line 170 "src/syntax.y"
                       { splerror(ERR_B, (yyloc).first_line, "Missing right parenthesis ')'", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2008 "syntax.tab.c"
    break;

  case 71: /* Exp: ID LP error RP  */
#line 171 "src/syntax.y"
                     { splerror(ERR_B, (yyloc).first_line, "Invalid argument: ", NULL); yyval = create_parent_node(AST_EXP, (yyloc).first_line, 0); yyerrok; }
#line 2014 "syntax.tab.c"
    break;

  case 72: /* Exp: Exp LSB Exp RSB  */
#line 173 "src/syntax.y"
                      { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 4, yyvsp[-3], yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2020 "syntax.tab.c"
    break;

  case 73: /* Exp: Exp DOT ID  */
#line 174 "src/syntax.y"
                 { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2026 "syntax.tab.c"
    break;

  case 74: /* Exp: ID  */
#line 176 "src/syntax.y"
         { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2032 "syntax.tab.c"
    break;

  case 75: /* Exp: INT  */
#line 177 "src/syntax.y"
          { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2038 "syntax.tab.c"
    break;

  case 76: /* Exp: FLOAT  */
#line 178 "src/syntax.y"
            { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2044 "syntax.tab.c"
    break;

  case 77: /* Exp: CHAR  */
#line 179 "src/syntax.y"
           { yyval = create_parent_node(AST_EXP, (yyloc).first_line, 1, yyvsp[0]); }
#line 2050 "syntax.tab.c"
    break;

  case 78: /* Args: Exp COMMA Args  */
#line 183 "src/syntax.y"
                     { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 3, yyvsp[-2], yyvsp[-1], yyvsp[0]); }
#line 2056 "syntax.tab.c"
    break;

  case 79: /* Args: Exp  */
#line 184 "src/syntax.y"
          { yyval = create_parent_node(AST_ARGS, (yyloc).first_line, 1, yyvsp[0]); }
#line 2062 "syntax.tab.c"
    break;


#line 2066 "syntax.tab.c"

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

#line 187 "src/syntax.y"


void yyerror(const char *s) {
    fprintf(stderr, "%s at line %d\n", s, yylloc.first_line);
    // suppressed
}

int main() {
    yyparse();
}
