#ifndef __LCY_THROW_LOG_EXCEPTION_HPP__
#define __LCY_THROW_LOG_EXCEPTION_HPP__

#include <cerrno>
#include <cstring>
#include <exception>
#include <string>

namespace lcy
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

inline void throw_log_exception(std::string usermsg)
{
    std::string msg = "log:{" + usermsg + "}";
    throw log_exception(std::move(msg));
}

}   // namespace lcy

#endif  // __LCY_THROW_LOG_EXCEPTION_HPP__