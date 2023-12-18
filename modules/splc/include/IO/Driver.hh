#ifndef __SPLC_IO_DRIVER_HH__
#define __SPLC_IO_DRIVER_HH__ 1

#include <cstddef>
#include <istream>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "AST/TranslationManager.hh"
#include "IO/IOBase.hh"
#include "IO/Parser.hh"
#include "IO/Scanner.hh"

namespace splc::IO {

class Driver {
  public:
    Driver();

    /// Parse from a file
    /// \param filename valid string with input file
    void parse(const std::string &filename);

    // TODO: remove experimental
    void parse(const std::vector<std::string> &filenameVector);

    /// Parse from a c++ input stream
    /// \param is std::istream&, valid input stream
    void parse(const std::string &streamName, std::istream &iss);

    // TODO: remove experimental
    int parseWrap(int size);

    void add_upper();
    void add_lower();
    void add_word(const std::string &word);
    void add_newline();
    void add_char();

    std::ostream &print(std::ostream &stream);

  private:
    void builtinParse(const std::string &bufferName, std::istream &stream);

    Ptr<TranslationManager> translationManager;
    Ptr<Parser> parser;
    Ptr<Scanner> scanner;

    // TODO: remove experimental
    std::vector<std::string> filenameVector;
    size_t fileIndex;

    std::size_t chars = 0;
    std::size_t words = 0;
    std::size_t lines = 0;
    std::size_t uppercase = 0;
    std::size_t lowercase = 0;
};

} // namespace splc::IO
#endif // __SPLC_IO_DRIVER_HH__
