#pragma once

#if defined(SS_BUILD_SHARED_LIBS) && !defined(SS_BUILD_JNI_LIBS)
#  if defined (_MSC_VER)
#    ifdef _SOGOU_API_EXPORT_
#      define __SOGOU_API__ __declspec(dllexport)
#    else
#      define __SOGOU_API__ __declspec(dllimport)
#    endif
#  else
#    define __SOGOU_API__ __attribute__ ((__visibility__("default")))
#  endif
#else
#  define __SOGOU_API__
#endif

#if __cplusplus >= 201402L || _MSC_VER >= 1900
#  define SS_DEPRECATED(msg, func) [[deprecated(msg)]] func
#else
#  if defined(__GNUC__) || defined(__clang__)
#    define SS_DEPRECATED(msg, func) func __attribute__ ((deprecated(msg)))
#  elif defined(_MSC_VER)
#    define SS_DEPRECATED(msg, func) __declspec(deprecated(msg)) func
#  else
#    define SS_DEPRECATED(msg, func) func
#  endif
#endif
