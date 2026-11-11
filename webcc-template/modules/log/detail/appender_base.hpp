#ifndef __WEBCC_MODULES_LOG_DETAIL_APPENDER_BASE_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_APPENDER_BASE_HPP__

#include "modules/noncopyable.hpp"
#include "modules/log/detail/level.hpp"
#include "modules/log/detail/event.hpp"
#include "modules/log/detail/formatter.hpp"

namespace webcc
{
namespace log
{
namespace detail
{

class appender_base :
    private noncopyable
{
public:
    appender_base() : log_fd_(-1) {}

    virtual ~appender_base() {}

    void set_pattern(std::string pattern)
    {
        formatter_.set_pattern(std::move(pattern));
    }

    virtual void reopen() {};
    virtual void log(event& e) = 0;

protected:
    int log_fd_;
    formatter formatter_;
};

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_APPENDER_BASE_HPP__