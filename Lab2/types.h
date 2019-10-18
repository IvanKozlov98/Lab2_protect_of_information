//
// Created by Ivan on 16.10.2019.
//

#ifndef LAB2_PROTECT_TYPES_H
#define LAB2_PROTECT_TYPES_H

#include <cstdint>
#include <vector>

namespace NClefia {
    using TBlock8Bits = uint8_t;
    using TBlock32Bits = uint32_t;

    template<typename T>
    using TMatrixType = std::vector<std::vector<T>>;

    static constexpr auto branchCount = 4;
    static constexpr auto bitCountInOneBlock = 128;
    static constexpr auto roundCount = 18;

    constexpr auto SizeBlock = 16 /*bytes*/;

    template <typename T>
    T BitsLeftRight(const T value, const uint16_t left, const uint16_t right) {
      T result = (value << left);
      result >>= right;
      return result;
    }
}


#endif //LAB2_PROTECT_TYPES_H
