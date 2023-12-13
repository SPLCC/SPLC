#include <cassert>
#include <cctype>
#include <fstream>

#include "driver.hh"

void splc::Driver::parse(const char *const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    tryParse(filename, in_file);
    return;
}

void splc::Driver::parse(const std::string &streamName, std::istream &stream)
{
    if (!stream.good() && stream.eof()) {
        return;
    }
    // else
    if (&stream == &std::cin)
    {
        tryParse(streamName, stream);
    }
    return;
}

void splc::Driver::tryParse(const std::string &filename, std::istream &stream)
{
    try {
        scanner.reset(new Scanner(filename, &stream));
    }
    catch (std::bad_alloc &ba) {
        // TODO:
        exit(EXIT_FAILURE);
    }

    try {
        parser.reset(new Parser((*scanner), (*this)));
    }
    catch (std::bad_alloc &ba) {
        // TODO:
        exit(EXIT_FAILURE);
    }

    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!!\n";
    }
    return;
}

void splc::Driver::add_upper()
{
    uppercase++;
    chars++;
    words++;
}

void splc::Driver::add_lower()
{
    lowercase++;
    chars++;
    words++;
}

void splc::Driver::add_word(const std::string &word)
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

void splc::Driver::add_newline()
{
    lines++;
    chars++;
}

void splc::Driver::add_char() { chars++; }

std::ostream &splc::Driver::print(std::ostream &stream)
{
    using SeqType = utils::logging::ControlSeq;
    stream << SeqType::BrightRed << "Results: " << SeqType::Reset << "\n";
    stream << SeqType::BrightBlue << "Uppercase: " << SeqType::Reset  << uppercase << "\n";
    stream << SeqType::BrightBlue << "Lowercase: " << SeqType::Reset  << lowercase << "\n";
    stream << SeqType::BrightBlue << "Lines: " << SeqType::Reset  << lines << "\n";
    stream << SeqType::BrightBlue << "Words: " << SeqType::Reset  << words << "\n";
    stream << SeqType::BrightBlue << "Characters: " << SeqType::Reset  << chars << "\n";
    return (stream);
}
