#include <cassert>
#include <cctype>
#include <fstream>
#include <string_view>

#include "Core/Utils.hh"

#include "IO/Driver.hh"
#include "Translation/TranslationManager.hh"

namespace splc::IO {

Driver::Driver() { translationManager = createPtr<TranslationManager>(); }

Ptr<TranslationUnit> Driver::parse(const std::string &filename)
{
    SPLC_ASSERT(!filename.empty());
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    Ptr<TranslationUnit> tunit = builtinParse(filename, in_file);
    return tunit;
}

// TODO: remove experimental
Ptr<TranslationUnit>
Driver::parse(const std::vector<std::string> &filenameVector_)
{
    filenameVector = filenameVector_;
    fileIndex = 0;
    std::ifstream in_file(filenameVector[0]);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    fileIndex++;
    SPLC_LOG_DEBUG(nullptr) << "parsing filename: " << filenameVector[0];
    Ptr<TranslationUnit> tunit = builtinParse(filenameVector[0], in_file);
    return tunit;
}

// TODO: revise whether this should remain
Ptr<TranslationUnit> Driver::parse(const std::string &streamName,
                                   std::istream &stream)
{
    if (!stream.good() && stream.eof()) {
        return {};
    }
    // else
    if (&stream == &std::cin) {
        Ptr<TranslationUnit> tunit = builtinParse(streamName, stream);
        return tunit;
    }
    return {};
}

Ptr<TranslationUnit> Driver::builtinParse(const std::string &bufferName,
                                          std::istream &stream)
{
    scanner = createPtr<Scanner>(*translationManager, &stream);
    parser = createPtr<Parser>(*translationManager, *this, *scanner);

    translationManager->startTranslationRecord();

    const int accept{0};

    if (parser->parse() != accept) {
        // TODO: revise
        std::cerr << "Parse failed!!\n";
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