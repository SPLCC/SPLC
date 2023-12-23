#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>

#include "AST/AST.hh"
#include "Core/splc.hh"

#include "AST/AST.hh"
#include "AST/ASTProcess.hh"
#include "IO/Driver.hh"

int main(const int argc, const char **argv)
{
    // check for the right # of arguments
    if (argc < 2) {
        //  exit with failure condition
        return (EXIT_FAILURE);
    }

    splc::IO::Driver driver{false};

    // TODO(future): just parse the first file first
    // assume file, prod code, use stat to check
    std::vector<std::string> filenameVector;
    filenameVector.reserve(argc - 1);
    std::transform(argv + 1, argv + argc, std::back_inserter(filenameVector),
                   [](const char *str) { return std::string{str}; });
    auto tunit = driver.parse(filenameVector[0]);

    auto node = tunit->getRootNode();
    if (node)
        std::cout << splc::treePrintTransform(*node);

    // test
    // using namespace splc;
    // using splc::AST;

    // auto node0 =
    //     splc::makeAST<AST>(splc::ASTSymbolType::CHAR, splc::Location{}, 1ULL);
    // auto node1 = splc::makeAST<AST>(splc::ASTSymbolType::UPPER,
    //                                   splc::Location{}, std::string{"hihihi"});
    // auto node = splc::makeAST<AST>(splc::ASTSymbolType::CHAR,
    //                                  splc::Location{}, node0, node1);

    // *node >> ASTProcessor::removeASTPunctuators >>
    //     ASTProcessor::removeASTPunctuators;

    // applyASTTransform(*node, ASTProcessor::removeASTPunctuators,
    //                   ASTProcessor::removeASTPunctuators);

    // std::cout << treePrintTransform(*node);
    // SPLC_LOG_DEBUG(nullptr) << "Hi" << SPLC_LOGGER_TAG("debug tag");

    splc_assert(std::string("23333").empty());

    return (EXIT_SUCCESS);
}
