#include "Core/splc.hh"
#include "AST/ASTContext.hh"
#include "AST/ASTProcess.hh"
#include "AST/DerivedAST.hh"
#include "CodeGen/ObjBuilder.hh"
#include "IO/Driver.hh"
#include "SIR/IRBuilder.hh"
#include "SIR/IROptimizer.hh"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>

using namespace splc;

void writeSIR(SPLCContext &C, Ptr<AST> root)
{
    using SIR::IRBuilder;
    using SIR::IROptimizer;
    using SIR::IRProgram;
    IRBuilder builder{C};

    Ptr<IRProgram> program = builder.makeProgram(root);

    // Disable
    // IROptimizer::optimizeProgram(program);

    IRProgram::writeProgram(std::cout, program);
}

void testObjBuilder(std::string_view path, Ptr<TranslationUnit> tunit,
                    bool writeMIPSObj)
{
    ObjBuilder builder;

    std::ofstream of{std::string{path} + ".ll"};

    builder.generateModule(*tunit);
    // builder.optimizeModule();
    builder.writeModuleAsLLVMIR(of);
    of.flush();
    if (writeMIPSObj)
        builder.writeModuleAsMIPSObj(std::string{path} + ".o");
    else
        builder.writeModuleAsDefaultObj(std::string{path} + ".o");
}

int main(const int argc, const char *const argv[])
{
    // check for the right # of arguments
    if (argc != 3) {
        //  exit with failure condition
        std::cout << "usage: [1 if generate for MIPS. 0 for default target] "
                     "[file] ...\n";
        return (EXIT_FAILURE);
    }

    // bool traceParsing = std::stoi(std::string{argv[1]}) != 0;
    bool traceParsing = false;
    bool writeMIPSObj = std::stoi(std::string{argv[1]}) != 0;

    UniquePtr<SPLCContext> context = makeUniquePtr<SPLCContext>();
    IO::Driver driver{*context, traceParsing};

    // TODO(future): just parse the first file first

    // assume file, prod code, use stat to check
    std::vector<std::string> filenameVector;
    filenameVector.reserve(argc - 2);
    std::transform(argv + 2, argv + argc, std::back_inserter(filenameVector),
                   [](const char *str) { return std::string{str}; });
    auto tunit = driver.parse(filenameVector[0]);

    auto root = tunit->getRootNode();
    if (root) {
        SPLC_LOG_DEBUG(nullptr, false) << "\n"
                                       << splc::treePrintTransform(*root);
        SPLC_LOG_DEBUG(nullptr, false) << "\n" << *root->getASTContext();
    }

    // writeSIR(tunit->getContext(), root); // Don't write it right now
    testObjBuilder(filenameVector[0], tunit, writeMIPSObj);

    return (EXIT_SUCCESS);
}