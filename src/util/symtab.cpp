#include <string>

#include "util/symtab.hpp"
#include "util/type/type.hpp"


namespace pascc::util {

template<typename SYM, typename DAT>
SymTab<SYM, DAT>::SymTab()
{
  enterScope();
}

template<typename SYM, typename DAT>
auto SymTab<SYM, DAT>::probe(const SYM &name) const -> DAT
{
  auto it = symtab_.back().find(name);
  if (it != symtab_.back().end())
  {
    return it->second;
  }
  return {};
}

template<typename SYM, typename DAT>
auto SymTab<SYM, DAT>::lookup(const SYM &name) const -> DAT
{
  for (auto it = symtab_.rbegin(); it != symtab_.rend(); ++it)
  {
    auto jt = it->find(name);
    if (jt != it->end())
    {
      return jt->second;
    }
  }
  return {};
}

template<typename SYM, typename DAT>
void SymTab<SYM, DAT>::insert(const SYM &name, const DAT &data)
{
  symtab_.back().emplace(name, data);
}

template<typename SYM, typename DAT>
void SymTab<SYM, DAT>::enterScope()
{
  symtab_.emplace_back();
}

template<typename SYM, typename DAT>
void SymTab<SYM, DAT>::exitScope()
{
  symtab_.pop_back();
}

template class SymTab<std::string, SymType *>;
template class SymTab<std::string, VarType *>;
template class SymTab<std::string, SubprogType *>;
template class SymTab<std::string, int*>;  // for testing

}  // namespace pascc::util