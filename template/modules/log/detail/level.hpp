#ifndef __WEBCC_MODULES_LOG_DETAIL_LEVEL_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_LEVEL_HPP__

#include <string>

namespace webcc
{
namespace log
{
namespace detail
{

#define WEBCC_MODULE_LOG_LEVEL_MAP(XX)  \
    XX(trace, 0, TRACE)                 \
    XX(debug, 1, DEBUG)                 \
    XX(info,  2, INFO)                  \
    XX(warn,  3, WARN)                  \
    XX(error, 4, ERROR)                 \
    XX(fatal, 5, FATAL)                 \


enum level
{
#define XX(xx_name, xx_num, xx_text)    \
        xx_name = xx_num,
    WEBCC_MODULE_LOG_LEVEL_MAP(XX)
#undef XX
};

inline std::string level_to_string(level l, const std::string& default_value = "")
{
    switch ( l ) {
    #define XX(xx_name, xx_num, xx_text)    \
            case xx_name : { return #xx_text; }
        WEBCC_MODULE_LOG_LEVEL_MAP(XX)
    #undef XX
        default: {
            return default_value;
        }
    }
}

inline bool level_from_string(const std::string& s, level& outl)
{
#define XX(xx_name, xx_num, xx_text)    \
        if ( s == #xx_text ) { outl = xx_name; return true; }
    WEBCC_MODULE_LOG_LEVEL_MAP(XX)
#undef XX
    return false;
}

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_LEVEL_HPP__