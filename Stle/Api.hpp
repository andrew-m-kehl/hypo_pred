#ifndef STLE_API_HPP
#define STLE_API_HPP

#ifdef _WIN32
    #if defined(STLE_COMPILE_DLL)
        #define STLE_API __declspec(dllexport)
    #elif defined(STLE_USE_DLL)    
        #define STLE_API __declspec(dllimport)
    #else
        #define STLE_API
    #endif
#else
    #define STLE_API
#endif

#endif
