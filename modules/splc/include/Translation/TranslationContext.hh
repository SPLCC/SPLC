#ifndef __SPLC_TRANSLATION_TRANSLATIONCONTEXT_HH__
#define __SPLC_TRANSLATION_TRANSLATIONCONTEXT_HH__ 1

#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "Translation/TranslationBase.hh"

namespace splc {

enum TranslationContextBufferType {
    File,
    MacroVarExpansion,
};

class TranslationContext : std::enable_shared_from_this<TranslationContext> {
  public:
    TranslationContext() = delete;

    TranslationContext(const TranslationContextIDType contextID_,
                       TranslationContextBufferType type_,
                       std::string_view name_,
                       WeakPtr<const TranslationContext> parent,
                       const Location *intrLocation_,
                       Ptr<std::istream> inputStream_)
        : contextID{contextID_}, type{type_}, name{name_}, parent{parent},
          intrLocation(intrLocation_ ? *intrLocation_ : Location{}), content{},
          inputStream{inputStream_}
    {
    }

    TranslationContext(const TranslationContextIDType contextID_,
                       TranslationContextBufferType type_,
                       std::string_view name_,
                       WeakPtr<const TranslationContext> parent,
                       const Location *intrLocation_, std::string_view content_,
                       Ptr<std::istream> inputStream_)
        : contextID{contextID_}, type{type_}, name{name_}, parent{parent},
          intrLocation(intrLocation_ ? *intrLocation_ : Location{}),
          content{content_}, inputStream{inputStream_}
    {
    }

    virtual ~TranslationContext() = default;

    TranslationContextKeyType getKey() const
    {
        return {&name, contextID,
                parent.use_count() ? &(parent.lock().get()->intrLocation)
                                   : nullptr};
    }

    const TranslationContextIDType contextID;
    const TranslationContextBufferType type;
    const TranslationContextNameType name;
    WeakPtr<const TranslationContext> parent;

    mutable Location intrLocation; // Interrupt Location
    const MacroContentType content;
    Ptr<std::istream> inputStream;

    friend class TranslationContextManager;
};

} // namespace splc

#endif // __SPLC_TRANSLATION_TRANSLATIONCONTEXT_HH__