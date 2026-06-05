#include "SignalVars.hpp"

namespace Stle {

SignalVars::SignalVars(VarsXNTag, int count) {
    if (count < 0)
        throw std::runtime_error("Variable count should be non-negative.");
    for (int i = 0; i < count; ++i) {
        std::stringstream ss;
        ss << "x" << i;
        add(ss.str(), i);
    }
}

void SignalVars::add(const std::string &name, int index) {
    _map[name] = index;
}

void SignalVars::add(std::string &&name, int index) {
    _map[std::move(name)] = index;
}

int SignalVars::index(const std::string &name) const {
    auto it = _map.find(name);
    if (it == _map.end()) {
        std::stringstream ss;
        ss << "No variable named '" << name << "'.";
        throw std::runtime_error(ss.str());
    }
    return it->second;
}

}
