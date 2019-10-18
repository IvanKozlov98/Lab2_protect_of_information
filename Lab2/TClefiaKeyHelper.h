//
// Created by Ivan on 15.10.2019.
//

#ifndef LAB2_PROTECT_TCLEFIAKEYHELPER_H
#define LAB2_PROTECT_TCLEFIAKEYHELPER_H

#include "types.h"
#include "TSubBlocks.h"

namespace NClefia {

    class TClefiaKeyHelper {

    public:
        static const std::vector<TBlock32Bits>& GetWhiteningKeys();
        static const std::vector<TBlock32Bits>& GetRoundKeys();

    public:
        static const std::vector<TBlock32Bits> Keys;
        static const std::vector<TBlock32Bits> CON;

    private:

        static void InitRoundAndWhiteningKeys();

        //TODO write test for this function
        // inline function
        static TSubBlocks<TBlock32Bits> DoubleSwap(const TSubBlocks<TBlock32Bits>& blocks);
    private:
        static std::vector<TBlock32Bits> roundKeys;
        static std::vector<TBlock32Bits> whiteningKeys;
    };
}



#endif //LAB2_PROTECT_TCLEFIAKEYHELPER_H
