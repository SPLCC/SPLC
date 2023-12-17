#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "Core/splc.hh"

#include "AST/ASTNode.hh"
#include "AST/ASTProcess.hh"
#include "IO/Driver.hh"

int main(const int argc, const char **argv)
{
    /** check for the right # of arguments **/
    if (argc < 2) {
        /** exit with failure condition **/
        return (EXIT_FAILURE);
    }

    if (std::strncmp(argv[1], "-h", 2) == 0) {
        /** simple help menu **/
        std::cout << "use -o for pipe to std::cin\n";
        std::cout << "just give a filename to count from a file\n";
        std::cout << "use -h to get this menu\n";
        return (EXIT_SUCCESS);
    }

    splc::IO::Driver driver;

    /** example for piping input from terminal, i.e., using cat **/
    if (std::strncmp(argv[1], "-o", 2) == 0) {
        driver.parse("stdin", std::cin);
    }
    /** example reading input from a file **/
    else {
        /** assume file, prod code, use stat to check **/
        std::vector<std::string> filenameVector;
        filenameVector.resize(argc - 1);
        std::transform(argv + 1, argv + argc, filenameVector.begin(), [](const char * str) { return std::string{str}; });
        driver.parse(filenameVector);
    }
    driver.print(std::cout) << "\n";

    using namespace splc;
    using splc::ASTNode;

    // test
    Ptr<ASTNode> node0, node1;
    auto node = splc::createASTNode(splc::ASTSymbolType::CHAR, splc::Location{},
                                    node0, node1);

    node >> ASTProcess::removeASTPunctuators >> ASTProcess::semanticAnalysis;

    applyASTTransform(node, ASTProcess::removeASTPunctuators,
                      ASTProcess::semanticAnalysis);

    std::cout << treePrintTransform(node);
    SPLC_ASSERT(std::string("23333").empty());
    return (EXIT_SUCCESS);
}
