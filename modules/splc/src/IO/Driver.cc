#include <cassert>
#include <cctype>
#include <fstream>
#include <string_view>

#include "Core/System.hh"
#include "Core/Utils.hh"

#include "IO/Driver.hh"
#include "Translation/TranslationBase.hh"
#include "Translation/TranslationManager.hh"

namespace splc::IO {

Driver::Driver() { transMgr = makeSharedPtr<TranslationManager>(); }

Ptr<TranslationUnit> Driver::parse(std::string_view filename)
{
    transMgr->startTranslationRecord();
    Ptr<TranslationContext> context = transMgr->pushTransFileContext(nullptr, filename);
    Ptr<TranslationUnit> tunit = internalParse(context);
    return tunit;
}

// // TODO: remove experimental
// Ptr<TranslationUnit>
// Driver::parse(const std::vector<std::string> &filenameVector_)
// {
//     filenameVector = filenameVector_;
//     fileIndex = 0;
//     std::ifstream in_file(filenameVector[0]);
//     if (!in_file.good()) {
//         exit(EXIT_FAILURE);
//     }
//     fileIndex++;
//     SPLC_LOG_DEBUG(nullptr) << "parsing filename: " << filenameVector[0];

//     Ptr<TranslationUnit> tunit = builtinParse(filenameVector[0], in_file);
//     return tunit;
// }

// TODO: revise whether this should remain
// Ptr<TranslationUnit> Driver::parse(const std::string &streamName,
//                                    std::istream &stream)
// {
//     if (!stream.good() && stream.eof()) {
//         return {};
//     }
//     // else
//     if (&stream == &std::cin) {
//         Ptr<TranslationUnit> tunit = builtinParse(streamName, stream);
//         return tunit;
//     }
//     return {};
// }

Ptr<TranslationUnit> Driver::internalParse(Ptr<TranslationContext> initialContext)
{
    scanner = makeSharedPtr<Scanner>(*transMgr);
    parser = makeSharedPtr<Parser>(*transMgr, *this, *scanner);
    scanner->setInitialContext(initialContext);

    const int accept{0};

    if (parser->parse() != accept) {
        // TODO: revise
        SPLC_LOG_DEBUG(nullptr, false) << "Parse failed.";
    }
    transMgr->endTranslationRecord();
    Ptr<TranslationUnit> tunit = transMgr->getTransUnit();
    return tunit;
}

} // namespace splc::IO