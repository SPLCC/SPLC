#include "IO/Scanner.hh"
#include "Core/Utils/Logging.hh"

namespace splc::IO {

int Scanner::yywrap()
{
    int k = driver.parseWrap(SPLC_BUF_SIZE);
    SPLC_LOG_DEBUG(nullptr, "Scanner wrapped: " << k);
    
    return k;
}

} // namespace splc::IO