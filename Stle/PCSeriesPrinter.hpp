#ifndef STLE_PC_SERIES_PRINTER_HPP
#define STLE_PC_SERIES_PRINTER_HPP

#include "Api.hpp"

#include "PCSeriesForward.hpp"

namespace Stle {

class STLE_API PCSeriesPrinter {
public:
    explicit PCSeriesPrinter(const PCSeries &series) : _series(&series), separator("; ") {        
    }
    
    template<typename TPrinter>
    void print(TPrinter &p) const;
    
private:
    const PCSeries *_series;
public:
    const char *separator; 
};

template<typename TPrinter>
void PCSeriesPrinter::print(TPrinter &p) const {
    if (_series->empty())
        p << "empty";
    else {            
        Time prevEndTime = Time(0);
        for (int i = 0, count = _series->size(); i < count; ++i) {
            const PCSeg &piece = _series->at(i);
            if (i > 0)
                p << separator;
            if (piece.endTime() != prevEndTime) {
                p << ((_series->leftClosed(i)) ? "[" : "(")
                  << prevEndTime << ", " << piece.endTime()
                  << ((_series->rightClosed(i)) ? "]" : ")");
            } else
                p << "[" << piece.endTime() << "]";
            p << " -> " << piece.value();
            prevEndTime = piece.endTime();
        }
    }
}

template<typename TPrinter>
TPrinter &operator<<(TPrinter &p, const PCSeriesPrinter &sp) {
    sp.print(p);
    return p;
}

std::ostream &operator<<(std::ostream &p, const PCSeriesPrinter &sp);

}

#endif
