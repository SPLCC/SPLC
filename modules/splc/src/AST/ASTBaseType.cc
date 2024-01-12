#include "AST/ASTBase.hh"
#include "Basic/DerivedTypes.hh"

using namespace splc;

using CS = splc::utils::logging::ControlSeq;

Type *AST::computeSimpleTypeSpec() const noexcept
{
    splc_assert(isSpecQualList() || isDeclSpec());

    if (context == nullptr) {
        splc_error() << "no type context has been provided";
    }

    Type *ret{nullptr};

    int nTypedef{0};
    int nVoid{0};
    int nChar{0};
    int nShort{0};
    int nInt{0};
    int nSigned{0};
    int nUnsigned{0};
    int nLong{0};
    int nFloat{0};
    int nDouble{0};

    int nAggr{0};

    for (auto &ent : getChildren()) {
        if (!ent->isTypeSpec())
            continue;

        auto realSpec = ent->getChildren()[0];

        switch (realSpec->getSymType()) {
        case ASTSymType::TypedefID: {
            if (nTypedef > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple typedef";
                return &getContext()->SInt32Ty;
            }
            ++nTypedef;
            break;
        }
        case ASTSymType::VoidTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || nUnsigned > 0 || nSigned > 0 ||
                ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            ++nVoid;
            break;
        }
        case ASTSymType::CharTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            ++nChar;
            break;
        }
        case ASTSymType::ShortTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            ++nShort;
            break;
        }
        case ASTSymType::IntTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            ++nInt;
            break;
        }
        case ASTSymType::SignedTy: {
            if (nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "contradictory type specifier";
                return &getContext()->SInt32Ty;
            }
            else if (nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "type specifier cannot be applied to floating-point "
                       "type";
                return &getContext()->SInt32Ty;
            }
            else if (ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "specifier cannot be applied to struct/union types";
                return &getContext()->SInt32Ty;
            }
            ++nSigned;
            break;
        }
        case ASTSymType::UnsignedTy: {
            if (nSigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "contradictory type specifier";
                return &getContext()->SInt32Ty;
            }
            else if (nFloat > 0 || nDouble > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "type specifier cannot be applied to floating-point "
                       "type";
                return &getContext()->SInt32Ty;
            }
            else if (ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "specifier cannot be applied to struct/union types";
                return &getContext()->SInt32Ty;
            }
            ++nUnsigned;
            break;
        }
        case ASTSymType::LongTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nFloat > 0 ||
                nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            else if (nLong == 2) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "too many `" << CS::Bold << "long" << CS::Reset
                    << "' as type specifier";
                return &getContext()->SInt32Ty;
            }
            ++nLong;
            break;
        }
        case ASTSymType::FloatTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            else if (nSigned > 0 || nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "cannot apply to floating-point type";
                return &getContext()->SInt32Ty;
            }
            ++nFloat;
            break;
        }
        case ASTSymType::DoubleTy: {
            if (nChar > 0 || nShort > 0 || nInt > 0 || nLong > 0 ||
                nFloat > 0 || nDouble > 0 || ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "multiple base type specifier";
                return &getContext()->SInt32Ty;
            }
            else if (nSigned > 0 || nUnsigned > 0) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "cannot apply to floating-point type";
                return &getContext()->SInt32Ty;
            }
            ++nDouble;
            break;
        }
        case ASTSymType::StructOrUnionSpec: {
            // TODO: handle struct/union
            if (ret == nullptr) {
                ret = realSpec->getLangType();
            }
            else if (nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 ||
                     nUnsigned > 0 || nLong > 0 || nFloat > 0 || nDouble > 0 ||
                     ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "invalid extra type specifier";
            }
            else {
                splc_error()
                    << "multiple struct/union specifier is not allowed";
            }
            break;
        }
        case ASTSymType::EnumSpec: {
            // TODO: handle enum
            if (ret == nullptr) {
                splc_error() << "using enum as type specifier is unsupported";
            }
            else if (nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 ||
                     nUnsigned > 0 || nLong > 0 || nFloat > 0 || nDouble > 0 ||
                     ret != nullptr) {
                SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                    << "invalid extra type specifier";
            }
            else {
                splc_error() << "multiple enum specifier is not allowed";
            }
            break;
        }
        default:
            SPLC_LOG_ERROR(&realSpec->getLocation(), false)
                << "invalid specifier: " << realSpec->getSymType();
        }
    }

    // Process primitive type declarations
    if (nVoid > 0 || nChar > 0 || nShort > 0 || nInt > 0 || nSigned > 0 ||
        nUnsigned > 0 || nLong > 0 || nFloat > 0 || nDouble > 0) {

        if (nVoid > 0)
            ret = &getContext()->VoidTy;
        if (nChar > 0)
            ret = &getContext()->SInt8Ty;
        if (nShort > 0)
            ret = &getContext()->SInt16Ty;
        if (nInt > 0)
            ret = &getContext()->SInt32Ty;
        if (nLong == 1)
            ret = &getContext()->SInt32Ty;
        if (nLong == 2)
            ret = &getContext()->SInt64Ty;
        if (nFloat == 2)
            ret = &getContext()->FloatTy;
        if (nDouble == 2)
            ret = &getContext()->DoubleTy;

        if (nSigned > 0) {
            if (ret == nullptr)
                ret = &getContext()->SInt32Ty;
            else
                ret = ret->getSigned();
        }
        if (nUnsigned > 0) {
            if (ret == nullptr)
                ret = &getContext()->UInt32Ty;
            else
                ret = ret->getUnsigned();
        }
    }

    if (nTypedef > 0) {
        setTypedef(true);
    }
    setLangType(ret);
    return ret;
}

