#ifndef __SPLC_IO_DRIVER_HH__
#define __SPLC_IO_DRIVER_HH__ 1

#include <cstddef>
#include <istream>
#include <string>
#include <string_view>
#include <vector>

#include "Core/splc.hh"

#include "IO/IOBase.hh"
#include "IO/Parser.hh"
#include "IO/Scanner.hh"
#include "Translation/TranslationBase.hh"
#include "Translation/TranslationManager.hh"

namespace splc::IO {

class Driver {
  public:
    Driver(bool traceParsing_ = false);

    virtual ~Driver() = default;

    ///
    /// \brief Parse from a file
    /// \param filename valid string with input file
    ///
    Ptr<TranslationUnit> parse(std::string_view filename);

    // // TODO: remove experimental
    // Ptr<TranslationUnit> parse(const std::vector<std::string> &filenameVector);

    // /// Parse from a c++ input stream
    // /// \param is std::istream&, valid input stream
    // Ptr<TranslationUnit> parse(const std::string &streamName,
    //                            std::istream &iss);

  protected:
    Ptr<TranslationUnit> internalParse(Ptr<TranslationContext> initialContext);

    Ptr<TranslationManager> transMgr;
    Ptr<Parser> parser;
    Ptr<Scanner> scanner;

    bool traceParsing;
};

} // namespace splc::IO
#endif // __SPLC_IO_DRIVER_HH__
