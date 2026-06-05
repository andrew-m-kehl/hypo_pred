#include "PCMonitor.hpp"

#include "PCMonitorVisitor.hpp"

namespace Stle {

PCSeries PCMonitor::evalOutput(const ExprPtr &expr) {
    PCMonitorVisitor visitor(*this);
    return visitor.visit(expr.get());
}

}
