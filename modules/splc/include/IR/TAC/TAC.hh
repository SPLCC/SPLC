#include "AST/ASTCommons.hh"
#include "AST/Type.hh"
#ifndef __SPLC_TAC_TAC_HH__
#define __SPLC_TAC_TAC_HH__ 1
#include "Core/Base.hh"
#include "IR/TAC/TACBase.hh"

#include <cstdio>
#include <memory>
#include <string>
#include <vector>


namespace splc {

class TAC {
  public:
    int address;
    std::string name; // decl
    splc::Type *type;

    TAC() = default;

    virtual std::string toString();

    virtual int generateID()
    {
        int ID = tacVector.size();
        tacVector.push_back(this);
        return ID;
    }
};

class LabelTAC : public TAC {
  public:
    LabelTAC(int address) 
    { 
        TAC::address = address; 
    }
    std::string toSting()
    {
        char buf[1024];
        sprintf(buf, "LABEL label%d :", TAC::address);
        return buf;
    }
};

class DecTAC : public TAC {
  public:
    // TODO
};

class FunctionTAC : public TAC {
  public:
    FunctionTAC(int address, std::string name)
    {
        TAC::address = address;
        TAC::name = name;
    }
    std::string toString() 
    {
        char buf[1024];
        sprintf(buf, "FUNCTION %s :", TAC::name.c_str());
        return buf;
    }
};

class AssignTAC : public TAC {
  public:
    int rightAddress;
    AssignTAC(int address, int rightAddress)
    {
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString() override
    {
        char buf[1024];
        // TODO
        return buf;
    }
};

class AssignAddressTAC : public TAC {
  public:
    int rightAddress;
    AssignAddressTAC(int address, int rightAddress)
    {
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString()
    {
        char buf[1024];
        sprintf(buf, "t%d := &t%d", TAC::address, rightAddress);
        return buf;
    }
};

class AssignValueTAC : public TAC {
  public:
    int rightAddress;
    AssignValueTAC(int address, int rightAddress)
    {
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString()
    {
        char buf[1024];
        sprintf(buf, "t%d := *t%d", TAC::address, rightAddress);
        return buf;
    }
};

class CopyValueTAC : public TAC {
  public:
    int rightAddress;
    CopyValueTAC(int address, int rightAddress)
    {
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString()
    {
        char buf[1024];
        sprintf(buf, "*t%d := t%d", TAC::address, rightAddress);
        return buf;
    }
};

class ArithmeticTAC : public TAC {
  public:
    // TODO
};

class GOTOTAC: public TAC{
public:
    int* label;
    GOTOTAC(int address, int* label){
        TAC::address = address;
        this->label = label;
    }
    std::string toString(){
        char buf[1024];
        sprintf(buf, "GOTO label%d", *label);
        return buf;
    }
};

class IFTAC : public TAC {
  public:
    // TODO
};

class ReturnTAC: public TAC{
public:
    int rightAddress;
    ReturnTAC(int address, int rightAddress){
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString(){
        char buf[1024];
        // TODO
        return buf;
    }
};

class ParamTAC : public TAC {
  // TODO
};

class ArgTAC : public TAC {
  // TODO
};

class CallTAC : public TAC {
  // TODO
};

class ReadTAC: public TAC{
public:
    ReadTAC(int address){
        TAC::address = address;
    }
    std::string toString(){
        char buf[1024];
        sprintf(buf, "READ t%d", TAC::address);
        return buf;
    }
};

class WriteTAC: public TAC{
public:
    int rightAddress;
    WriteTAC(int address, int rightAddress){
        TAC::address = address;
        this->rightAddress = rightAddress;
    }
    std::string toString(){
        char buf[1024];
        sprintf(buf, "WRITE t%d", rightAddress);
        return buf;
    }
};

ExprType translExpr(Ptr<splc::AST> node);

std::vector<ExprType> translArgs(Ptr<splc::AST> node);



} // namespace splc

#endif // __SPLC_TAC_TAC_HH__
