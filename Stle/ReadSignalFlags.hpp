#ifndef STLE_READ_SIGNAL_FLAGS_HPP
#define STLE_READ_SIGNAL_FLAGS_HPP

#include "Api.hpp"

namespace Stle {

enum ReadSignalFlags {
    READ_SIGNAL_TIME_MASK = 0x1,
    READ_SIGNAL_START_END_MASK = 0x2,
    READ_SIGNAL_OPEN_CLOSED_MASK = 0x4,
    
    READ_SIGNAL_TIME_FIRST = 0x0,
    READ_SIGNAL_TIME_LAST = 0x1,
    
    READ_SIGNAL_START_POINTS = 0x0,
    READ_SIGNAL_END_POINTS = 0x2,    
    
    READ_SIGNAL_RIGHT_OPEN = 0x0,
    READ_SIGNAL_RIGHT_CLOSED = 0x4,
    
    READ_SIGNAL_DEFAULT = 0x0 // = READ_SIGNAL_TIME_FIRST | READ_SIGNAL_START_POINTS | READ_SIGNAL_RIGHT_OPEN
};

STLE_API inline ReadSignalFlags operator~(ReadSignalFlags f) {
    return static_cast<ReadSignalFlags>(~static_cast<int>(f));
}

STLE_API inline ReadSignalFlags operator&(ReadSignalFlags f1, ReadSignalFlags f2) {
    return static_cast<ReadSignalFlags>(static_cast<int>(f1) & static_cast<int>(f2));
}

STLE_API inline ReadSignalFlags operator|(ReadSignalFlags f1, ReadSignalFlags f2) {
    return static_cast<ReadSignalFlags>(static_cast<int>(f1) | static_cast<int>(f2));
}

STLE_API ReadSignalFlags readSignalFlagsFromStr(const char *flagsStr);

}

#endif
