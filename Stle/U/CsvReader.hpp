#ifndef U_CSV_READER_HPP
#define U_CSV_READER_HPP

#include "Api.hpp"

#include "ReaderRef.hpp"
#include "CsvToken.hpp"
#include "OfReader.hpp"

#include <utility>
#include <vector>
#include <climits>
#include <stdexcept>

namespace U {

// TODO: Perhaps skip '\r' so that it can accept data read in binary mode.
template <typename TReader>
class CsvReader {
public:
    template<typename... TArgs>
    explicit CsvReader(OfReaderTag, TArgs&&... args)
        : _reader(std::forward<TArgs>(args)...), _state(STATE_NONE), _token(CSV_NONE), fieldSep(','), lineSep('\n') {
    }
    
    int len() const {
#ifndef NDEBUG
        if (_str.size() > INT_MAX)
            throw std::runtime_error("Buffer size is greater than INT_MAX.");
#endif
        return static_cast<int>(_str.size());
    }
    
    const std::string &str() const {
        return _str;
    }
    
    CsvToken token() const {
        return _token;
    }
    
    CsvToken read() {
        _str.clear();
        
        switch (_state) {
        case STATE_EOF: {
            _token = CSV_EOF;
            return CSV_EOF;
        }
            
        case STATE_LINE_END: {
            _state = STATE_NONE;
            _token = CSV_LINE_END;
            return CSV_LINE_END;
        }
            
        case STATE_NONE: {
            int c = _reader.getc();
            if (c == EOF) {
                _token = CSV_EOF;
                return _token;
            }
            else if (c == fieldSep) {
                _token = CSV_FIELD;
                return _token;
            }
            else if (c == lineSep) {
                _token = CSV_LINE_END;
                return _token;
            }
            _state = STATE_FIELD;
            _token = CSV_FIELD;
            _str.push_back(static_cast<char>(c));            
        }
            // fall through
            
        case STATE_FIELD: {
            while(true) {
                int c = _reader.getc();
                
                if (c == EOF) {
                   _state = STATE_EOF;                                           
                   break;
                }                
                else if (c == fieldSep) {
                    break;
                }
                else if (c == lineSep) {
                    _state = STATE_LINE_END;
                    break;
                }                
                _str.push_back(static_cast<char>(c));
            }            
            return _token;
        }
            
        default:
            throw std::runtime_error("Invalid CSV reader state.");
        }
    }
      
private:   
    enum State {
        STATE_NONE,
        STATE_FIELD,
        STATE_LINE_END,
        STATE_EOF
    };
    
    TReader _reader;
    std::string _str;
    State _state;
    CsvToken _token;
    
public:
    char fieldSep;
    char lineSep;
};

}

#endif
