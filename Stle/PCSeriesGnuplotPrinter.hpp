#ifndef STLE_PC_SERIES_GNUPLOT_PRINTER_HPP
#define STLE_PC_SERIES_GNUPLOT_PRINTER_HPP

#include "Api.hpp"

#include "PCSeries.hpp"

namespace Stle {

class STLE_API PCSeriesGnuplotPrinter {
public:
    explicit PCSeriesGnuplotPrinter(const PCSeries &series) : _series(&series), name(nullptr) {        
    }
    
    template<typename TPrinter>
    void print(TPrinter &p) const;
    
private:
    const PCSeries *_series;
    
public:
    const char *name;
};

template<typename TPrinter>
void PCSeriesGnuplotPrinter::print(TPrinter &p) const {
    p << "\"" << (name ? name : "Series") << "\"\n";
    Time seriesEndTime = _series->endTime();
    {    
        Time startTime = Time(0);        
        for (int i = 0, size = _series->size(); i < size; ++i) {
            const PCSeg &seg = _series->at(i);
            Time endTime = seg.endTime();
            Space value = seg.value();
            if (endTime > startTime) {
                p << startTime << " " << value << "\n";
                if (endTime == seriesEndTime)
                    p << endTime << " " << value << "\n";
            }
            startTime = endTime;
        }
    }    
    p << "\n\n\"" << (name ? name : "Series") << " points\"\n";
    {
        Time startTime = Time(0);
        for (int i = 0, size = _series->size(); i < size; ++i) {
            const PCSeg &seg = _series->at(i);
            Time endTime = seg.endTime();
            Space value = seg.value();
            if (_series->leftClosed(i))
                p << startTime << " " << value << "\n";
            if (i == size - 1 && endTime > startTime)
                p << endTime << " " << value << "\n";
            startTime = endTime;
        }
    }
}

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSeriesGnuplotPrinter &sp) {
    sp.print(p);
    return p;
}

inline std::ostream &operator<<(std::ostream &p, const PCSeriesGnuplotPrinter &sp) {
    sp.print(p);
    return p;
}

}

#endif
