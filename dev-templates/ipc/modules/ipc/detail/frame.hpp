#ifndef __WEBCC_IPC_DETAIL_FRAME_HPP__
#define __WEBCC_IPC_DETAIL_FRAME_HPP__

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

namespace webcc
{
namespace ipc
{
namespace detail
{

struct frame_header
{
    uint32_t action_;
    uint32_t length_;
    uint32_t flag_;
};

inline int parse_frame_header(const void* data,
    std::size_t length, frame_header& fh)
{
    if ( length < sizeof(frame_header) ) {
        return 0;
    }

    ::memcpy(&fh, data, sizeof(frame_header));

    return 1;
}

}   // namespace detail
}   // namespace ipc
}   // namespace webcc

#endif  // __WEBCC_IPC_DETAIL_FRAME_HPP__
