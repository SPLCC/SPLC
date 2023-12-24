#include "AST/ASTBase.hh"
#include <AST/DerivedTypes.hh>

using namespace splc;

Type *ASTHelper::getBaseTySpecRecursive(const AST &root) noexcept
{
    // TODO: use a general judge:
    // TODO: require refactoring of the ASTSymbolType class.
    if (root.symbolType == ASTSymbolType::IntTy)
        return &root.typeContext->SInt32Ty; // Just assume it is signed int32

    Type *ty = nullptr;
    for (auto &child : root.children) {
        Type *cTy = getBaseTySpecRecursive(*child);
        ty = cTy == nullptr ? ty : cTy;
        if (ty != nullptr)
            break;
    }

    return ty;
}

/// traverse down the sequence
Type *ASTHelper::getBaseTySpec(const AST &root) noexcept
{
    splc_dbgassert(
        isASTSymbolTypeOneOf(root.symbolType, ASTSymbolType::DeclSpec));
    // TODO: remove the assumption
    // ASSUMPTION: only one type specifier, i.e. no things like unsigned long
    // long or some stupid things. Either BE Int32Ty, or just **** up everything
    Type *baseType = getBaseTySpecRecursive(root);
    splc_dbgassert(baseType)
        << "no type specifier has been provided, at " << root.loc;
    return baseType;
}

Type *ASTHelper::processParamDeclRecursive(const AST &root) noexcept
{
    splc_dbgassert(root.symbolType == ASTSymbolType::ParamDecltr);

    Ptr<AST> paramDeclSpecNode = root.children[0];
    Type *paramDeclSpec = getBaseTySpec(*paramDeclSpecNode);
    Type *paramTy = paramDeclSpec;

    // If there is a declarator, then use it.
    if (root.getChildrenNum() > 1) {
        Ptr<AST> paramDirDeclNode = root.children[1];
        paramTy = processDecltrRecursive(*paramDirDeclNode, paramDeclSpec);
    }

    return paramTy;
}

/// Examine
/// - `PtrDecltr`

Type *ASTHelper::processDecltrSubDispatch(const AST &root, Type *base) noexcept
{
    if (isASTSymbolTypeOneOf(root.getSymbolType(), ASTSymbolType::DirDecltr,
                                 ASTSymbolType::DirFuncDecltr)) {
        if (root.children[0]->symbolType == ASTSymbolType::ID) {
            return base;
        }
        else if (root.children[0]->symbolType ==
                 ASTSymbolType::WrappedDirDecltr) {
            ///===----If root contains a function declaration and a parameter
            /// list
            std::vector<Type *> params;

            // If there is indeed param list
            if (auto it = std::find_if(
                    root.children.begin(), root.children.end(),
                    [](const Ptr<AST> &node) {
                        return node->symbolType == ASTSymbolType::ParamList;
                    });
                it != root.children.end()) {
                auto &paramNode = *it;
                std::transform(paramNode->children.begin(),
                               paramNode->children.end(),
                               std::back_inserter(params), [](Ptr<AST> child) {
                                   return processParamDeclRecursive(*child);
                               });
            }
            Type *func =
                FunctionType::get(base, params, false); // TODO: support VarArg
                                                        // no var arg support

            return processDecltrSubDispatch(*root.children[0], func);
        }
        else if (root.children[0]->symbolType == ASTSymbolType::DirDecltr &&
                 root.getChildrenNum() >= 3 &&
                 root.children[1]->symbolType == ASTSymbolType::OpLSB) {

            splc_dbgassert(root.getChildrenNum() == 4)
                << " at " << root.loc
                << "\n"; // You cannot let SPLC infer array size
            ///===----Arrays
            // TODO: add `CallDecltr` and `ArrayDecltr` to `ASTSymbolType`
            // TODO: add support for constant expression
            // ASSUMPTION: don't mess up with AssignExpr
            Ptr<AST> dirDeclNode = root.children[0];
            Ptr<AST> literalNode = root.children[2]->children[0]->children[0];
            auto val = literalNode->getValue<ASTUIntType>();

            auto &context = base->getContext();
            Type *arrType = ArrayType::get(base, val);
            return processDecltrSubDispatch(*dirDeclNode, arrType);
        }
    }
    else if (root.symbolType == ASTSymbolType::PtrDecltr) {
        ///===----Pointers
        return processDecltrSubDispatch(*root.children[1],
                                        base->getPointerTo());
    }
    else if (root.symbolType == ASTSymbolType::WrappedDirDecltr) {
        return processDecltrRecursive(*root.children[0], base);
    }

    splc_error() << "\neither a syntax error is present, or the system does "
                    "not support this type: "
                 << root.getSymbolType();
    return nullptr;
}

