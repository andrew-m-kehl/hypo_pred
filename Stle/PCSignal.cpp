#include "PCSignal.hpp"

#include "CsvPCSignalReader.hpp"

namespace Stle {

PCSignal::PCSignal(std::vector<PCSeries> &&series) : _series(std::move(series)) {
    if (!valid(_series))
        throw std::runtime_error("Argument does not represent a valid signal.");
}

PCSignal::PCSignal(PCSeries &&series) {
    _series.emplace_back(std::move(series));
    if (!valid(_series))
        throw std::runtime_error("Argument does not represent a valid signal.");
}

void PCSignal::add(PCSignal &&other) {
    if (empty()) {
        *this = std::move(other);
        return;
    }
    if (other.empty())
        throw std::runtime_error("Signal to append is empty.");
    if (other.endTime() != endTime() || other.closed() != closed())
        throw std::runtime_error("Signal to append should have the same end time and right-openness.");
    for (int i=0, size = other.size(); i < size; ++i)
        _series.emplace_back(std::move(other._series[i]));
    other.clear();
}

bool PCSignal::valid(const std::vector<PCSeries> &series) {
    return valid(series.begin(), series.end());
}

std::ostream &operator<<(std::ostream &p, const PCSignalPrinter &sp) {
    sp.print(p);
    return p;
}

std::ostream &operator<<(std::ostream &p, const PCSignal &signal) {
    signal.print(p);
    return p;
}

}
