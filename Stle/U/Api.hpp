#ifndef U_API_HPP
#define U_API_HPP

#ifdef _WIN32
    #if defined(STLE_COMPILE_DLL)
        #define U_API __declspec(dllexport)
    #elif defined(STLE_USE_DLL)    
        #define U_API __declspec(dllimport)
    #else
        #define U_API
    #endif
#else
    #define U_API
#endif

#endif
