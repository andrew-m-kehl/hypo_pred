#ifndef STLE_SAMPLED_PC_SIGNAL_BUILDER_HPP
#define STLE_SAMPLED_PC_SIGNAL_BUILDER_HPP

#include "Api.hpp"

#include "PCSeries.hpp"
#include "PCSignal.hpp"
#include "SpaceTime.hpp"
#include "ReadSignalFlags.hpp"
#include "ReadSignalError.hpp"

#include "U/StrPrinter.hpp"
#include "U/MemoryU.hpp"

#include <vector>

namespace Stle {

// TODO: Think more about the TArrayRef thing.
template <typename TArrayRef>
class SampledPCSignalBuilder : U::CannotCopy {
public:
    SampledPCSignalBuilder()
        : _valid(false),
          _timeFlag(READ_SIGNAL_DEFAULT & READ_SIGNAL_TIME_MASK), _pointsFlag(READ_SIGNAL_DEFAULT & READ_SIGNAL_START_END_MASK),
          _openClosedFlag(READ_SIGNAL_DEFAULT & READ_SIGNAL_OPEN_CLOSED_MASK),
          _columns(0), _timeI(0), _firstFieldI(0), _lastFieldI(0), _lastLastTime(0), _lastTime(0), _line(0),
          _lastFields() {        
    }
    
    SampledPCSignalBuilder(ReadSignalFlags flags, int columns)
        : _valid(true),
          _timeFlag(flags & READ_SIGNAL_TIME_MASK), _pointsFlag(flags & READ_SIGNAL_START_END_MASK),
          _openClosedFlag(flags & READ_SIGNAL_OPEN_CLOSED_MASK),
          _columns(columns), _timeI(0), _firstFieldI(0), _lastFieldI(0), _lastLastTime(0), _lastTime(0), _line(0),
          _lastFields() {      
        if (columns < 2) {
            throwError("There should be two or more columns.", _line);
        }
        if (_timeFlag == READ_SIGNAL_TIME_FIRST) {
            _timeI = 0;
            _firstFieldI = 1;
            _lastFieldI = columns - 1;
        }
        else { // if (_timeFlag == SIGNAL_READ_TIME_LAST)
            _timeI = columns - 1;
            _firstFieldI = 0;
            _lastFieldI = columns - 2;
        }
    }
    
    int line() const {
        return _line;
    }
    
    int columns() const {
        return _columns;
    }
    
    void addLine(TArrayRef fields);
    PCSignal make();    
    void throwError(const char *msg, int _line = -1, int column = -1) const;

private:    
    void addSegments(Time endTime, TArrayRef fields, bool twice = false);
    
    bool _valid;
    std::vector<PCSeries> _series;
    ReadSignalFlags _timeFlag;
    ReadSignalFlags _pointsFlag;
    ReadSignalFlags _openClosedFlag;
    int _columns;
    int _timeI;
    int _firstFieldI;
    int _lastFieldI;
    double _lastLastTime;
    double _lastTime;
    int _line;
    // TODO: If TFields is a pointer itself, maybe find a way to store it directly.
    TArrayRef _lastFields;
};

template <typename TArrayRef>
void SampledPCSignalBuilder<TArrayRef>::addLine(TArrayRef fields) {
    if (!_valid)
        throw std::runtime_error("Builder object is not valid.");
    
    double time(fields[_timeI]);
    if (time < 0)
        throwError("Sample time should be non-negative.", _line + 1);        
    if (time < _lastTime)
        throwError("Sample time should be strictly greater than the previous one.", _line + 1);        
    // Right-open end-point mode corresponds to the internal format and allows to have two (but not more) consecutive samples with the same time.
    if (_line > 0 && time == _lastTime && (_pointsFlag != READ_SIGNAL_END_POINTS || _openClosedFlag != READ_SIGNAL_RIGHT_OPEN))
        throwError("Sample time should be strictly greater than the previous one.", _line + 1);        
    if (_line > 0 && time == _lastTime && _lastTime == _lastLastTime && (_pointsFlag == READ_SIGNAL_END_POINTS && _openClosedFlag == READ_SIGNAL_RIGHT_OPEN))
        throwError("Right-open end-point mode allows at most two consecutive samples with the same time and at most one sample with time 0.", _line + 1);
    if (_line == 0 && time != 0 && _pointsFlag == READ_SIGNAL_START_POINTS)
        throwError("In start-point mode, first sample should have time 0.", _line + 1);
    
    if (_line == 0) {
         for (int i = 1, size = _columns; i < size; ++i)
             _series.emplace_back();
    }
    
    // In start point mode, the first sample has time 0 and it will be added to the series, but it will be deleted when adding the second sample.        
    TArrayRef segmentFields = (_line == 0 || _pointsFlag == READ_SIGNAL_END_POINTS) ? fields : _lastFields;
    addSegments(Time(time), segmentFields, _openClosedFlag == READ_SIGNAL_RIGHT_CLOSED && time > 0);    
            
    _lastLastTime = _lastTime;
    _lastTime = time;
    _lastFields = fields;
    ++_line;
}

template <typename TNumber>
PCSignal SampledPCSignalBuilder<TNumber>::make() {
    if (!_valid)
        throw std::runtime_error("Builder object is not valid.");
    
    if (_series.empty())
        throw ReadSignalError("No samples to read.");
    
    if (_openClosedFlag == READ_SIGNAL_RIGHT_OPEN) {
        if (_pointsFlag == READ_SIGNAL_START_POINTS && _lastTime > 0)
            addSegments(Time(_lastTime), _lastFields, false);
        else {
            for (int i = 0, size = static_cast<int>(_series.size()); i < size; ++i)
                _series[i].close();
        }
    }
    
    // If something goes wrong below, there's no way to fix it.
    _valid = false;
    
    if (!PCSignal::valid(_series.begin(), _series.end()))
        throwError("Not a valid signal.");
    
    return PCSignal(std::move(_series));
}

template <typename TArrayRef>
void SampledPCSignalBuilder<TArrayRef>::addSegments(Time endTime, TArrayRef fields, bool twice) {
    for (int fieldI = _firstFieldI, seriesI = 0; fieldI <= _lastFieldI; ++fieldI, ++seriesI) {
        Space value(fields[fieldI]);                
        _series[seriesI].add(endTime, value);
        if (twice)
            _series[seriesI].add(endTime, value);             
    }
}

template <typename TArrayRef>
void SampledPCSignalBuilder<TArrayRef>::throwError(const char *msg, int line, int column) const {
    U::StrPrinter p;
    if (line != -1 && column != -1) {
        p.printf("Line %d column %d: %s", line, column, msg);
        throw ReadSignalError(p.str());
    }
    else if (line != -1) {
        p.printf("Line %d: %s", line, msg);
        throw ReadSignalError(p.str());
    }
    else {
        throw ReadSignalError(msg);
    }
}

}

#endif
