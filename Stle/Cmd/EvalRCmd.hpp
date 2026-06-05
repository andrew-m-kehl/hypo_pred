#ifndef STLE_CMD_EVAL_R_CMD_HPP
#define STLE_CMD_EVAL_R_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

#include "Stle/ExprAll.hpp"

namespace Stle {
namespace Cmd {

class STLE_API EvalRCmd : public Cmd {
public:
    EvalRCmd(const ExprPtr &expr) : Cmd(CMD_EVAL_R), _expr(expr)  {
    }
    
    const ExprPtr &expr() const {
        return _expr;
    }    
    
    bool equals(const Cmd &other) const override;
        
private:
    ExprPtr _expr;
};

}
}

#endif
