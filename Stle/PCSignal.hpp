#ifndef STLE_PC_SIGNAL_HPP
#define STLE_PC_SIGNAL_HPP

#include "Api.hpp"

#include "PCSeries.hpp"
#include "ReadSignalFlags.hpp"
#include "U/FileU.hpp"

#include <vector>
#include <sstream>
#include <climits>
#include <ostream>

namespace Stle {

// NOTE: deriving from U::CannotCopy disables copy constructor and assignment to make sure an object is not copied accidentally.
// If PCSignal genuinely needs to be copied, remove this base class.
#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API PCSignal : U::CannotCopy {
public:    
    PCSignal() {        
    }
    
    explicit PCSignal(std::vector<PCSeries> &&series);
    
    explicit PCSignal(PCSeries &&series);
    
    void add(PCSignal &&other);
    
    void clear() {
        _series.clear();
    }
    
    bool empty() const {
        // TODO: More on checking for empty signals/series.
        return _series.empty();
    }
    
    int size() const {
        return static_cast<int>(_series.size());
    }
    
    const PCSeries &at(int i) const {
#ifndef NDEBUG
        if (i < 0 || i >= size())
            throw std::runtime_error("Series index out of range.");
#endif
        return _series[static_cast<size_t>(i)];        
    }
    
    const PCSeries &operator[](int i) const {
        return at(i);
    }
    
    Time endTime() const {
        return _series[0].endTime();
    }
    
    bool closed() const {
        return _series[0].closed();
    }
    
    bool operator==(const PCSignal &other) const {
        return _series == other._series;
    }
    
    bool operator!=(const PCSignal &other) const {
        return !(*this == other);
    }
    
    template<typename TPrinter>
    void print(TPrinter &printer) const;
    
    template<typename TIt>
    static bool valid(TIt begin, TIt end);
    
    static bool valid(const std::vector<PCSeries> &series);
    
private:
    std::vector<PCSeries> _series;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

class PCSignalPrinter {
public:
    explicit PCSignalPrinter(const PCSignal &signal)
        : _signal(&signal), segmentSeparator("; ") {
    }
    
    template <typename TPrinter>
    void print(TPrinter &p) const {
        for (int i = 0; i < _signal->size(); ++i) {
            if (i > 0)
                p << "\n";
            p << "Series " << i << ":\n";
            PCSeriesPrinter sp(_signal->at(i));
            sp.separator = segmentSeparator;
            p << sp;
        }
    }
    
private:
    const PCSignal *_signal;
public:
    const char *segmentSeparator;
};

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSignalPrinter &sp) {
    sp.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &p, const PCSignalPrinter &sp);

template<typename TPrinter>
void PCSignal::print(TPrinter &p) const {
    PCSignalPrinter(*this).print(p);
}

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSignal &signal) {
    signal.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &p, const PCSignal &signal);

template<typename TIt>
bool PCSignal::valid(TIt begin, TIt end) {
    auto count = std::distance(begin, end);        
    if (count == 0)
        return false;
    const PCSeries &first = *begin;
    if (first.empty())
        return false;
    Time endTime = first.endTime();
    bool closed = first.closed();
    for (TIt it = std::next(begin); it != end; ++it) {
        const PCSeries &series = *it;
        if (series.empty())
            return false;
        if (series.endTime() != endTime)
            return false;
        if (series.closed() != closed)
            return false;
    }
    return true;
}

}

#endif
