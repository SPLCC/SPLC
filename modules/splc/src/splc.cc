#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Core/splc.hh"

#include "AST/ASTBase.hh"
#include "AST/ASTProcess.hh"
#include "IO/Driver.hh"

int main(const int argc, const char **argv)
{
    // check for the right # of arguments
    if (argc < 2) {
        //  exit with failure condition
        return (EXIT_FAILURE);
    }

    splc::IO::Driver driver;

    // TODO(future): just parse the first file first
    // assume file, prod code, use stat to check
    std::vector<std::string> filenameVector;
    filenameVector.resize(argc - 1);
    std::transform(argv + 1, argv + argc, filenameVector.begin(),
                   [](const char *str) { return std::string{str}; });
    driver.parse(filenameVector[0]);
    driver.print(std::cout) << "\n";

    using namespace splc;
    using splc::AST;

    // test
    Ptr<AST> node0, node1;
    auto node = splc::createAST<AST>(splc::ASTSymbolType::CHAR,
                                     splc::Location{}, node0, node1);

    *node >> ASTProcess::removeASTPunctuators >>
        ASTProcess::removeASTPunctuators;

    applyASTTransform(*node, ASTProcess::removeASTPunctuators,
                      ASTProcess::removeASTPunctuators);

    std::cout << treePrintTransform(*node);
    SPLC_LOG_DEBUG(nullptr) << "Hi" << SPLC_LOGGER_TAG("debug tag");

    SPLC_ASSERT(std::string("23333").empty());

    return (EXIT_SUCCESS);
}
