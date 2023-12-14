#ifndef __SPLC_DRIVER_HH__
#define __SPLC_DRIVER_HH__ 1

#include <cstddef>
#include <istream>
#include <string>
#include <string_view>

#include "context.hh"
#include "core/splc.hh"
#include "scanner.hh"

namespace splc {

class Driver {
  public:
    Driver() = default;

    /**
     * parse - parse from a file
     * @param filename - valid string with input file
     */
    void parse(const char *const filename);
    /**
     * parse - parse from a c++ input stream
     * @param is - std::istream&, valid input stream
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
    
    Ptr<splc::Parser> parser;
    Ptr<splc::Scanner> scanner;

    std::size_t chars = 0;
    std::size_t words = 0;
    std::size_t lines = 0;
    std::size_t uppercase = 0;
    std::size_t lowercase = 0;
};

} // namespace splc
#endif /* __SPLC_DRIVER_HH__ */
