#include "ReadSignalFlags.hpp"

#include <sstream>
#include <stdexcept>

namespace Stle {

ReadSignalFlags readSignalFlagsFromStr(const char *flagsStr) {
    ReadSignalFlags flags = READ_SIGNAL_DEFAULT;
    while (*flagsStr != '\0') {
        switch (*flagsStr) {
        case 'f':
            flags  = (flags  & (~READ_SIGNAL_TIME_MASK)) | READ_SIGNAL_TIME_FIRST;
            break;
            
        case 'l':
            flags  = (flags  & (~READ_SIGNAL_START_END_MASK)) | READ_SIGNAL_TIME_LAST;
            break;
            
        case 's':
            flags  = (flags  & (~READ_SIGNAL_START_END_MASK)) | READ_SIGNAL_START_POINTS;
            break;
            
        case 'e':
            flags  = (flags  & (~READ_SIGNAL_START_END_MASK)) | READ_SIGNAL_END_POINTS;
            break;
            
        case 'o':
            flags  = (flags  & (~READ_SIGNAL_OPEN_CLOSED_MASK)) | READ_SIGNAL_RIGHT_OPEN;
            break;
            
        case 'c':
            flags  = (flags  & (~READ_SIGNAL_OPEN_CLOSED_MASK)) | READ_SIGNAL_RIGHT_CLOSED;
            break;
            
        default:
            std::stringstream ss;
            ss << "Unknown format flag: '" << *flagsStr << "'.";
            throw std::runtime_error(ss.str());
        }
        ++flagsStr;
    }
    return flags ;
}

}
