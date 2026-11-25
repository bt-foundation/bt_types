#pragma once

#ifndef BT_ID_TYPE
    #include "cstdint.hpp"
    #define BT_ID_TYPE bt::types::uint64_t
#endif

#ifdef BT_DISABLE_ASSERT
    #undef BT_ASSERT
    #define BT_ASSERT(condition, msg) (void(0))
#elif !defined BT_ASSERT
    #include "cassert.hpp"
    #define BT_ASSERT(condition, msg) assert(((condition) && (msg)))
#endif

#define BT_FAIL(msg) BT_ASSERT(false, msg)

#if defined __clang__ || defined __GNUC__
    #define BT_PRETTY_FUNCTION __PRETTY_FUNCTION__
    #define BT_PRETTY_FUNCTION_PREFIX '='
    #define BT_PRETTY_FUNCTION_SUFFIX ']'
#elif defined _MSC_VER
    #define BT_PRETTY_FUNCTION __FUNCSIG__
    #define BT_PRETTY_FUNCTION_PREFIX '<'
    #define BT_PRETTY_FUNCTION_SUFFIX '>'
#endif

using ID = BT_ID_TYPE;