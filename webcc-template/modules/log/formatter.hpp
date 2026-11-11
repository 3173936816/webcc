#ifndef __WEBCC_MODULES_LOG_FORMATTER_HPP__
#define __WEBCC_MODULES_LOG_FORMATTER_HPP__

#include <list>
#include <string>
#include <sstream>

#include "modules/log/event.hpp"
#include "modules/noncopyable.hpp"

namespace webcc
{
namespace log
{

class formatter :
    private noncopyable
{
public:
    class item
    {
    public:
        virtual ~item() {}
        virtual void get_item(event& e,
            std::ostringstream& oss) const = 0;
    };

public:
    formatter();
    formatter(std::string pattern);
    ~formatter();

    const std::string& get_pattern() const;
    void set_pattern(std::string pattern);

    std::string format(event& e) const;

private:
    void parse_pattern();
    void clear_items();

private:
    std::string pattern_;
    std::list<item*> items_;
};

}   // namespace log
}   // namespace webcc

#include "modules/log/impl/formatter.ipp"

#endif  // __WEBCC_MODULES_LOG_FORMATTER_HPP__