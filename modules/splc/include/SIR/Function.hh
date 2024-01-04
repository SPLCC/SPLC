#ifndef __SPLC_SIR_FUNCTION_HH__
#define __SPLC_SIR_FUNCTION_HH__ 1

#include "Basic/SPLCContext.hh"
#include "Core/Base.hh"
#include "SIR/SIRCommons.hh"
#include <string>
#include <vector>

namespace splc {

namespace SIR {

class Function {
  public:
    using BasicBlockListType = std::vector<PtrBB>;
    using ArgumentListType = std::vector<PtrArg>;

  private:
    BasicBlockListType basicBlocks;
    ArgumentListType arguments;
    String name;
    Ptr<Module> parent;
    SPLCContext &tyCtx;

    Function(const String &name, SPLCContext &C, Ptr<Module> parent = nullptr)
        : name(name), tyCtx(C), parent(parent)
    {
    }

  public:
    static Ptr<Function> create(const String &name, SPLCContext &C,
                                Ptr<Module> parent = nullptr)
    {
        return makeSharedPtr<Function>(name, C, parent);
    }

    ~Function() = default;

    const String &getName() const { return name; }
    Ptr<Module> getParentModule() const { return parent; }

    /// Basic block management
    void addBasicBlock(PtrBB BB) { basicBlocks.push_back(BB); }
    PtrBB getBasicBlock(unsigned index) { return basicBlocks[index]; }
    size_t getBasicBlocksNum() const { return basicBlocks.size(); }

    // Argument management
    void addArgument(PtrArg arg) { arguments.push_back(arg); }
    PtrArg getArgument(unsigned index) const { return arguments[index]; }
    size_t getArgumentsNum() const { return arguments.size(); }
};

} // namespace SIR
} // namespace splc

#endif // __SPLC_SIR_FUNCTION_HH__