Type *AST::computeSimpleDecltrList() const noexcept
{
    // TODO
    return nullptr;
}

Ptr<AST> AST::getRootIDNode() noexcept
{

    splc_assert(isSymTypeOneOf(
        ASTSymType::Decltr, ASTSymType::DirDecltr, ASTSymType::ID,
        ASTSymType::FuncDecltr, ASTSymType::DirFuncDecltr,
        ASTSymType::ParamDecltr, ASTSymType::InitDecltr));

    if (this->isID()) {
        return shared_from_this();
    }

    if (auto node = findFirstChildDFS(
            ASTSymType::Decltr, ASTSymType::DirDecltr, ASTSymType::ID,
            ASTSymType::FuncDecltr, ASTSymType::DirFuncDecltr,
            ASTSymType::ParamDecltr, ASTSymType::InitDecltr);
        node != nullptr) {
        return node->getRootIDNode();
    }

    return nullptr;
}

Ptr<const AST> AST::getRootIDNode() const noexcept
{

    splc_assert(isSymTypeOneOf(
        ASTSymType::Decltr, ASTSymType::DirDecltr, ASTSymType::ID,
        ASTSymType::FuncDecltr, ASTSymType::DirFuncDecltr,
        ASTSymType::ParamDecltr, ASTSymType::InitDecltr));

    if (this->isID()) {
        return shared_from_this();
    }

    if (auto node = findFirstChildDFS(
            ASTSymType::Decltr, ASTSymType::DirDecltr, ASTSymType::ID,
            ASTSymType::FuncDecltr, ASTSymType::DirFuncDecltr,
            ASTSymType::ParamDecltr, ASTSymType::InitDecltr);
        node != nullptr) {
        return node->getRootIDNode();
    }

    return nullptr;
}

Type *AST::getRootIDLangType() const noexcept
{
    auto node = getRootIDNode();
    if (node != nullptr) {
        return node->getLangType();
    }
    else {
        return nullptr;
    }
}

std::string_view AST::getRootID() const noexcept
{
    auto node = getRootIDNode();
    if (node != nullptr) {
        return node->getConstVal<ASTIDType>();
    }
    else {
        return "<invalid>";
    }
}

