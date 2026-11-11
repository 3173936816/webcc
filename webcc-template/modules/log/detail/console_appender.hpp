#ifndef __WEBCC_MODULES_LOG_DETAIL_CONSOLE_APPENDER_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_CONSOLE_APPENDER_HPP__

#include <string>
#include <unistd.h>

#include "modules/log/detail/appender_base.hpp"

namespace webcc
{
namespace log
{
namespace detail
{

class console_appender :
    public appender_base
{
public:
    console_appender()
    {
        log_fd_ = STDOUT_FILENO;
    }

    void log(event& e) override
    {
        std::string line = formatter_.format(e);
        line.push_back('\n');

        const char* data = line.data();
        std::size_t left = line.size();

        while ( left > 0 ) {
            ssize_t n = ::write(log_fd_, data, left);
            if ( n <= 0 ) {
                break;
            }

            data += n;
            left -= static_cast<std::size_t>(n);
        }
    }
};

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_CONSOLE_APPENDER_HPP__