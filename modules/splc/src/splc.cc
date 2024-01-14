#include "Core/splc.hh"
#include "AST/ASTContext.hh"
#include "AST/ASTProcess.hh"
#include "AST/DerivedAST.hh"
#include "CodeGen/ObjBuilder.hh"
#include "Core/Utils/CommandLineParser.hh"
#include "IO/Driver.hh"
#include "SIR/IRBuilder.hh"
#include "SIR/IROptimizer.hh"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string_view>

using namespace splc;
using namespace std::string_view_literals;
using utils::CommandLineParser;
using CS = utils::logging::ControlSeq;

static bool writeAssembly = false;
static bool writeMIPSTarget = false; ///< If true, write MIPS instead
std::vector<std::string> sourceFiles;

bool parseArgs(const int argc, const char *const argv[])
{
    CommandLineParser parser{"splc"};

    parser.addSeqDirArgName("SOURCE_FILE");
    parser.addPositionalArg("genasm", CommandLineParser::ArgOption::NoOption);
    parser.addPositionalArg("target", CommandLineParser::ArgOption::WithOption);

    parser.parseArgs(argc, argv);

    if (auto ivec = parser.get("genasm")) {
        writeAssembly = true;
        SPLC_LOG_DEBUG(nullptr, false) << "writing assembly instead";
    }
    if (auto ivec = parser.get<std::string_view>("target")) {
        // TODO(future): allow generation for multiple targets simulatenously
        // maybe
        if ((*ivec)[0] == "mips"sv) {
            writeMIPSTarget = true;
        }
        else if ((*ivec)[0] == "default") {
            // SKIP
        }
        else {
            SPLC_LOG_ERROR(nullptr, false) << "unknown target " << CS::BrightRed
                                           << (*ivec)[0] << CS::Reset;
        }
        SPLC_LOG_DEBUG(nullptr, false) << "writing MIPS instead";
    }
    if (auto ivec = parser.getDirectArgVec(); !ivec.empty()) {
        sourceFiles = ivec;
    }

    return parser.isHelpParsed();
}

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

void testObjBuilder(std::string_view path, Ptr<TranslationUnit> tunit)
{
    ObjBuilder builder;

    std::ofstream of{std::string{path} + ".ll"};

    builder.generateModule(*tunit);
    // builder.optimizeModule();
    builder.writeModuleAsLLVMIR(of);
    of.flush();

    if (writeAssembly) {
        if (writeMIPSTarget) {
            builder.writeModuleAsAsm(std::string{path} + ".asm", "mips");
        }
        else {
            builder.writeModuleAsAsm(std::string{path} + ".asm");
        }
    }
    else {
        if (writeMIPSTarget) {
            builder.writeModuleAsObj(std::string{path} + ".o", "mips");
        }
        else {
            builder.writeModuleAsObj(std::string{path} + ".o");
        }
    }
}

int main(const int argc, const char *const argv[])
{
    bool helpOnly = parseArgs(argc, argv);

    if (helpOnly) {
        return (EXIT_SUCCESS);
    }

    if (sourceFiles.empty()) {{
        SPLC_LOG_FATAL_ERROR(nullptr, false) << "no input files";
        return (EXIT_FAILURE);}
    }

    UniquePtr<SPLCContext> context = makeUniquePtr<SPLCContext>();
    IO::Driver driver{*context};

    // TODO(future): just parse the first file first

    auto tunit = driver.parse(sourceFiles[0]);

    auto root = tunit->getRootNode();
    if (root) {
        SPLC_LOG_DEBUG(nullptr, false) << "\n"
                                       << splc::treePrintTransform(*root);
        SPLC_LOG_DEBUG(nullptr, false) << "\n" << *root->getASTContext();
    }

    // writeSIR(tunit->getContext(), root); // Don't write it right now
    testObjBuilder(sourceFiles[0], tunit);

    return (EXIT_SUCCESS);
}