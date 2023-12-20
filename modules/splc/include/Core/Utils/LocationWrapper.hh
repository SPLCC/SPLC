#ifndef __SPLC_CORE_UTILS_LOCATIONWRAPPER_HH__
#define __SPLC_CORE_UTILS_LOCATIONWRAPPER_HH__ 1

#include "Core/Utils/Location.hh"

namespace splc {

namespace utils {

/// \brief Wrapper class for printing the position without context.
struct PositionNoContextWrapper {
  public:
    PositionNoContextWrapper(const Position &position_) : pos{position_} {}

    const Position &pos;
};

/** \brief Intercept output stream redirection.
 ** \param ostr the destination output stream
 ** \param posWrapper a reference to the PositionNoContextWrapper to redirect
 */
inline std::ostream &operator<<(std::ostream &ostr, const PositionNoContextWrapper &posWrapper)
{
    return ostr << posWrapper.pos.line << '.' << posWrapper.pos.column;
}

inline PositionNoContextWrapper printPositionNoContext(const Position &pos_) { return {pos_}; }


/// \brief Wrapper class for printing the position without context.
struct LocationNoContextWrapper {
  public:
    LocationNoContextWrapper(const Location &location_) : loc{location_} {}

    const Location &loc;
};

/** \brief Intercept output stream redirection.
 ** \param ostr the destination output stream
 ** \param locWrapper a reference to the LocationNoContextWrapper to redirect
 */
inline std::ostream &operator<<(std::ostream &ostr, const LocationNoContextWrapper &locWrapper)
{
    Location::CounterType end_col = locWrapper.loc.end.column;
    if (locWrapper.loc.end.contextName) {
        if (!locWrapper.loc.begin.contextName ||
            locWrapper.loc.begin.contextID == locWrapper.loc.end.contextID) {
            ostr << printPositionNoContext(locWrapper.loc.begin);
            ostr << '-' << locWrapper.loc.end.line << '.' << end_col;
        }
        else {
            // locWrapper.loc.begin.contextID != locWrapper.loc.end.contextID
            ostr << *locWrapper.loc.begin.contextName << " [" << locWrapper.loc.begin.contextID << "]"
                 << ":" << locWrapper.loc.begin.line << "." << locWrapper.loc.begin.column;

            ostr << '-' << *locWrapper.loc.end.contextName << " [" << locWrapper.loc.end.contextID
                 << "]" << ':' << locWrapper.loc.end.line << '.' << end_col;
        }
    }
    else {
        ostr << printPositionNoContext(locWrapper.loc.begin);
        if (locWrapper.loc.begin.line < locWrapper.loc.end.line)
            ostr << '-' << locWrapper.loc.end.line << '.' << end_col;
        else if (locWrapper.loc.begin.column < end_col)
            ostr << '-' << end_col;
    }
    return ostr;
}

inline LocationNoContextWrapper printLocationNoContext(const Location &loc_) { return {loc_}; }


} // namespace utils

using Position = utils::Position;
using Location = utils::Location;
using ContextKeyType = utils::Location::ContextKeyType;

} // namespace splc

#endif // __SPLC_CORE_UTILS_LOCATIONWRAPPER_HH__
