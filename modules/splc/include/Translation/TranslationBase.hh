#ifndef __SPLC_TRANSLATION_TRANSLATIONBASE_HH__
#define __SPLC_TRANSLATION_TRANSLATIONBASE_HH__ 1

#include <map>
#include <string>
#include <vector>

#include "Core/splc.hh"

namespace splc {

// forward declarations

class TranslationUnit;
class TranslationContext;
class TranslationContextManager;
class TranslationUnit;
class TranslationManager;
class TranslationLogger;
class TranslationOption;

using TranslationContextNameType = std::string;
using TranslationContextNameArgType = std::string_view;
using TranslationContextIDType = utils::Location::ContextIDType;
using TranslationContextKeyType = ContextKeyType;

using MacroVarIDType = TranslationContextNameType;
using MacroVarEntry = std::pair<Location, Ptr<TranslationContext>>;
using MacroVarConstEntry = std::pair<Location, Ptr<const TranslationContext>>;
using MacroVarMap = std::map<MacroVarIDType, MacroVarEntry, std::less<>>;
using MacroContentType = std::string;

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONBASE_HH__