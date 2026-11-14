#ifndef __WEBCC_MODULES_NONCOPYABLE_HPP__
#define __WEBCC_MODULES_NONCOPYABLE_HPP__

namespace webcc
{

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
};

}   // namespace webcc

#endif  // __WEBCC_MODULES_NONCOPYABLE_HPP__