#include "AST/Type.hh"
#include "AST/AST.hh"

namespace splc {

Type::Type(Ptr<const AST> newDeclRoot_) : declRoot(newDeclRoot_) {}

Type Type::makeType(Ptr<const AST> declRoot)
{
    // TODO: depending on the actual type, erase the unnecessary element.
    // - If it is a direct declaration (struct, union, other), store everything
    // - If it is a function definition, store without its body.
    return {declRoot->copy()};
}

TypeCastResult Type::castTo(Type t2) const
{
    if (this == &t2 || this->declRoot == t2.declRoot)
        return TypeCastResult::Equivalent;
    return TypeCastResult::Equivalent;
}

Type Type::getSubType() const
{
    // TODO
    return makeType(this->declRoot);
}

Type Type::decay() const
{
    // TODO
    return makeType(this->declRoot);
}

} // namespace splc