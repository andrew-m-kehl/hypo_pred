#ifndef STLE_PC_SERIES_HPP
#define STLE_PC_SERIES_HPP

#include "Api.hpp"

// PCSeries is declared in PCSeriesForward.
// Required to resolve the dependency with PCSeriesPrinter.
#include "PCSeriesForward.hpp"

#include "PCSeriesPrinter.hpp"

#include <vector>
#include <algorithm>
#include <stdexcept>

namespace Stle {

template<typename TPrinter>
void PCSeries::print(TPrinter &p) const {
    PCSeriesPrinter(*this).print(p);
}

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSeries &series) {
    series.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &p, const PCSeries &series);

template<typename TOp>
PCSeries PCSeries::apply1(TOp op, const PCSeries &series) {
    PCSeries result;
    result.reserve(series.size());
    
    for (int i = 0, count = series.size(); i < count; ++i) {
        const PCSeg &piece = series[i];
        result.add(piece.endTime(), op(piece.value()));
    }            
    
    return result;
}

template<typename TOp>
PCSeries PCSeries::apply2(TOp op, const PCSeries &series1, const PCSeries &series2) {
    PCSeries result;
    if (series1.empty() && series2.empty())
        return result;
    
    if (series1.empty() || series2.empty())    
        throw std::runtime_error("Series should have the same end time and be both open or both closed.");
    if (series1.endTime() != series2.endTime() || series1.closed() != series2.closed())
        throw std::runtime_error("Series should have the same end time and be both open or both closed.");
    
    result.reserve(std::max(series1.size(), series2.size()));
    
    // At this point both series are not empty, have the same end-time, and are both open or both closed.
    int i1 = 0;
    int count1 = series1.size();
    PCSeg piece1 = series1.at(i1);
    int i2 = 0;
#ifndef NDEBUG
    int count2 = series2.size();
#endif
    PCSeg piece2 = series2.at(i2);
    while (true) {        
        if (piece1.endTime() < piece2.endTime()) {
            result.add(piece1.endTime(), op(piece1.value(), piece2.value()));
            ++i1;
            piece1 = series1.at(i1);
        } else if (piece1.endTime() > piece2.endTime()) {
            result.add(piece2.endTime(), op(piece1.value(), piece2.value()));
            ++i2;
            piece2 = series2.at(i2);
        } else { // if (piece1.endTime() == piece2.endTime())
            result.add(piece1.endTime(), op(piece1.value(), piece2.value()));
            ++i1;
            ++i2;
            if (i1 >= count1) // || i2 >= count2; they should go out of bounds at the same time.
                break;                
            piece1 = series1.at(i1);
            piece2 = series2.at(i2);
        }
    }
#ifndef NDEBUG
    if (i1 < count1 || i2 < count2)
        throw std::runtime_error("Both i1 and i2 should go out of bounds at the same time.");
#endif            
    
    return result;
}

// TODO: Perhaps have apply3 for ite.

template<typename TOp, typename TIt>
PCSeries PCSeries::applyR(TOp op, TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count == 0)
        // NOTE: At this point it is not valid to apply an operation to an empty argument list,
        // since we don't know the duration and closedness of the result, even if 'op' can take 0 arguments.
        throw std::runtime_error("Range of arguments is empty.");
    TIt second = std::next(begin);
    
    PCSeries result;
    
    bool isEmpty = (*begin)->empty();    
    for (TIt it = second; it != end; ++it) {
        if ((*it)->empty() != isEmpty)
            throw std::runtime_error("Series should have the same end time and be both open or both closed.");
    }     
    if (isEmpty)
        return result;
    
    Time endTime = (*begin)->endTime();
    bool closed = (*begin)->closed();    
    int maxSize = (*begin)->size();
    for (TIt it = second; it != end; ++it) {
        if ((*it)->endTime() != endTime)
            throw std::runtime_error("Series should have the same end time and be both open or both closed.");
        if ((*it)->closed() != closed)
            throw std::runtime_error("Series should have the same end time and be both open or both closed.");
        maxSize = std::max(maxSize, (*it)->size());
    }

    // Just a guess how many segments the result can reasonably have.
    result.reserve(maxSize);    
    
    std::vector<int> indices(count, 0);
    std::vector<Space> opArgs(count, Space(0));
    
    bool done = false;
    while (!done) {
        PCSeg beginPiece = (*begin)->at(indices[0]);
        Time minEndTime = beginPiece.endTime();
        opArgs[0] = beginPiece.value();
        int i=1;
        for (TIt it = second; it != end; ++it, ++i) {
            PCSeg piece = (*it)->at(indices[i]);
            minEndTime = std::min(minEndTime, piece.endTime());
            opArgs[static_cast<size_t>(i)] = piece.value();
        }
        result.add(minEndTime, op(opArgs.begin(), opArgs.end()));
        i = 0;        
        for (TIt it = begin; it != end; ++it, ++i) {
            if ((*it)->at(indices[i]).endTime() == minEndTime) {
                ++indices[static_cast<size_t>(i)];
                if (indices[i] >= (*it)->size()) {
                    done = true;
#ifndef NDEBUG
                    if (i != 0)
                        throw std::runtime_error("All indices should go out of bounds at the same time.");
                    int j = 1;
                    for (TIt jt = second; jt != end; ++jt, ++j) {
                        if ((*jt)->at(indices[j]).endTime() != minEndTime)
                            throw std::runtime_error("All indices should go out of bounds at the same time.");
                        if (indices[j] != (*jt)->size() - 1)
                            throw std::runtime_error("All indices should go out of bounds at the same time.");
                    }
#endif
                    break;
                }                
            }
        }
    }
       
    return result;
}

