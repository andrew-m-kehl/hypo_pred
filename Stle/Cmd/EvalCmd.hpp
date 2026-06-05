#ifndef STLE_CMD_EVAL_CMD_HPP
#define STLE_CMD_EVAL_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

#include "Stle/ExprAll.hpp"

namespace Stle {
namespace Cmd {

class STLE_API EvalCmd : public Cmd {
public:
    EvalCmd(const ExprPtr &expr) : Cmd(CMD_EVAL), _expr(expr)  {
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
