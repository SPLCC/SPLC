#ifndef __SPLC_IO_DRIVER_HH__
#define __SPLC_IO_DRIVER_HH__ 1

#include <cstddef>
#include <istream>
#include <string>
#include <string_view>

#include "Core/splc.hh"

#include "AST/TranslationManager.hh"
#include "IO/Parser.hh"
#include "IO/Scanner.hh"

namespace splc::IO {

class Driver {
  public:
    Driver();

    /**
     * Parse from a file
     * \param filename valid string with input file
     */
    void parse(const std::string &filename);

    /**
     * Parse from a c++ input stream
     * \param is std::istream&, valid input stream
     */
    void parse(const std::string &streamName, std::istream &iss);

    void add_upper();
    void add_lower();
    void add_word(const std::string &word);
    void add_newline();
    void add_char();

    std::ostream &print(std::ostream &stream);

  private:
    void tryParse(const std::string &filename, std::istream &stream);

    Ptr<TranslationManager> translationManager;
    Ptr<Parser> parser;
    Ptr<Scanner> scanner;

    std::size_t chars = 0;
    std::size_t words = 0;
    std::size_t lines = 0;
    std::size_t uppercase = 0;
    std::size_t lowercase = 0;
};

} // namespace splc
#endif /* __SPLC_IO_DRIVER_HH__ */
