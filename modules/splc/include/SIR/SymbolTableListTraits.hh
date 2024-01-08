#ifndef __SPLC_SIR_SYMBOLTABLELISTTRAITS_HH__
#define __SPLC_SIR_SYMBOLTABLELISTTRAITS_HH__ 1

#include "SIR/SIRCommons.hh"

namespace splc::SIR {

template <class T, typename... args>
class SymbolTableList {
  public:
    using iterator = List<T>::iterator;
    using const_iterator = List<T>::const_iterator;
    using reverse_iterator = List<T>::reverse_iterator;
    using const_reverse_iterator = List<T>::const_reverse_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    size_t size() const;
    bool empty() const;
};

} // namespace splc::SIR

#endif // __SPLC_SIR_SYMBOLTABLELISTTRAITS_HH__