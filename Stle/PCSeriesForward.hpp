#ifndef STLE_PC_SERIES_FORWARD_HPP
#define STLE_PC_SERIES_FORWARD_HPP

#include "Api.hpp"

// Forward declaration of PCSeries class.
// Required to resolve the dependency with PCSeriesPrinter.
// Include PCSeries.hpp in when using PCSeries class.

#include "SpaceTime.hpp"
#include "PCSeg.hpp"
#include "ClosedIntvl.hpp"

#include "U/MemoryU.hpp"

#include <vector>

namespace Stle {

// Deriving from U::CannotCopy disables copy constructor and assignment to make sure an object is not copied accidentally.
// If PCSeries genuinely needs to be copied, remove this base class.
// Every method that accesses _series should check series state and its arguments in debug mode.
#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API PCSeries : U::CannotCopy {
public:
    PCSeries() {
    }

    PCSeries(Time endTime, bool closed, Space value);

    PCSeries(int endTime, bool closed, int value) : PCSeries(Time(endTime), closed, Space(value)) {
    }

    explicit PCSeries(std::vector<PCSeg> &&segments) : _series(std::move(segments)) {
        // TODO: Have an option to validate the series after construction.
    }

    Time endTime() const {
#ifndef NDEBUG
        if (empty())
            throw std::runtime_error("Series is empty.");
#endif
        return _series[_series.size() - 1].endTime();
    }

    bool closed() const {
        return rightClosed(size() - 1);
    }

    void close() {
#ifndef NDEBUG
        if (empty())
            throw std::runtime_error("Series is empty.");
#endif
        if (!closed()) {
            PCSeg last = _series[_series.size() - 1];
            add(last.endTime(), last.value());
        }
    }
    
    void unclose() {
#ifndef NDEBUG
        if (empty())
            throw std::runtime_error("Series is empty.");
#endif
        if (closed())
            _series.erase(std::next(_series.begin(), size()-1));        
    }

    // NOTE: Prefer static version when looping over segments.
    bool leftClosed(int i) const {
        return i == 0 || (i > 0 && !rightClosed(i - 1));
    }

    static bool leftClosed(int i, Time prevEndTime, Time prevPrevEndTime) {
        return i == 0 || (i > 0 && !rightClosed(i - 1, prevEndTime, prevPrevEndTime));
    }
    
    // NOTE: Prefer static version when looping over segments.
    bool rightClosed(int i) const {
        return (i == 0 && at(i).endTime() == Time(0)) || (i > 0 && at(i).endTime() == at(i - 1).endTime());
    }

    static bool rightClosed(int i, Time endTimeI, Time prevEndTime) {
        return (i == 0 && endTimeI == Time(0)) || (i > 0 && endTimeI == prevEndTime);
    }

    // NOTE: Prefer static version when looping over segments.
    bool point(int i) const {
        // Only points can be right-closed.
        return rightClosed(i);
    }
    
    static bool point(int i, Time endTimeI, Time prevEndTime) {
        return rightClosed(i, endTimeI, prevEndTime);
    }

    Time startTime(int i) const {
        if (i == 0)
            return Time(0);
        return at(i-1).endTime();
    }

    int size() const {
        return static_cast<int>(_series.size());
    }

    bool empty() const {
        return size() == 0;
    }

    PCSeg &at(int i) {
#ifndef NDEBUG
        if (i < 0 || i >= size())
            throw std::runtime_error("Piece index out of range.");
#endif
        return _series[static_cast<size_t>(i)];
    }

    const PCSeg &at(int i) const {
        return const_cast<PCSeries*>(this)->at(i);
    }

    PCSeg &operator[](int i) {
        return at(i);
    }

    const PCSeg &operator[](int i) const {
        return at(i);
    }

    PCSeg atOrDefault(int i) const {
        if (i >= 0 && i < size())
            return at(i);
        else
            return PCSeg{};
    }
    
    Time endTimeOrDefault(int i) const {
        if (i >= 0 && i < size())
            return at(i).endTime();
        else
            return Time(0);
    }

    void reserve(int size) {
#ifndef NDEBUG
        if (size < 0)
            throw std::runtime_error("Size should not be negative.");
#endif
        _series.reserve(static_cast<size_t>(size));
    }

    void add(Time endTime, Space value);

    void add(int endTime, int value) {
        add(Time(endTime), Space(value));
    }

    void add(int endTime, double value) {
        add(Time(endTime), Space(value));
    }

    bool operator==(const PCSeries &other) const {
        return _series == other._series;
    }

    bool operator!=(const PCSeries &other) const {
        return !(*this == other);
    }

    template<typename TPrinter>
    void print(TPrinter &printer) const;

    template<typename TOp>
    static PCSeries apply1(TOp op, const PCSeries &series);

    template<typename TOp>
    static PCSeries apply2(TOp op, const PCSeries &series1, const PCSeries &series2);

    template<typename TOp, typename TIt>
    static PCSeries applyR(TOp op, TIt begin, TIt end);

    // Used by tests.
    template<typename TOp>
    static PCSeries applyI(TOp op, std::initializer_list<const PCSeries*> seriesArgs) {
        return applyR(op, seriesArgs.begin(), seriesArgs.end());
    }

    // These could be non-static members, but now there's some consistency. 
    // Non-static modify the instance, static return a new instance.    
    static PCSeries backshift(const PCSeries &series, Time delay, Space emptyValue);

    template<typename TInit, typename TOp>
    static PCSeries backshift(const PCSeries &series, ClosedIntvl intvl, TInit init, TOp op, Space emptyValue);

    template<typename TOp>
    static PCSeries backshift(const PCSeries &series, ClosedIntvl intvl, TOp op, Space emptyValue) {
        return backshift(series, intvl, SpaceId{}, op, emptyValue);
    }

    // NOTE: The result is always closed, because a series has to start at time 0 inclusive.
    static PCSeries reverse(const PCSeries &series, bool unclose = false);

private:
    template<typename TOp>
    static void backshiftAdd(PCSeries &series, Time start, bool leftClosed, Time end, bool rightClosed, Space value, TOp op2);

    std::vector<PCSeg> _series;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
