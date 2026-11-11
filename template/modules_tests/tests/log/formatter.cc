#include <cassert>
#include <iostream>
#include <regex>
#include <string>

#include "modules/log/event.hpp"
#include "modules/log/formatter.hpp"

using webcc::log::event;
using webcc::log::formatter;
using webcc::log::level;

namespace
{

event make_event(level l,
                 const char* file,
                 std::size_t line,
                 const char* logger_name,
                 const std::string& message)
{
    event e(l, file, line, logger_name);
    e.oss_ << message;
    return e;
}

void print(const char* title, const std::string& out)
{
    std::cout << "--- " << title << " ---\n";
    std::cout << out << "\n\n";
}

void test_default_pattern()
{
    formatter f;

    assert(f.get_pattern() ==
           "[%d] [%l] [pid:%p] [tid:%t] [%f:%n] [%c] %m");

    event e = make_event(level::info, "main.cpp", 42, "root", "hello");

    std::string out = f.format(e);

    std::regex re(R"(^\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] )"
                  R"(\[INFO\] )"
                  R"(\[pid:\d+\] )"
                  R"(\[tid:\d+\] )"
                  R"(\[main\.cpp:42\] )"
                  R"(\[root\] )"
                  R"(hello$)");

    assert(std::regex_match(out, re));

    print("default pattern", out);
}

void test_custom_pattern()
{
    formatter f("%l %m");

    event e = make_event(level::warn, "x.cpp", 7, "lg", "msg");

    std::string out = f.format(e);

    assert(out == "WARN msg");

    print("%l %m", out);
}

void test_set_pattern()
{
    formatter f("[%l] %m");

    event e1 = make_event(level::error, "x.cpp", 7, "lg", "first");

    assert(f.format(e1) == "[ERROR] first");

    f.set_pattern("%m/%l");

    event e2 = make_event(level::debug, "x.cpp", 8, "lg", "second");

    std::string out = f.format(e2);

    assert(out == "second/DEBUG");

    print("after set_pattern", out);
}

void test_literal_only()
{
    formatter f("hello world");

    event e = make_event(level::info, "a.cpp", 1, "lg", "");

    std::string out = f.format(e);

    assert(out == "hello world");

    print("literal only", out);
}

void test_all_placeholders()
{
    formatter f("[%d] [%l] [pid:%p] [tid:%t] [%f:%n] [%c] %m");

    event e = make_event(level::fatal, "file.cpp", 99, "app", "boom");

    std::string out = f.format(e);

    assert(out.find("[FATAL]") != std::string::npos);
    assert(out.find("[pid:") != std::string::npos);
    assert(out.find("[tid:") != std::string::npos);
    assert(out.find("[file.cpp:99]") != std::string::npos);
    assert(out.find("[app]") != std::string::npos);
    assert(out.find(" boom") != std::string::npos);

    print("all placeholders", out);
}

void test_percent_escape()
{
    formatter f("100%% done: %m");

    event e = make_event(level::info, "a.cpp", 1, "lg", "ok");

    std::string out = f.format(e);

    assert(out == "100% done: ok");

    print("percent escape", out);
}

void test_unknown_placeholder()
{
    formatter f("%x %m");

    event e = make_event(level::info, "a.cpp", 1, "lg", "ok");

    std::string out = f.format(e);

    assert(out == "%x ok");

    print("unknown placeholder", out);
}

void test_trailing_percent()
{
    formatter f("end%");

    event e = make_event(level::info, "a.cpp", 1, "lg", "");

    std::string out = f.format(e);

    assert(out == "end%");

    print("trailing percent", out);
}

void test_empty_pattern()
{
    formatter f("");

    event e = make_event(level::info, "a.cpp", 1, "lg", "ignored");

    std::string out = f.format(e);

    assert(out.empty());

    print("empty pattern", out);
}

void test_empty_message()
{
    formatter f("[%l] %m");

    event e = make_event(level::info, "a.cpp", 1, "lg", "");

    std::string out = f.format(e);

    assert(out == "[INFO] ");

    print("empty message", out);
}

void test_repeated_format()
{
    formatter f("[%l] %m");

    event e1 = make_event(level::info, "a.cpp", 1, "lg", "one");
    event e2 = make_event(level::warn, "a.cpp", 2, "lg", "two");

    assert(f.format(e1) == "[INFO] one");
    assert(f.format(e2) == "[WARN] two");
    assert(f.format(e1) == "[INFO] one");

    print("repeated format", f.format(e1));
}

}   // namespace

int main()
{
    test_default_pattern();
    test_custom_pattern();
    test_set_pattern();
    test_literal_only();
    test_all_placeholders();
    test_percent_escape();
    test_unknown_placeholder();
    test_trailing_percent();
    test_empty_pattern();
    test_empty_message();
    test_repeated_format();

    std::cout << "all formatter tests passed\n";

    return 0;
}