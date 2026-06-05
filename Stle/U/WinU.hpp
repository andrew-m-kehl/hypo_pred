#ifndef U_WIN_U_HPP
#define U_WIN_U_HPP

#ifdef _WIN32

#include "Api.hpp"

#include <memory>

namespace U {

U_API int u8StrLen(const wchar_t *wStr);

U_API std::unique_ptr<char[]> makeU8Str(const wchar_t *wStr);

U_API int wStrLen(const char *u8Str);

U_API std::unique_ptr<wchar_t[]> makeWStr(const char *u8Str);

class U_API ArgvDelete {
public:
    explicit ArgvDelete(size_t count): _count(count) {        
    }
    
    void operator()(char **argv);
    
private:
    size_t _count;
};

U_API std::unique_ptr<char*, ArgvDelete> makeU8Argv(int argc, wchar_t **wArgv);

}

#endif

#endif
