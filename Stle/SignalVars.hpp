#ifndef STLE_SIGNAL_VARS_HPP
#define STLE_SIGNAL_VARS_HPP

#include "Api.hpp"

#include "U/MemoryU.hpp"

#include <unordered_map>
#include <sstream>

namespace Stle {

struct STLE_API VarsXNTag {};
constexpr VarsXNTag VARS_XN;

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API SignalVars : public U::CannotCopy {
public:
    SignalVars() {        
    }
    
    SignalVars(VarsXNTag, int count);
    
    void add(const std::string &name, int index);
    
    void add(std::string &&name, int index);
    
    void add(const char *name, int id) {
        add(std::string(name), id);
    }
    
    int index(const std::string &name) const;
    
    int operator[](const std::string &name) const {
        return index(name);
    }
    
    int operator[](const char *name) const {
        return index(std::string(name));
    }
        
private:
    std::unordered_map<std::string, int> _map;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
