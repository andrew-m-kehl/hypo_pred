#ifdef _WIN32

#include "WinU.hpp"

#include <stdexcept>
#include <sstream>

#include <Windows.h>

namespace U {
    
int u8StrLen(const wchar_t *wStr) {
#ifndef NDEBUG
    if (!wStr)
        throw std::runtime_error("Argument is null.");
#endif        
    int len = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, nullptr, 0, nullptr, nullptr);
    if (!len) {
        std::stringstream ss;
        ss << "WideCharToMultiByte error: " << GetLastError() << ".";
        throw std::runtime_error(ss.str());
    }
    return len - 1;
}

std::unique_ptr<char[]> makeU8Str(const wchar_t *wStr) {
#ifndef NDEBUG
    if (!wStr)
        throw std::runtime_error("Argument is null.");
#endif
    int len = u8StrLen(wStr);
    std::unique_ptr<char[]> u8Str = std::make_unique<char[]>(static_cast<size_t>(len + 1));
    int result = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, u8Str.get(), len + 1, nullptr, nullptr);
    if (!result) {
        std::stringstream ss;
        ss << "WideCharToMultiByte error: " << GetLastError() << ".";
        throw std::runtime_error(ss.str());
    }
    return u8Str;
}

int wStrLen(const char *u8Str) {
#ifndef NDEBUG
    if (!u8Str)
        throw std::runtime_error("Argument is null.");
#endif
    int len = MultiByteToWideChar(CP_UTF8, 0, u8Str, -1, nullptr, 0);
    if (!len) {
        std::stringstream ss;
        ss << "MultiByteToWideChar error: " << GetLastError() << ".";
        throw std::runtime_error(ss.str());
    }
    return len - 1;
}

std::unique_ptr<wchar_t[]> makeWStr(const char *u8Str) {
#ifndef NDEBUG
    if (!u8Str)
        throw std::runtime_error("Argument is null.");
#endif
    int len = wStrLen(u8Str);
    std::unique_ptr<wchar_t[]> wStr = std::make_unique<wchar_t[]>(static_cast<size_t>(len + 1));
    int result = MultiByteToWideChar(CP_UTF8, 0, u8Str, -1, wStr.get(), len + 1);
    if (!result) {
        std::stringstream ss;
        ss << "MultiByteToWideChar error: " << GetLastError() << ".";
        throw std::runtime_error(ss.str());
    }
    return wStr;
}

std::unique_ptr<char*, ArgvDelete> makeU8Argv(int argc, wchar_t **wArgv) {
    if (argc < 0)
        return std::unique_ptr<char*, ArgvDelete>(nullptr, ArgvDelete(0));
    
    size_t sizeArgc = static_cast<size_t>(argc);
    std::unique_ptr<char*, ArgvDelete> u8Argv(new char*[sizeArgc](), ArgvDelete(sizeArgc));
    for (size_t i = 0; i < sizeArgc; ++i)
        // Here we take ownership of the pointer, it will be deleted by the custom deleter of u8Argv.
        u8Argv.get()[i] = makeU8Str(wArgv[i]).release();
    return u8Argv;
}

void ArgvDelete::operator()(char **argv) {
    if (argv) {
        for (size_t i = 0; i < _count; ++i)
            delete [] argv[i];
        delete [] argv;
    }
}

}

#endif
