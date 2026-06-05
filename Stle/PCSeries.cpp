#include "PCSeries.hpp"

#include <ostream>

namespace Stle {

PCSeries::PCSeries(Time endTime, bool closed, Space value) {
#ifndef NDEBUG
    if (endTime < Time(0))
        throw std::runtime_error("End time should be non-negative.");
    if (endTime == Time(0) && !closed)
        throw std::runtime_error("Series that ends at time 0 should be right-closed.");
#endif
    add(endTime, value);
    if (closed && endTime != Time(0))
        add(endTime, value);
}

void PCSeries::add(Time endTime, Space value) {
    int count = size();
    
    if (count == 0) {
#ifndef NDEBUG
        if (endTime < Time(0))
            throw std::runtime_error("New interval should not end before time 0.");
        if (size() == INT_MAX)
            throw std::runtime_error("Size will become more than INT_MAX.");
#endif
        _series.emplace_back(endTime, value);
        return;
    }
    
    int lastI = count - 1;
    Time lastEnd = at(lastI).endTime();
#ifndef NDEBUG
    if (endTime < lastEnd)
        throw std::runtime_error("New interval should not end before the last one.");
#endif
    if (endTime == lastEnd) {
#ifndef NDEBUG
        if (rightClosed(lastI))
            throw std::runtime_error("New interval should end strictly after the last one because the last one is closed.");
        if (size() == INT_MAX)
            throw std::runtime_error("Size will become more than INT_MAX.");
#endif
        _series.emplace_back(endTime, value);
        return;
    }
    
    // This will not happen more than twice.
    while (lastI >= 0 && at(lastI).value() == value) {
        _series.pop_back();
        --lastI;        
    }
#ifndef NDEBUG
        if (size() == INT_MAX)
            throw std::runtime_error("Size will become more than INT_MAX");
#endif
    _series.emplace_back(endTime, value);
}

PCSeries PCSeries::backshift(const PCSeries &series, Time delay, Space emptyValue) {
#ifndef NDEBUG
    if (series.empty())
        throw std::runtime_error("Series is empty.");
#endif
    // TODO: Support past lookup.
    if (delay < Time(0))
        throw std::runtime_error("Past lookup not implemented.");
    
    PCSeries result;
    for (int i=0; i < series.size(); ++i) {
        PCSeg seg = series[i];
        Time endTime = seg.endTime() - delay;
        if (endTime < Time(0) || (endTime == Time(0) && !series.rightClosed(i)))
            continue;
        result.add(endTime, seg.value());
    }
    
    if (delay > Time(0)) {
        result.add(series.endTime(), emptyValue);
        if (series.closed())
            result.close();
    }
    
    return result;
}

PCSeries PCSeries::reverse(const PCSeries &series, bool unclose) {
#ifndef NDEBUG
    if (series.empty())
        throw std::runtime_error("Series is empty.");
#endif

    PCSeries result;
    result.reserve(series.size());
    
    Time duration = series.endTime();

    // TODO: Perhaps need some sort of iterator that would automatically maintain prevEndTime and prevPrevEndTime,
    // which are needed for closedness queries.
    int i = series.size() - 1;    
    Time prevEndTime = series.endTimeOrDefault(i - 1);
    Time prevPrevEndTime = series.endTimeOrDefault(i - 2);
    while (i >= 0) {
        PCSeg seg = series[i];
        Time reverseEndTime = duration - prevEndTime;
        
        result.add(reverseEndTime, seg.value());
        if (PCSeries::leftClosed(i, prevEndTime, prevPrevEndTime) && !PCSeries::point(i, seg.endTime(), prevEndTime))
            result.add(reverseEndTime, seg.value());
        
        --i;
        prevEndTime = prevPrevEndTime;
        prevPrevEndTime = series.endTimeOrDefault(i - 2);
    }
    
    if (unclose)
        result.unclose();
    
    return result;
}

std::ostream &operator<<(std::ostream &p, const PCSeries &series) {
    series.print(p);
    return p;
}

}
