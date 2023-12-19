#include "IO/Scanner.hh"
#include "Core/Utils/Logging.hh"

namespace splc::IO {

int Scanner::yywrap()
{
    int k = driver.parseWrap(SPLC_BUF_SIZE);
    return k;
}

} // namespace splc::IO