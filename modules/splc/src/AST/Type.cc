#include "AST/Type.hh"
#include "AST/AST.hh"

namespace splc {

Type::Type(Ptr<const AST> newDeclRoot_) : declRoot(newDeclRoot_) {}

Type Type::makeType(Ptr<const AST> declRoot)
{
    // TODO: depending on the actual type, erase the unnecessary element.
    return {declRoot->copy([](Ptr<const AST>) { return true; }, false)};
}

bool Type::compareType(Type t1, Type t2)
{
    // TODO
    return true;
}

Type Type::getSubType(Type t1)
{
    // TODO
    return {nullptr};
}

Type Type::decay(Type t1)
{
    // TODO
    return {nullptr};
}

bool Type::implicitCast(Type dst, Type src)
{
    // TODO
    return true;
}

} // namespace splc