#ifndef __SPLC_CORE_BASE_HH__
#define __SPLC_CORE_BASE_HH__ 1

#include <memory>

namespace splc {

// Convenient notation
template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
using WeakPtr = std::weak_ptr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T>;

// Convenient notation
template <class T, class... Args>
Ptr<T> makeSharedPtr(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T, class... Args>
UniquePtr<T> makeUniquePtr(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace splc

#endif // __SPLC_CORE_BASE_HH__
