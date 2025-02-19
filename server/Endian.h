#ifndef ENDIAN_UTILS_H
#define ENDIAN_UTILS_H

#include <cstdint>

namespace Endian {
    inline std::uint32_t toLittleEndian(std::uint32_t value) {
        return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value >> 8) & 0xFF00) | ((value >> 24) & 0xFF);
    }
}

#endif // ENDIAN_UTILS_H
