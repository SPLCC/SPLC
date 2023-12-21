#ifndef __SPLC_BASIC_SPECIFIERS_HH__
#define __SPLC_BASIC_SPECIFIERS_HH__ 1

namespace splc {

enum DeclSpecifier {
    DS_Auto,
    DS_Extern,
    DS_Register,
    DS_Static,
    DS_Typedef,
    DS_Inline,
    DS_Const,
    DS_Restrict,
    DS_Volatile,
    DS_Void,
    DS_Int,
    DS_Float,
    DS_Char,
    DS_Signed,
    DS_Unsigned,
    DS_Long,
    DS_Struct,
    DS_Union,
    DS_Enum,
};

} // namespace splc

#endif // __SPLC_BASIC_SPECIFIERS_HH__