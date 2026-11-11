#ifndef __WEBCC_MODULES_LOG_EVENT_HPP__
#define __WEBCC_MODULES_LOG_EVENT_HPP__

#include <string>
#include <sstream>
#include <cstddef>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "modules/log/level.hpp"

namespace webcc
{
namespace log
{

namespace internal
{

inline pid_t pid()
{
    thread_local pid_t pid = ::getpid();
    return pid;
}

inline pthread_t thread_id()
{
    thread_local pthread_t tid = ::pthread_self();
    return tid;
}

}   // namespace internal

struct event
{
    level level_;
    pid_t pid_;
    pthread_t thread_id_;
    std::time_t timestamp_;
    std::string file_name_;
    std::size_t file_line_;
    std::string logger_name_;
    std::ostringstream oss_;

    event(level l,
          std::string file_name,
          std::size_t file_line,
          std::string logger_name) :
        level_(l),
        pid_(internal::pid()),
        thread_id_(internal::thread_id()),
        timestamp_(std::time(0)),
        file_name_(std::move(file_name)),
        file_line_(file_line),
        logger_name_(std::move(logger_name))
    {
    }
};
    
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_EVENT_HPP__