template<typename TInit, typename TOp>
PCSeries PCSeries::backshift(const PCSeries &series, ClosedIntvl intvl, TInit init, TOp op, Space emptyValue) {
#ifndef NDEBUG
        if (series.size() == 0)
            throw std::length_error("Series is empty.");
#endif
        
    Time minShift = intvl.start();
    Time maxShift = intvl.end();
    
    if (minShift == maxShift)
        return backshift(series, minShift, emptyValue);
        
    if (minShift < Time(0))
        // NOTE: Now past operators are implemented via reversing the input.
        throw std::runtime_error("Lower bound should not be less than 0.");
    
    PCSeries result;
    result.reserve(series.size());

    Time prevEnd(0);
    Time prevPrevEnd(0);
    for (int i = 0; i < series.size(); ++i) {
        PCSeg seg = series[i];
        Time end = seg.endTime();        
        backshiftAdd(result, prevEnd - maxShift, PCSeries::leftClosed(i, prevEnd, prevPrevEnd), end - minShift, PCSeries::rightClosed(i, end, prevEnd), init(seg.value()), op);
        prevPrevEnd = prevEnd;
        prevEnd = end;        
    }
    
    if (minShift > Time(0)) {        
        result.add(series.endTime(), emptyValue);
        if (series.closed())
            result.close();
    }
    
    return result;
}

template<typename TOp>
void PCSeries::backshiftAdd(PCSeries &series, Time start, bool leftClosed, Time end, bool rightClosed, Space value, TOp op) {
#ifndef NDEBUG
    if (start > end || (start == end && (!leftClosed || !rightClosed)))
        throw std::runtime_error("The new interval should not be empty.");
#endif
    if (end < Time(0) || (end == Time(0) && !rightClosed))
        return;
    if (start < Time(0)) {
        start = Time(0);
        leftClosed = true;
    }
    
    if (series.empty()) {
#ifndef NDEBUG
        if (!(start == Time(0) && leftClosed))    
            throw std::runtime_error("The first interval should be left-closed and start at 0.");
#endif
        series.add(end, value);
        if (rightClosed && start != end)
            series.add(end, value);
        return;
    }

#ifndef NDEBUG
    Time seriesEnd = series.endTime();
    bool seriesClosed = series.closed();
    if (!(end > seriesEnd || (end == seriesEnd && (!seriesClosed && rightClosed))))
        throw std::runtime_error("New interval should end strictly after the current series.");
    if (start == seriesEnd && !(seriesClosed && leftClosed))
        // This function will not work correctly if the added interval does not intersect with
        // the last segment.
        throw std::runtime_error("Should be unreachable. Lookup/delay is implemented separately.");
#endif
    
    // Now modifying the existing segments. The new segment will be added later.
    // If the added segment is left-open and its start falls into a right-open segment,
    // this may create a new closing point.
    // But if the added segment is left-open, then the last added one was right closed, so the extra
    // point will be added only if at least one segment was discarded.
    // This means that when modifying the existing segments, the size of the series will not increase.
    std::vector<PCSeg> &segments = series._series;    
    // Indices [readI+1, end) will be considered unoccupied and will be erased in the end.
    int readI = series.size() - 1;
    // Since delay is handled elsewhere, the new segment has overlap with at least the last existing
    // segment, so the loop should do at least one iteration.
    // TODO: Maybe make this work correctly for delay/lookup as well.
    while (readI >= 0) {
        Space valueI = series[readI].value();
        
        if (op(valueI, value) != value)
            break;        
        
        Time startI = series.startTime(readI);
        bool leftClosedI = series.leftClosed(readI);            
        
        // If new interval starts when or after the current segment starts,
        // need to perform the finishing sequence.
        if (start > startI || (start == startI && ((!leftClosed && leftClosedI) || (leftClosed == leftClosedI)))) {
            // If the new interval starts strictly after the current segment starts, need to split the current segment.
            if (start > startI || (start == startI && (!leftClosed && leftClosedI))) {
                segments[static_cast<size_t>(readI)] = PCSeg(start, valueI);
                // If the new segment erval is left-open, need to right-close the current segment:
                // if it became a point, it is already right-closed, otherwise need to add a closing segment after it.
                if (!leftClosed && start != startI) {
                    // This requires that readI was decremented at least once, so the closing point takes the place
                    // of a discarded segment.
#ifndef NDEBUG
                    if (readI == series.size() - 1)
                        throw std::runtime_error("No place to a add a closing segment.");
#endif
                    ++readI;
                    segments[static_cast<size_t>(readI)] = PCSeg(start, valueI);
                }
            } else {
                // If the new segment starts when the current segment starts,
                // the current segment has to be erased, but after that we're done.
                --readI;
            }
            // Making sure that there are no adjacent segments with same value as the new segment.
            // The loop body may execute once or twice.
            while (readI >= 0 && segments[static_cast<size_t>(readI)].value() == value)
                --readI;            
            break;
        }

        --readI;        
    }
    
    // Erasing the segments that are to be discarded.
    segments.erase(std::next(segments.begin(), readI+1), segments.end());
#ifndef NDEBUG
        if (segments.size() == INT_MAX)
            throw std::runtime_error("Size will become more than INT_MAX");
#endif
    // Adding the new segment.
    segments.emplace_back(end, value);
    if (rightClosed)
        series.close();
}
    
}

#endif
