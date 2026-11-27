#ifndef __WEBCC_IPC_DETAIL_MASTER_HPP__
#define __WEBCC_IPC_DETAIL_MASTER_HPP__

#include "modules/noncopyable.hpp"

namespace webcc
{
namespace ipc
{
namespace detail
{

class master
{
public:

private:
    typedef lcy::asio::local::stream::socket stream_socket_type;

    stream_socket_type stream_socket_;
};

}   // namespace detail
}   // namespace ipc
}   // namespace webcc

#endif  // __WEBCC_IPC_DETAIL_MASTER_HPP__