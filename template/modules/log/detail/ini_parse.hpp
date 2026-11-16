#ifndef __WEBCC_LOG_DETAIL_INI_PARSE_HPP__
#define __WEBCC_LOG_DETAIL_INI_PARSE_HPP__

#include <cstddef>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <inifile/inifile.h>
#include <lcy/log4cc/log4cc.hpp>

#include "modules/log/detail/throw_log_exception.hpp"

namespace webcc
{
namespace log
{
namespace detail
{

inline std::string to_upper(const std::string& s)
{
    for ( char& c : s ) {
        c = std::toupper(c);
    }
    return s;
}

inline void ini_parse(const std::string& ini_file,
    std::list<lcy::log4cc::logger::sptr_type>& logger_list)
{
    ini::inifile inif;
    if ( !inif.load(ini_file) ) {
        throw_log_exception("ini_parse: cannot load " + ini_file);
    }

    std::string default_level_str = inif.get("log", "level", "INFO");
    std::string default_pattern = inif.get("log", "pattern",
        "[%d{%Y-%m-%d %H:%M:%S}] [%l] [pid:%p] [tid:%t] [%f:%n] [%c] %m%E");

    lcy::log4cc::level default_level = lcy::log4cc::level::info;
    lcy::log4cc::level_from_string(to_upper(default_level_str), default_level);

    struct appender_desc
    {
        std::string type;
        std::string path;
        std::size_t size_limit;
    };

    std::map<std::string, appender_desc> appender_map;

    for ( const auto& sec : inif.sections() ) {
        if ( sec.rfind("appender.", 0) != 0 ) {
            continue;
        }

        std::string app_name = sec.substr(9);
        appender_desc desc;

        desc.type = inif.get(sec, "type", "");
        desc.path = inif.get(sec, "path", "");

        std::string size_str = inif.get(sec, "size_limit", "0");
        desc.size_limit = static_cast<std::size_t>(std::stoull(size_str));

        appender_map[app_name] = std::move(desc);
    }

    for ( const auto& sec : inif.sections() ) {
        if ( sec.rfind("logger.", 0) != 0 ) {
            continue;
        }

        std::string logger_name = sec.substr(7);

        std::string level_str = inif.get(sec, "level", default_level_str);
        lcy::log4cc::level lvl = default_level;
        lcy::log4cc::level_from_string(to_upper(level_str), lvl);

        auto lg = std::make_shared<lcy::log4cc::logger>(logger_name, lvl);

        std::string pattern = inif.get(sec, "pattern", default_pattern);
        lg->set_formatter(std::make_shared<lcy::log4cc::formatter>(pattern));

        std::string appenders_str = inif.get(sec, "appenders", "");
        auto app_names = ini::split(appenders_str, ',');

        for ( auto& app_name : app_names ) {
            ini::trim(app_name);
            if ( app_name.empty() ) {
                continue;
            }

            auto it = appender_map.find(app_name);
            if ( it == appender_map.end() ) {
                continue;
            }

            const auto& desc = it->second;

            lcy::log4cc::appender_base::sptr_type app;

            if ( desc.type == "console" ) {
                app = std::make_shared<lcy::log4cc::console_appender_st>();
            }
            else if ( desc.type == "file" ) {
                app = std::make_shared<lcy::log4cc::file_appender_st>(desc.path);
            }
            else if ( desc.type == "rotating_file" ) {
                app = std::make_shared<lcy::log4cc::rotating_file_appender_st>(
                    desc.path, desc.size_limit);
            }
            else {
                continue;
            }

            lg->add_appender(app);
        }

        logger_list.push_back(lg);
    }
}

}   // namespace detail
}   // namespace log
}   // namespace webcc

#endif  // __WEBCC_LOG_DETAIL_INI_PARSE_HPP__