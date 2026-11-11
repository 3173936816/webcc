#ifndef __WEBCC_MODULES_LOG_DETAIL_THROW_LOG_EXCEPTION_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_THROW_LOG_EXCEPTION_HPP__

#include <cerrno>
#include <cstring>
#include <exception>
#include <string>

namespace webcc
{
namespace log
{
namespace detail
{

class log_exception :
    public std::exception
{
public:
    explicit log_exception(std::string errmsg) :
        errmsg_(std::move(errmsg)) {}

    const char* what() const noexcept override
    {
        return errmsg_.c_str();
    }

private:
    std::string errmsg_;
};

inline void throw_log_exception(int errnum, std::string usermsg)
{
    std::string msg = "log:{" + usermsg + "}: " + ::strerror(errnum);
    throw log_exception(std::move(msg));
}

inline void throw_log_exception(std::string usermsg)
{
    throw_log_exception(errno, std::move(usermsg));
}

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_THROW_LOG_EXCEPTION_HPP__