#ifndef STLE_CSV_PC_SIGNAL_READER_HPP
#define STLE_CSV_PC_SIGNAL_READER_HPP

#include "PCSignal.hpp"
#include "SampledPCSignalBuilder.hpp"
#include "ReadSignalError.hpp"
#include "ReadSignalFlags.hpp"

#include "U/CsvToken.hpp"

namespace Stle {

template <typename TCsvReader>
class CsvPCSignalReader {
public:
    explicit CsvPCSignalReader(TCsvReader &reader, ReadSignalFlags flags = READ_SIGNAL_DEFAULT)
        : _reader(&reader), flags(flags), skipLines(0), maxLines(INT_MAX) {        
    }    
    
    PCSignal read();

private:
    TCsvReader *_reader;    
public:
    ReadSignalFlags flags;
    int skipLines;
    int maxLines;
};

template <typename TCsvReader>
CsvPCSignalReader<TCsvReader> makeCsvPCSignalReader(TCsvReader &reader) {
    return CsvPCSignalReader<TCsvReader>(reader);
}

template <typename TCsvReader>
static PCSignal readPCSignalCsv(TCsvReader &reader, ReadSignalFlags flags = READ_SIGNAL_DEFAULT) {
    auto signalReader = makeCsvPCSignalReader(reader);
    signalReader.flags = flags;
    return signalReader.read();
}

template<typename TCsvReader>
PCSignal CsvPCSignalReader<TCsvReader>::read() {    
    for (int line=0; line < skipLines; ++line) {
        bool eof = false;        
        while(true) {
            U::CsvToken token = _reader->read();
            if (token == U::CSV_EOF)
                eof = true;
            if (token == U::CSV_EOF || token == U::CSV_LINE_END)
                break;
        }        
        if (eof)
            break;
    }
    
    std::vector<double> fields1;
    std::vector<double> fields2;
    std::vector<double> *fields = &fields1;
    std::vector<double> *lastFields = &fields2;    
    SampledPCSignalBuilder<const double*> builder;
    
    // Default-constructed builder has line 0, so this condition is fine.
    while (builder.line() < maxLines) {
        U::CsvToken token = U::CSV_NONE;
        while (true) {
            token = _reader->read();
            if (token == U::CSV_EOF || token == U::CSV_LINE_END)
                break;            
            
            char *fieldEnd = nullptr;
            const char *field = _reader->str().c_str();
            // TODO: strtod and atof are very slow.
            double value = strtod(field, &fieldEnd);
            if (fieldEnd == field) {
                builder.throwError("Not a valid number.", builder.line() + 1, static_cast<int>(fields->size()) + 1);                
            }
            fields->emplace_back(value);
        }
        
        if (fields->size() == 0) {
            if (token == U::CSV_LINE_END) {
                // For now, ignore empty lines. Maybe do something else later.
                continue;
            }
            else // if (token == U::CSV_EOF)
                break;
        }
        
        // Here, fields has at least one element.
        // Default-constructed builder has line 0, so this condition is fine.
        if (builder.line() == 0) {
#ifndef NDEBUG
            if (fields->size() > INT_MAX)
                throw std::runtime_error("Number of columns should not be greater than INT_MAX.");
#endif
            // Constructor will throw if there's too few columns.
            builder = SampledPCSignalBuilder<const double*>(flags, static_cast<int>(fields->size()));            
        }
        else if (static_cast<int>(fields->size()) != builder.columns())  {
            builder.throwError("Every line should have the same number of columns.", builder.line() + 1);            
        }
        
        builder.addLine(fields->data());
        
        std::swap(lastFields, fields);
        fields->clear();        
    }
    
    return builder.make();
}

}

#endif
