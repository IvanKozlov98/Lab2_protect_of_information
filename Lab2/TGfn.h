//
// Created by Ivan on 13.10.2019.
//

#ifndef LAB2_PROTECT_TGFN_H
#define LAB2_PROTECT_TGFN_H

#include "types.h"
#include "TSubBlocks.h"

namespace NClefia {
    struct TGfn {
    public:
        static TSubBlocks<TBlock32Bits> Do(const TSubBlocks<TBlock32Bits>& blocks, const std::vector<TBlock32Bits>& roundKeys);
        static TSubBlocks<TBlock32Bits> DoOpposite(const TSubBlocks<TBlock32Bits>& blocks, const std::vector<TBlock32Bits>& roundKeys);
    };

}

#endif //LAB2_PROTECT_TGFN_H
