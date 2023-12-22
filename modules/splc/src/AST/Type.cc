#include "Core/Base.hh"

#include "AST/AST.hh"
#include "AST/Type.hh"

namespace splc {

Type::Type(Ptr<const AST> newDeclRoot_) { declRoot = newDeclRoot_->copy(); }

TypeCastResult Type::castTo(const Type &t2) const
{
    if (this == &t2 || this->declRoot == t2.declRoot)
        return TypeCastResult::Equivalent;
    return TypeCastResult::Equivalent;
}

Ptr<Type> Type::getSubType() const
{
    // TODO
    return makeSharedPtr<Type>(this->declRoot);
}

Ptr<Type> Type::decay() const
{
    // TODO
    return makeSharedPtr<Type>(this->declRoot);
}

} // namespace splc