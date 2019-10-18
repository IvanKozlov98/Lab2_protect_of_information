
//
// Created by Ivan on 15.10.2019.
//
#include "pch.h"
#include <cassert>
#include "TClefiaKeyHelper.h"
#include "TGfn.h"

namespace NClefia {

    std::vector<TBlock32Bits> TClefiaKeyHelper::roundKeys;
    std::vector<TBlock32Bits> TClefiaKeyHelper::whiteningKeys;

    const std::vector<TBlock32Bits> TClefiaKeyHelper::Keys = {0xffeeddcc, 0xbbaa9988, 0x77665544, 0x33221100};

    const std::vector<TBlock32Bits> TClefiaKeyHelper::CON = {
            0xf56b7aeb, 0x994a8a42, 0x96a4bd75, 0xfa854521,
            0x735b768a, 0x1f7abac4, 0xd5bc3b45, 0xb99d5d62,
            0x52d73592, 0x3ef636e5, 0xc57a1ac9, 0xa95b9b72,
            0x5ab42554, 0x369555ed, 0x1553ba9a, 0x7972b2a2,
            0xe6b85d4d, 0x8a995951, 0x4b550696, 0x2774b4fc,
            0xc9bb034b, 0xa59a5a7e, 0x88cc81a5, 0xe4ed2d3f,
            0x7c6f68e2, 0x104e8ecb, 0xd2263471, 0xbe07c765,
            0x511a3208, 0x3d3bfbe6, 0x1084b134, 0x7ca565a7,
            0x304bf0aa, 0x5c6aaa87, 0xf4347855, 0x9815d543,
            0x4213141a, 0x2e32f2f5, 0xcd180a0d, 0xa139f97a,
            0x5e852d36, 0x32a464e9, 0xc353169b, 0xaf72b274,
            0x8db88b4d, 0xe199593a, 0x7ed56d96, 0x12f434c9,
            0xd37b36cb, 0xbf5a9a64, 0x85ac9b65, 0xe98d4d32,
            0x7adf6582, 0x16fe3ecd, 0xd17e32c1, 0xbd5f9f66,
            0x50b63150, 0x3c9757e7, 0x1052b098, 0x7c73b3a7
    };


    TSubBlocks<TBlock32Bits> TClefiaKeyHelper::DoubleSwap(const TSubBlocks<TBlock32Bits>& blocks) {
        // separate on four parts - a, b, c, d
        uint64_t part1 = (static_cast<uint64_t>(blocks[0]) << 32) | static_cast<uint64_t>(blocks[1]);
        uint64_t part2 = (static_cast<uint64_t>(blocks[2]) << 32) | static_cast<uint64_t>(blocks[3]);
        //
        uint64_t a1 = part1 >> 57u;
        uint64_t b1 = BitsLeftRight<uint64_t>(part1, 7u, 7u);

        uint64_t a2 = BitsLeftRight<uint64_t>(part2, 57u, 57u);
        uint64_t b2 = part2 >> 7u;

        //
        uint64_t newPart1 = (b1 << 7u) | a2;
        uint64_t newPart2 = (a1 << 57u) | b2;
        //
        TBlock32Bits a, b, c, d;
        a = BitsLeftRight<uint64_t>(newPart1, 0, 32u);
        b = BitsLeftRight<uint64_t>(newPart1, 32u, 32u);
        c = BitsLeftRight<uint64_t>(newPart2, 0, 32u);
        d = BitsLeftRight<uint64_t>(newPart2, 32u, 32u);

        return TSubBlocks<TBlock32Bits>({a, b, c, d});
    }


    TSubBlocks<TBlock32Bits> operator^(const TSubBlocks<TBlock32Bits> &subBlock1, const TSubBlocks<TBlock32Bits> &subBlock2) {
        assert(subBlock1.GetSize() == subBlock2.GetSize());
        TSubBlocks<TBlock32Bits> result(subBlock1);
        for (auto idx = 0u; idx < subBlock1.GetSize(); ++idx) {
            result[idx] = subBlock1[idx] ^ subBlock2[idx];
        }
        return result;
    }
    void TClefiaKeyHelper::InitRoundAndWhiteningKeys() {
        // generate round keys :
        /*TSubBlocks<TBlock32Bits> keysBlock(Keys);
        const std::vector<TBlock32Bits> conAsRoundKey(CON.begin(), CON.begin() + 24);
        // step 1
        auto L = TGfn::Do(keysBlock, conAsRoundKey);
        // expanding K and L
        // step 2
        whiteningKeys = Keys;
        roundKeys.resize(2 * roundCount);
        // step3
        for (auto idx = 0u; idx <= 8u; ++idx) {
            auto tempBlocks = L ^
                    TSubBlocks<TBlock32Bits>({
                        CON[24 + 4 * idx],
                        CON[24 + 4 * idx + 1],
                        CON[24 + 4 * idx + 2],
                        CON[24 + 4 * idx + 3]
                        }
            );
            L = DoubleSwap(L);
            if (idx % 2) {
                tempBlocks = tempBlocks ^ keysBlock;
            }
            roundKeys[4 * idx] = tempBlocks[0];
            roundKeys[4 * idx + 1] = tempBlocks[1];
            roundKeys[4 * idx + 2] = tempBlocks[2];
            roundKeys[4 * idx + 3] = tempBlocks[3];
        }*/

        whiteningKeys = Keys;
        roundKeys = {
            0xf3e6cef9, 0x8df75e38, 0x41c06256, 0x640ac51b,
            0x6a27e20a, 0x5a791b90, 0xe8c528dc, 0x00336ea3,
            0x59cd17c4, 0x28565583, 0x312a37cc, 0xc08abd77,
            0x7e8e7eec, 0x8be7e949, 0xd3f463d6, 0xa0aad6aa,
            0xe75eb039, 0x0d657eb9, 0x018002e2, 0x9117d009,
            0x9f98d11e, 0xbabee8cf, 0xb0369efa, 0xd3aaef0d,
            0x3438f93b, 0xf9cea4a0, 0x68df9029, 0xb869b4a7,
            0x24d6406d, 0xe74bc550, 0x41c28193, 0x16de4795,
            0xa34a20f5, 0x33265d14, 0xb19d0554, 0x5142f434 };
    }


    const std::vector<TBlock32Bits>& TClefiaKeyHelper::GetWhiteningKeys() {
        if (whiteningKeys.empty()) {
            InitRoundAndWhiteningKeys();
        }
        return whiteningKeys;
    }

    const std::vector<TBlock32Bits>& TClefiaKeyHelper::GetRoundKeys() {
        if (roundKeys.empty()) {
            InitRoundAndWhiteningKeys();
        }
        return roundKeys;
    }
}


