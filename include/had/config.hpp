#pragma once

#ifndef NDEBUG
    #define HAD_DEBUG
#else
    #undef HAD_DEBUG
#endif

#ifdef HAD_DEBUG
    #include <cassert>

    #define HAD_DROP_VOLATILE_COMPARISONS 
    #define HAD_LOW_DEBBUGGING_LEVEL 1
    #define HAD_MID_DEBBUGGING_LEVEL 2
    #define HAD_HIGH_DEBUGGING_LEVEL 3
    #define HAD_DEBUGGING_LEVEL 1
    #define HAD_CONTAINER_DEBUGGING_LEVEL 1
    #define HAD_ITERATOR_DEBUGGING_LEVEL 
    #define HAD_DEBUG_CHECK_OUT_OF_BOUNDS_ACCESS 1
    #define HAD_WARN_IF_MOVING_CONST_OBJECT 1
    #define HAD_ASSERT_IF(CONDITION,MESSAGE) assert(!(CONDITION),MESSAGE)
#else
#   define HAD_DEBUG_CHECK_OUT_OF_BOUNDS_ACCESS 0
#   define HAD_DEBUGGING_LEVEL 0
#   define HAD_ASSERT_IF(CONDITION,MESSAGE)
#endif