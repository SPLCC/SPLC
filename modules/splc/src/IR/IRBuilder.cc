#include "IR/IRBuilder.hh"
#include "IR/IR.hh"

using namespace splc;

void IRBuilder::recursiveParseAST(PtrAST &astRoot)
{
    for (auto &child : astRoot->getChildren()) {
        if (isASTSymbolTypeOneOfThem(astRoot->getSymbolType(),
                                     ASTSymbolType::ParseRoot,
                                     ASTSymbolType::TransUnit)) {
            recursiveParseAST(child);
        }
    }
}

void IRBuilder::parseAST(PtrAST astRoot)
{
    astCtxt = astRoot->getASTContext();
    recursiveParseAST(astRoot);
}