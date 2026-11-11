#ifndef __WEBCC_MODULES_LOG_DETAIL_FILE_APPENDER_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_FILE_APPENDER_HPP__

#include <cerrno>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "modules/log/detail/appender_base.hpp"
#include "modules/log/detail/throw_log_exception.hpp"

namespace webcc
{
namespace log
{
namespace detail
{

class file_appender :
    public appender_base
{
public:
    file_appender(std::string path, std::size_t buffer_size) :
        is_dirty_(false), path_(std::move(path)), buffer_size_(buffer_size)
    {
        log_fd_ = ::open(path_.c_str(),
                O_WRONLY | O_CREAT | O_APPEND, 0644);
        if ( log_fd_ < 0 ) {
            throw_log_exception("log open " + path_ + " file error");
        }
    }

    ~file_appender()
    {
        flush();
        ::close(log_fd_);
    }

    void reopen() override
    {
        flush();
        ::close(log_fd_);
        log_fd_ = ::open(path_.c_str(),
            O_WRONLY | O_CREAT | O_APPEND, 0644);
        if ( log_fd_ < 0 ) {
            throw_log_exception("log open " + path_ + " file error");
        }
    }

    void flush()
    {
        if ( is_dirty_ ) {
            ::write(log_fd_, buffer_.data(), buffer_.size());
            buffer_.clear();

            is_dirty_ = false;
        }
    }

    void log(event& e) override
    {
        buffer_ += formatter_.format(e);
        buffer_.push_back('\n');

        if ( buffer_.size() < buffer_size_ ) {
            is_dirty_ = true;
            return;
        }

        flush();
    }

private:
    bool is_dirty_;
    std::string path_;
    std::string buffer_;
    std::size_t buffer_size_;
};

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_FILE_APPENDER_HPP__
