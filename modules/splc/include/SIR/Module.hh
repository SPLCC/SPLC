#ifndef __SPLC_SIR_MODULE_HH__
#define __SPLC_SIR_MODULE_HH__ 1

#include "SIR/SIRCommons.hh"
#include "Basic/SPLCContext.hh"
#include "SIR/Function.hh"

namespace splc {
namespace SIR {

class Module {
    SPLCContext &tyCtx;
    String moduleID;
    Vec<PtrFunc> functions;

public:
    Module(const String &ID, SPLCContext &C) : moduleID(ID), tyCtx(C) {}

    const String &getModuleID() const {return moduleID;}

    void addFunction(PtrFunc func) {
        functions.push_back(func);
    }

    PtrFunc getFunction(const String &name) {
        for (auto &func : functions) {
            if (func->getName() == name) {
                return func;
            }
        }
        return nullptr;
    }

    ~Module() = default;
};

}
} // namespace splc

#endif // __SPLC_SIR_MODULE_HH__