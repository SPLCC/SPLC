#include "AST/ASTContextManager.hh"

namespace splc {

ASTContextManager::ASTContextManager() {}

Ptr<ASTContext> ASTContextManager::pushContext(Location &intrLoc,
                                               std::string &fileName_)
{
    Ptr<ASTContext> context =
        createPtr<ASTContext>(ASTContextBufferType::File, fileName_, intrLoc);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<ASTContext> ASTContextManager::pushContext(Location &intrLoc,
                                               std::string &macroName_,
                                               std::string &content_)
{
    Ptr<ASTContext> context = createPtr<ASTContext>(
        ASTContextBufferType::File, macroName_, intrLoc, content_);
    contextStack.push_back(context);
    allContexts.push_back(context);
    return context;
}

Ptr<ASTContext> ASTContextManager::popContext()
{
    Ptr<ASTContext> context = contextStack.back();
    contextStack.pop_back();
    return context;
}

bool ASTContextManager::isContextExistInStack(ASTContextBufferType type_,
                                              std::string_view contextName_)
{
    for (auto &contextPtr : contextStack)
    {
        if (contextPtr->name == contextName_)
            return true;
    }
    return false;
}

} // namespace splc