#ifndef __SPLC_SIR_IROPTIMIZER_HH__
#define __SPLC_SIR_IROPTIMIZER_HH__ 1

#include "SIR/IRBase.hh"
#include "SIR/IRBuilder.hh"

namespace splc::SIR {

class DepNode {
  public:
    enum class Type {
        Unspecified,
        Input,
        Mid,
        Output,
    };

    enum class Color {
        Marked,
        Unmarked,
    };

  public:
    DepNode(IRVar *var_, IRStmt *stmt_, Type type_)
        : type{type_}, var{var_}, stmt{stmt_}, color{Color::Unmarked}
    {
    }

    static Ptr<DepNode> make(IRVar *var, IRStmt *stmt, Type type) noexcept
    {
        return makeSharedPtr<DepNode>(var, stmt, type);
    }

    static Ptr<DepNode> make(Ptr<IRVar> var, Ptr<IRStmt> stmt,
                             Type type) noexcept
    {
        return makeSharedPtr<DepNode>(var.get(), stmt.get(), type);
    }

    static void connect(Ptr<DepNode> parent, Ptr<DepNode> child) noexcept
    {
        parent->children.insert(child.get());
        child->parents.insert(parent.get());
    }

    static void disconnect(Ptr<DepNode> parent, Ptr<DepNode> child) noexcept
    {
        parent->children.erase(child.get());
        child->parents.erase(parent.get());
    }

    void setType(Type type_) noexcept { type = type_; }

    auto getType() const noexcept { return type; }

    void setColor(Color color_) noexcept { color = color_; }

    auto getColor() const noexcept { return color; }

    auto isUnmarked() const noexcept { return color == Color::Unmarked; }

    auto isMarked() const noexcept { return color == Color::Marked; }

    void setUnmarked() noexcept { color = Color::Unmarked; }

    void setMarked() noexcept { color = Color::Marked; }

    auto getVar() const noexcept { return var; }

    void setVar(IRVar *var_) noexcept { var = var_; }

    auto getStmt() const noexcept { return stmt; }

    void setStmt(IRStmt *stmt_) noexcept { stmt = stmt_; }

    auto getParents() noexcept { return parents; }

    auto getParents() const noexcept { return parents; }

    auto getChildren() noexcept { return children; }

    auto getChildren() const noexcept { return children; }

    Type type;
    Color color;
    IRVar *var;
    IRStmt *stmt;
    IRSet<DepNode *> parents;
    IRSet<DepNode *> children;
};

using DepNodeList = IRVec<Ptr<DepNode>>;
using DepNodeMap = std::map<IRVar *, Ptr<DepNode>>;
using DepKey = std::tuple<DepNodeList, DepNodeMap, DepNodeList, DepNodeList>;

class IROptimizer {
  public:
  protected:
    static void examineStmt(DepKey &key, Ptr<IRFunction> func,
                            Ptr<IRStmt> &stmt);

    static void examineStmt(DepKey &key, Ptr<IRFunction> func,
                            decltype(func->body.begin()) it);

    static DepKey buildDependency(Ptr<IRFunction> func);

    static void searchBranchIf(DepKey &key, Ptr<IRFunction> func,
                               decltype(func->body.begin()) it);

  public:
    static void removeUnusedStmts(Ptr<IRFunction> func);

    static void constPropagate(Ptr<IRFunction> func);

    static void optimizeArithmetic(Ptr<IRFunction> func);

    static void optimizeFunction(Ptr<IRFunction> func);

    static void optimizeProgram(Ptr<IRProgram> func);
};

} // namespace splc::SIR

#endif // __SPLC_SIR_IROPTIMIZER_HH__