#include "Core/splc.hh"
#include "AST/ASTContext.hh"
#include "AST/ASTProcess.hh"
#include "AST/DerivedAST.hh"
#include "IO/Driver.hh"
#include "SIR/IRBuilder.hh"
#include "SIR/IROptimizer.hh"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace splc;

using SIR::IRBuilder;
using SIR::IROptimizer;
using SIR::IRProgram;

int main(const int argc, const char **argv)
{
    // check for the right # of arguments
    if (argc < 3) {
        //  exit with failure condition
        std::cout << "usage: [traceParsing 0/1] [file] ...\n";
        return (EXIT_FAILURE);
    }

    bool traceParsing = std::stoi(std::string{argv[1]}) != 0;

    IO::Driver driver{traceParsing};

    // TODO(future): just parse the first file first

    // assume file, prod code, use stat to check
    std::vector<std::string> filenameVector;
    filenameVector.reserve(argc - 2);
    std::transform(argv + 2, argv + argc, std::back_inserter(filenameVector),
                   [](const char *str) { return std::string{str}; });
    auto tunit = driver.parse(filenameVector[0]);

    auto node = tunit->getRootNode();
    if (node) {
        SPLC_LOG_DEBUG(nullptr, false) << "\n"
                                       << splc::treePrintTransform(*node);
        SPLC_LOG_DEBUG(nullptr, false) << "\n" << *node->getContext();
    }

    IRBuilder builder{*tunit->getTypeContext()};

    Ptr<IRProgram> program = builder.makeProgram(node);

    // Disable
    // IROptimizer::optimizeProgram(program);

    // Open the output file
    std::ofstream outputFile("output.ir");
    if (!outputFile.is_open()) {
        std::cout << "Failed to open output file\n";
        return (EXIT_FAILURE);
    }

    // Write the program to the output file
    IRProgram::writeProgram(outputFile, program);

    // Close the output file
    outputFile.close();

    return (EXIT_SUCCESS);
}