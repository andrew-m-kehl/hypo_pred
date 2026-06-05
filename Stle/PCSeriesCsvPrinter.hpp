#ifndef STLE_PC_SERIES_CSV_PRINTER_HPP
#define STLE_PC_SERIES_CSV_PRINTER_HPP

#include "Api.hpp"

#include "PCSeries.hpp"

namespace Stle {

class STLE_API PCSeriesCsvPrinter {
public:
    explicit PCSeriesCsvPrinter(const PCSeries &series) : _series(&series) {        
    }
    
    template<typename TPrinter>
    void print(TPrinter &p) const;
    
private:
    const PCSeries *_series;
};

template<typename TPrinter>
void PCSeriesCsvPrinter::print(TPrinter &p) const {
    // TODO: An option to not duplicate time.
    // Time prevTime(0);
    for (int i = 0, size = _series->size(); i < size; ++i) {
        const PCSeg &piece = _series->at(i);
        Time time = piece.endTime();        
        // if (!(i > 0 && i == size - 1 && time == prevTime)) {
            if (i > 0) 
                p << "\n";
        // }
        // prevTime = time;
    }    
}

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSeriesCsvPrinter &sp) {
    sp.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, const PCSeriesCsvPrinter &sp);

}

#endif
