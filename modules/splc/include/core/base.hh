#ifndef __SPLC_CORE_BASE_HH__
#define __SPLC_CORE_BASE_HH__ 1

#include <memory>

namespace splc {

/* Convenient notation */
template <class T>
using Ptr = std::shared_ptr<T>;

/* Convenient notation */
template <class T, class... Args>
auto createPtr(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} /* namespace splc */

#endif /* __SPLC_CORE_BASE_HH__ */
