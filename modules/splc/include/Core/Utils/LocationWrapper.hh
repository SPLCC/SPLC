#ifndef __SPLC_CORE_UTILS_LOCATION_WRAPPER_HH__
#define __SPLC_CORE_UTILS_LOCATION_WRAPPER_HH__ 1

#include "Core/Utils/location.hh"

namespace splc {

namespace utils {

using Position = splc::IO::position;
using Location = splc::IO::location;

} // namespace utils

using Position = splc::utils::Position;
using Location = splc::utils::Location;

} // namespace splc

#endif /* __SPLC_CORE_UTILS_LOCATION_WRAPPER_HH__ */
