#ifndef __WEBCC_MODULES_LOG_DETAIL_LOGGER_MANAGER_HPP__
#define __WEBCC_MODULES_LOG_DETAIL_LOGGER_MANAGER_HPP__

#include <string>
#include <unordered_map>
#include <lcy/log4cc/log4cc.hpp>

#include "modules/noncopyable.hpp"

namespace webcc
{
namespace log
{
namespace detail
{

class logger_manager :
    private noncopyable
{
public:
    logger_manager(const std::string& config_file)
    {

    }

    lcy::log4cc::logger::sptr_type get_logger_by_name(const std::string& name)
    {

    }

private:
    std::unordered_map<
        std::string,
        lcy::log4cc::logger::sptr_type
    > loggers_;
};

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_MODULES_LOG_DETAIL_LOGGER_MANAGER_HPP__