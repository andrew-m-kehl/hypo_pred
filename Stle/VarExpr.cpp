#include "VarExpr.hpp"

namespace Stle {

void VarExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_name);   
}

bool VarExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const VarExpr &otherVar = static_cast<const VarExpr&>(other);
    return _name == otherVar._name;            
}

void VarExpr::print(U::VPrinter &p) const {
    p << _name.c_str();
}

}
