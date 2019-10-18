//
// Created by Ivan on 16.10.2019.
//

#ifndef LAB2_PROTECT_TGALUAHELPER_H
#define LAB2_PROTECT_TGALUAHELPER_H

#include "types.h"

using NClefia::TBlock32Bits;
using NClefia::TBlock8Bits;

class TGaluaHelper {

private:
    static constexpr auto m = 8;
    static constexpr auto n = 255;
    static constexpr auto k = 253;

private:

    static std::vector<TBlock32Bits> alphaTo;
    static std::vector<TBlock32Bits> indexOf;
    // polynom
    constexpr static const int p[n + 1] = {1, 0, 1, 1, 1, 0, 0, 0, 1 };

    static void GenerateGf()
    {
        alphaTo.resize(n + 1);
        indexOf.resize(n + 1);
        //
        int i, mask;
        mask = 1; alphaTo[m] = 0;
        for (i = 0; i < m; i++)
        {
            alphaTo[i] = mask;
            indexOf[alphaTo[i]] = i;
            if (p[i] != 0) alphaTo[m] ^= mask;
            mask <<= 1;
        } indexOf[alphaTo[m]] = m; mask >>= 1;

        for (i = m+1; i < n; i++)
        {
            if (alphaTo[i-1] >= mask)
                alphaTo[i] = alphaTo[m] ^ ((alphaTo[i-1]^mask)<<1);
            else
                alphaTo[i] = alphaTo[i-1]<<1;
            indexOf[alphaTo[i]] = i;
        } indexOf[0] = -1;
    }

public:
    static TBlock8Bits Multiple(const TBlock8Bits a8, const TBlock8Bits b8) {
        if (indexOf.empty()) {
            GenerateGf();
        }
        TBlock32Bits sum;
        TBlock32Bits a = a8, b = b8;

        if (a == 0 || b == 0)
            return 0;
        sum = indexOf[a] + indexOf[b];
        sum = sum % (n + 1);
        return static_cast<TBlock8Bits>(alphaTo[sum]);
    }

    static TBlock8Bits Sum(const TBlock8Bits a, const TBlock8Bits b) {
        return a ^ b;
    }
};
std::vector<TBlock32Bits> TGaluaHelper::alphaTo;
std::vector<TBlock32Bits> TGaluaHelper::indexOf;

#endif //LAB2_PROTECT_TGALUAHELPER_H
