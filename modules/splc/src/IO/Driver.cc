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

Driver::Driver() { translationManager = createPtr<TranslationManager>(); }

Ptr<TranslationUnit> Driver::parse(std::string_view filename)
{
    translationManager->startTranslationRecord();
    Ptr<TranslationContext> context = translationManager->pushTranslationContext(nullptr, filename);
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
    scanner = createPtr<Scanner>(*translationManager);
    parser = createPtr<Parser>(*translationManager, *this, *scanner);
    
    yy_buffer_state *initialState = scanner->yy_create_buffer(initialContext->inputStream.get(), SPLC_BUF_SIZE);
    scanner->yypush_buffer_state(initialState);

    const int accept{0};

    if (parser->parse() != accept) {
        // TODO: revise
        SPLC_LOG_DEBUG(nullptr) << "Parse failed.";
    }
    translationManager->endTranslationRecord();

    Ptr<TranslationUnit> tunit = translationManager->getTranslationUnit();
    return tunit;
}

void Driver::add_upper()
{
    uppercase++;
    chars++;
    words++;
}

void Driver::add_lower()
{
    lowercase++;
    chars++;
    words++;
}

void Driver::add_word(const std::string &word)
{
    words++;
    chars += word.length();
    for (const char &c : word) {
        if (islower(c)) {
            lowercase++;
        }
        else if (isupper(c)) {
            uppercase++;
        }
    }
}

void Driver::add_newline()
{
    lines++;
    chars++;
}

void Driver::add_char() { chars++; }

std::ostream &Driver::print(std::ostream &stream)
{
    using SeqType = utils::logging::ControlSeq;
    stream << SeqType::BrightRed << "Results: " << SeqType::Reset << "\n";
    stream << SeqType::BrightBlue << "Uppercase: " << SeqType::Reset
           << uppercase << "\n";
    stream << SeqType::BrightBlue << "Lowercase: " << SeqType::Reset
           << lowercase << "\n";
    stream << SeqType::BrightBlue << "Lines: " << SeqType::Reset << lines
           << "\n";
    stream << SeqType::BrightBlue << "Words: " << SeqType::Reset << words
           << "\n";
    stream << SeqType::BrightBlue << "Characters: " << SeqType::Reset << chars
           << "\n";
    return (stream);
}

} // namespace splc::IO