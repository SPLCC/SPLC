#ifndef __SPLC_BASIC_SPECIFIERS_HH__
#define __SPLC_BASIC_SPECIFIERS_HH__ 1

namespace splc {

enum class DeclSpecifier {
    Auto,
    Extern,
    Register,
    Static,
    Typedef,
    Inline,
    Const,
    Restrict,
    Volatile,
    Void,
    Int,
    Float,
    Char,
    Signed,
    Unsigned,
    Long,
    Struct,
    Union,
    Enum,
};

} // namespace splc

#endif // __SPLC_BASIC_SPECIFIERS_HH__