Type *ASTHelper::processDecltrRecursive(const AST &root, Type *base) noexcept
{
    splc_dbgassert(root.getSymbolType() == ASTSymbolType::Decltr)
        << ", at " << root.loc;
    return processDecltrSubDispatch(*root.children[0], base);
}

Type *ASTHelper::processInitDecltr(const AST &root, Type *base) noexcept
{
    splc_dbgassert(root.getSymbolType() == ASTSymbolType::InitDecltr);
    // don't care about the initializer
    return processDecltrRecursive(*root.children[0], base);
}

std::vector<Type *> ASTHelper::processInitDeclList(const AST &root,
                                                   Type *base) noexcept
{
    splc_dbgassert(root.symbolType == ASTSymbolType::InitDecltrList);
    std::vector<Type *> res;
    std::transform(
        root.children.begin(), root.children.end(), std::back_inserter(res),
        [=](const Ptr<AST> &node) { return processInitDecltr(*node, base); });
    return res;
}

std::vector<Type *> ASTHelper::getDeclTy(const AST &root) noexcept
{
    // TODO: maybe just add more support, whatsoever
    TypeContext &context = *root.typeContext;
    const AST &dirDecl = *root.children[0];

    // Get base type
    Type *baseType = getBaseTySpec(*dirDecl.children[0]);

    // From base type, get extended type
    std::vector<Type *> extTypes =
        processInitDeclList(*dirDecl.children[1], baseType);

    return extTypes;
}

Type *ASTHelper::getFuncTy(const AST &root) noexcept
{
    TypeContext &context = *root.typeContext;

    // Get base type. There is always
    // a base type specifier, provided by the parser.
    Type *baseType = nullptr;
    baseType = getBaseTySpec(*root.children[0]);

    auto dirDeclRoot = root.children[1]->children[0]; // FuncDef -> FuncDecltr
    Type *retType = processDecltrSubDispatch(*dirDeclRoot, baseType);
    while (dirDeclRoot->getSymbolType() != ASTSymbolType::DirFuncDecltr &&
           !dirDeclRoot->isChildrenEmpty()) {
        dirDeclRoot = dirDeclRoot->children.back();
    }
    splc_dbgassert(dirDeclRoot->getSymbolType() == ASTSymbolType::DirFuncDecltr)
        << ": " << dirDeclRoot->getSymbolType();

    // There is always a parameter list for a function. It can be either
    // empty, or contains ParamDecltr. In both cases, we can directly
    // transform its children using processParamDeclRecursive()
    std::vector<Type *> paramTys;
    auto &paramListNode = dirDeclRoot->children[1]->children[0];
    // SPLC_LOG_DEBUG(&root.loc, true) << "processing";
    if (paramListNode->getChildrenNum() > 0) {
        std::transform(paramListNode->children.begin(),
                       paramListNode->children.end(),
                       std::back_inserter(paramTys), [](const Ptr<AST> &node) {
                           return processParamDeclRecursive(*node);
                       });
    }

    Type *funcType = FunctionType::get(retType, paramTys, false);
    return funcType;
}

std::vector<Type *> ASTHelper::getTypeHelperDispatch(const AST &root)
{
    // TODO: deal with more types
    switch (root.symbolType) {
    case ASTSymbolType::Decl:
        return getDeclTy(root);
    case ASTSymbolType::FuncDef:
    case ASTSymbolType::FuncDecl:
        return {getFuncTy(root)};
    default:
        splc_error() << "invalid symbol type: "
                     << getASTSymbolColor(root.symbolType) << root.symbolType;
    }
    // TODO: verify
    SPLC_LOG_ERROR(&root.loc, true) << "invalid type";
    throw RuntimeError(233, "invalid type");
}

std::vector<Type *> AST::getType() const
{
    splc_dbgassert(isASTSymbolTypeOneOf(
        this->symbolType, ASTSymbolType::Decl, ASTSymbolType::FuncDef,
        ASTSymbolType::FuncDecl));
    auto vec = ASTHelper::getTypeHelperDispatch(*this);
    return vec;
}
