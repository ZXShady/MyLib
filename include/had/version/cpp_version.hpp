#pragma once

#ifndef HAD_VERSION_CPP_VERSION_HPP
#define HAD_VERSION_CPP_VERSION_HPP 1

#ifdef _MSC_VER 
    #define HAD_CPP_VER _MSVC_LANG 
#else
    #define HAD_CPP_VER __cplusplus
#endif

#if HAD_CPP_VER >=  201103L
    #define HAD_CPP11
#if HAD_CPP_VER == 201103L  
#define HAD_CPP_CURRENT_VERSION 11
#endif

#endif

#if HAD_CPP_VER >=  201402L
    #define HAD_CPP14
    
    #if HAD_CPP_VER == 201402L  
    #define HAD_CPP_CURRENT_VERSION 14
    #endif

#endif

#if HAD_CPP_VER >= 201703L
    #define HAD_CPP17
    
    #if HAD_CPP_VER == 201703L  
    #define HAD_CPP_CURRENT_VERSION 17
    #endif


#endif

#if HAD_CPP_VER >= 202002L
    #define HAD_CPP20

    #if HAD_CPP_VER == 202002L  
    #define HAD_CPP_CURRENT_VERSION 20
    #endif
    
#endif

#if HAD_CPP_VER >= 202302L
    #define HAD_CPP23

    #if HAD_CPP_VER == 202302L  
    #define HAD_CPP_CURRENT_VERSION 23
    #endif
#endif

#endif // !HAD_VERSION_CPP_VERSION_HPP