Type *ASTHelper::getBaseTySpecRecursive(const AST &root) noexcept
{
    // TODO: use a general judge:
    // TODO: require refactoring of the ASTSymbolType class.
    if (root.symType == ASTSymType::IntTy)
        return &root.context->SInt32Ty; // Just assume it is signed int32

    Type *ty = nullptr;
    for (auto &child : root.children_) {
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
    splc_dbgassert(isASTSymbolTypeOneOf(root.symType, ASTSymType::DeclSpec));
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
    splc_dbgassert(root.symType == ASTSymType::ParamDecltr);

    Ptr<AST> paramDeclSpecNode = root.children_[0];
    Type *paramDeclSpec = getBaseTySpec(*paramDeclSpecNode);
    Type *paramTy = paramDeclSpec;

    // If there is a declarator, then use it.
    if (root.getChildrenNum() > 1) {
        Ptr<AST> paramDirDeclNode = root.children_[1];
        paramTy = processDecltrRecursive(*paramDirDeclNode, paramDeclSpec);
    }

    return paramTy;
}

Type *ASTHelper::processDecltrSubDispatch(const AST &root, Type *base) noexcept
{
    if (isASTSymbolTypeOneOf(root.getSymType(), ASTSymType::DirDecltr,
                             ASTSymType::DirFuncDecltr)) {
        if (root.children_[0]->symType == ASTSymType::ID) {
            return base;
        }
        else if (root.children_[0]->symType == ASTSymType::WrappedDirDecltr) {
            ///===----If root contains a function declaration and a parameter
            /// list
            std::vector<Type *> params;

            // If there is indeed param list
            if (auto it = std::find_if(
                    root.children_.begin(), root.children_.end(),
                    [](const Ptr<AST> &node) {
                        return node->symType == ASTSymType::ParamList;
                    });
                it != root.children_.end()) {
                auto &paramNode = *it;
                std::transform(paramNode->children_.begin(),
                               paramNode->children_.end(),
                               std::back_inserter(params), [](Ptr<AST> child) {
                                   return processParamDeclRecursive(*child);
                               });
            }
            Type *func =
                FunctionType::get(base, params, false); // TODO: support VarArg
                                                        // no var arg support

            return processDecltrSubDispatch(*root.children_[0], func);
        }
        else if (root.children_[0]->symType == ASTSymType::DirDecltr &&
                 root.getChildrenNum() >= 3 &&
                 root.children_[1]->symType == ASTSymType::OpLSB) {

            splc_dbgassert(root.getChildrenNum() == 4)
                << " at " << root.loc
                << "\n"; // You cannot let SPLC infer array size
            ///===----Arrays
            // TODO: add `CallDecltr` and `ArrayDecltr` to `ASTSymbolType`
            // TODO: add support for constant expression
            // ASSUMPTION: don't mess up with AssignExpr
            Ptr<AST> dirDeclNode = root.children_[0];
            Ptr<AST> literalNode =
                root.children_[2]->children_[0]->children_[0];
            auto val = literalNode->getConstVal<ASTUIntType>();

            auto &context = base->getContext();
            Type *arrType = ArrayType::get(base, val);
            return processDecltrSubDispatch(*dirDeclNode, arrType);
        }
    }
    else if (root.symType == ASTSymType::WrappedDirDecltr) {
        return processDecltrRecursive(*root.children_[0], base);
    }

    splc_error() << "\neither a syntax error is present, or the system does "
                    "not support this type: "
                 << root.getSymType();
    return nullptr;
}

Type *ASTHelper::processDecltrRecursive(const AST &root, Type *base) noexcept
{
    splc_dbgassert(root.getSymType() == ASTSymType::Decltr)
        << ", at " << root.loc;
    return processDecltrSubDispatch(*root.children_[0], base);
}

Type *ASTHelper::processInitDecltr(const AST &root, Type *base) noexcept
{
    splc_dbgassert(root.getSymType() == ASTSymType::InitDecltr);
    // don't care about the initializer
    return processDecltrRecursive(*root.children_[0], base);
}

std::vector<Type *> ASTHelper::processInitDeclList(const AST &root,
                                                   Type *base) noexcept
{
    splc_dbgassert(root.symType == ASTSymType::InitDecltrList);
    std::vector<Type *> res;
    std::transform(
        root.children_.begin(), root.children_.end(), std::back_inserter(res),
        [=](const Ptr<AST> &node) { return processInitDecltr(*node, base); });
    return res;
}

std::vector<Type *> ASTHelper::getDeclTy(const AST &root) noexcept
{
    // TODO: maybe just add more support, whatsoever
    SPLCContext &context = *root.context;
    const AST &dirDecl = *root.children_[0];

    // Get base type
    Type *baseType = getBaseTySpec(*dirDecl.children_[0]);

    // From base type, get extended type
    std::vector<Type *> extTypes =
        processInitDeclList(*dirDecl.children_[1], baseType);

    return extTypes;
}

Type *ASTHelper::getFuncTy(const AST &root) noexcept
{
    SPLCContext &context = *root.context;

    // Get base type. There is always
    // a base type specifier, provided by the parser.
    Type *baseType = nullptr;
    baseType = getBaseTySpec(*root.children_[0]);

    auto dirDeclRoot = root.children_[1]->children_[0]; // FuncDef -> FuncDecltr
    Type *retType = processDecltrSubDispatch(*dirDeclRoot, baseType);
    while (dirDeclRoot->getSymType() != ASTSymType::DirFuncDecltr &&
           !dirDeclRoot->isChildrenEmpty()) {
        dirDeclRoot = dirDeclRoot->children_.back();
    }
    splc_dbgassert(dirDeclRoot->getSymType() == ASTSymType::DirFuncDecltr)
        << ": " << dirDeclRoot->getSymType();

    // There is always a parameter list for a function. It can be either
    // empty, or contains ParamDecltr. In both cases, we can directly
    // transform its children using processParamDeclRecursive()
    std::vector<Type *> paramTys;
    auto &paramListNode = dirDeclRoot->children_[1]->children_[0];
    // SPLC_LOG_DEBUG(&root.loc, true) << "processing";
    if (paramListNode->getChildrenNum() > 0) {
        std::transform(paramListNode->children_.begin(),
                       paramListNode->children_.end(),
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
    switch (root.symType) {
    case ASTSymType::Decl:
        return getDeclTy(root);
    case ASTSymType::FuncDef:
    case ASTSymType::FuncProto:
        return {getFuncTy(root)};
    default:
        splc_error() << "invalid symbol type: "
                     << getASTSymbolColor(root.symType) << root.symType;
    }
    // TODO: verify
    SPLC_LOG_ERROR(&root.loc, true) << "invalid type";
    throw RuntimeError(233, "invalid type");
}

// std::vector<Type *> AST::getContainedTys() const
// {
//     splc_dbgassert(isASTSymbolTypeOneOf(
//         this->symType, ASTSymType::Decl, ASTSymType::FuncDef,
//         ASTSymType::FuncProto));
//     auto vec = ASTHelper::getTypeHelperDispatch(*this);
//     return vec;
// }
