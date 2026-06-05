#include "State.hpp"

#include "ErrorResult.hpp"
#include "StringResult.hpp"

namespace Stle {
namespace Cmd {

void State::setOkResult() {
    _result = std::make_unique<OkResult>();
}

void State::setErrorResult(const char *msg) {
    _result = std::make_unique<ErrorResult>(msg);
}

void State::setStringResult(const char *str) {
    _result = std::make_unique<StringResult>(str);
}

}
}
