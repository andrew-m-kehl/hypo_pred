#ifndef STLE_ARRAY_PC_SIGNAL_READER_HPP
#define STLE_ARRAY_PC_SIGNAL_READER_HPP

#include "Api.hpp"

#include "SampledPCSignalBuilder.hpp"

namespace Stle {

// TODO: Think more about the TArrayRef thing.
template<typename TArrayRef>
class ArrayPCSignalReader {
public:
    ArrayPCSignalReader(TArrayRef data, int sampleCount, int fieldCount, ReadSignalFlags flags = READ_SIGNAL_DEFAULT)
        : _data(data), _sampleCount(sampleCount), _fieldCount(fieldCount), flags(flags) {        
    }
    
    PCSignal read();    
    
private:   
    TArrayRef _data;
    int _sampleCount;
    int _fieldCount;    
public:
    ReadSignalFlags flags;
};

template <typename TArrayRef>
ArrayPCSignalReader<TArrayRef> makeArrayPCSignalReader(TArrayRef data, int sampleCount, int fieldCount, ReadSignalFlags flags = READ_SIGNAL_DEFAULT) {
    return ArrayPCSignalReader<TArrayRef>(data, sampleCount, fieldCount, flags);
}

template <typename TArrayRef>
PCSignal readPCSignalArray(TArrayRef data, int sampleCount, int fieldCount, ReadSignalFlags flags = READ_SIGNAL_DEFAULT) {
    return makeArrayPCSignalReader(data, sampleCount, fieldCount, flags).read();
}

template<typename TArrayRef>
PCSignal ArrayPCSignalReader<TArrayRef>::read() {    
    SampledPCSignalBuilder<typename std::decay<decltype(_data[0])>::type> builder(flags, _fieldCount);
    for (int i = 0; i < _sampleCount; ++i)
        builder.addLine(_data[i]);
    return builder.make();
}

}

#endif
