#include <cassert>
#include <cctype>
#include <fstream>
#include <string_view>

#include "AST/TranslationManager.hh"
#include "Core/Utils.hh"
#include "Core/Utils/Logging.hh"
#include "IO/Driver.hh"

namespace splc::IO {

Driver::Driver() { translationManager = createPtr<TranslationManager>(); }

void Driver::parse(const std::string &filename)
{
    SPLC_ASSERT(!filename.empty());
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    builtinParse(filename, in_file);
    return;
}

// TODO: remove experimental
void Driver::parse(const std::vector<std::string> &filenameVector_)
{
    filenameVector = filenameVector_;
    fileIndex = 0;
    std::ifstream in_file(filenameVector[0]);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    fileIndex++;
    SPLC_ASSERT(in_file.good());
    SPLC_LOG_DEBUG(nullptr, "filename: " << filenameVector[0]);
    builtinParse(filenameVector[0], in_file);
    return;
}

// TODO: revise
void Driver::parse(const std::string &streamName, std::istream &stream)
{
    if (!stream.good() && stream.eof()) {
        return;
    }
    // else
    if (&stream == &std::cin) {
        builtinParse(streamName, stream);
    }
    return;
}

// TODO: remove experimental
int Driver::parseWrap(int size) {
    if (fileIndex >= filenameVector.capacity())
    {
        return 1;
    }
    else
    {
        std::ifstream *in_file = new std::ifstream(filenameVector[fileIndex++]);
        std::string testString;
        *in_file >> testString;
        SPLC_LOG_DEBUG(nullptr, "test string: " << testString);
        yy_buffer_state *newBuffer = scanner->yy_create_buffer(in_file, size);
        // SPLC_LOG_DEBUG(nullptr, "buffer state: " << newBuffer);
        scanner->yy_switch_to_buffer((newBuffer));
        return 0;
    }
}

void Driver::builtinParse(const std::string &bufferName, std::istream &stream)
{
    scanner = createPtr<Scanner>(*this, &stream);
    parser = createPtr<Parser>(*translationManager, *this, *scanner);

    const int accept{0};

    if (parser->parse() != accept) {
        // TODO: revise
        std::cerr << "Parse failed!!\n";
    }
    return;
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