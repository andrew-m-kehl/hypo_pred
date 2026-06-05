#include "PCRobMonitor.hpp"

#include "PCRobMonitorVisitor.hpp"

namespace Stle {

PCSeries PCRobMonitor::evalOutput(const ExprPtr &expr) {
    PCRobMonitorVisitor visitor(*this);
    return visitor.visit(expr.get());
}

}
