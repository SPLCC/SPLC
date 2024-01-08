#ifndef __SPLC_SIR_MODULE_HH__
#define __SPLC_SIR_MODULE_HH__ 1

#include "Basic/SPLCContext.hh"
#include "SIR/Function.hh"
#include "SIR/SIRCommons.hh"
#include "SIR/SymbolTableListTraits.hh"

namespace splc::SIR {

/// A Module instance is used to store all the information related to an
/// SPLC module. Modules are the top level container of all other SPLC
/// Intermediate Representation (IR) objects. Each module directly contains a
/// list of globals variables, a list of functions, a list of libraries (or
/// other modules) this module depends on, a symbol table, and various data
/// about the target's characteristics.
class Module {
  public:
    using globalListType = SymbolTableList<GlobalVariable>;
    using functionListType = SymbolTableList<Function>;

    using global_iterator = globalListType::iterator;
    using const_global_iterator = globalListType::const_iterator;

    using iterator = functionListType::iterator;
    using const_iterator = functionListType::const_iterator;

    using reverse_iterator = functionListType::reverse_iterator;
    using const_reverse_iterator = functionListType::const_reverse_iterator;

    Module(const String &moduleID, SPLCContext &C);
    ~Module();

    /// @}
    /// @name Module Level Accessors and Mutators
    /// @{
    /// Get the module identifier which is, essentially, the name of the module.
    /// @returns the module identifier as a string
    const String &getModuleIdentifier() const;
    void setModuleIdentifier(const String &ID);

    /// Get the module's original source file name.
    const String &getSourceFileName() const;
    void setSourceFileName();

    /// Returns the number of non-debug IR instructions in the module.
    unsigned getInstructionCount() const;

    SPLCContext &getContext() const;

    /// @}
    /// @name Generic Value Accessors
    /// @{
    /// Return the global value in the module with the specified name, of
    /// arbitrary type.
    Ptr<GlobalValue> getNamedValue(const String &name) const;

    /// Return the number of global values in the module.
    unsigned getNamedValueNum() const;

    /// @}
    /// @name Function Accessors
    /// @{
    /// Look up the specified function in the module symbol table.
    // TODO FunctionCallee 
    Ptr<Function> getFunction(const String &name) const;

    /// @}
    /// @name Global Variable Iteration
    /// @{
    const Ptr<GlobalVariable> getGlobalVariable(const String &name) const;
    Ptr<GlobalVariable> getGlobalVariable(const String &name);

    global_iterator global_begin() { return globalList.begin(); }
    const_global_iterator global_begin() const { return globalList.begin(); }
    global_iterator global_end() { return globalList.end(); }
    const_global_iterator global_end() const { return globalList.end(); }
    size_t global_size() const { return globalList.size(); }
    bool global_empty() const { return globalList.empty(); }

    /// @}
    /// @name Function Iteration
    /// @{

    iterator begin() { return functionList.begin(); }
    const_iterator begin() const { return functionList.begin(); }
    iterator end() { return functionList.end(); }
    const_iterator end() const { return functionList.end(); }
    reverse_iterator rbegin() { return functionList.rbegin(); }
    const_reverse_iterator rbegin() const { return functionList.rbegin(); }
    reverse_iterator rend() { return functionList.rend(); }
    const_reverse_iterator rend() const { return functionList.rend(); }
    size_t size() const { return functionList.size(); }
    bool empty() const { return functionList.empty(); }

    /// @}
    /// @name Utility functions
    /// @{
    void print(std::ostream &os);


    friend class Constant;

    /// @}
    /// @name Member Variables
    /// @{
  private:
    globalListType globalList;
    functionListType functionList;
    String moduleID;
    String SourceFileName;
};

} // namespace splc::SIR

#endif // __SPLC_SIR_MODULE_HH__