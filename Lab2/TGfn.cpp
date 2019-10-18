//
// Created by Ivan on 13.10.2019.
//

#include "pch.h"
#include "TGfn.h"
#include <cassert>
#include "TFfunction.h"
#include "TClefiaKeyHelper.h"

namespace NClefia {

    static inline void TranslateBlocks(TSubBlocks<TBlock32Bits>* blocks, const bool isRightDirection = false) {
        assert(blocks->GetSize() == 4);

        if (isRightDirection) {
            const auto lastElement = (*blocks)[3];
            for(auto idx = 3; idx >= 1; --idx) {
                (*blocks)[idx] = (*blocks)[idx - 1];
            }
            (*blocks)[0] = lastElement;
        } else {
            const auto firstElement = (*blocks)[0];
            for(auto idx = 0; idx < 3; ++idx) {
                (*blocks)[idx] = (*blocks)[idx + 1];
            }
            (*blocks)[3] = firstElement;
        }
    }

    TSubBlocks<TBlock32Bits> TGfn::Do(const TSubBlocks<TBlock32Bits>& blocks, const std::vector<TBlock32Bits>& roundKeys) {
        const auto iterationCount = roundKeys.size() / 2;
        //step 1
        TSubBlocks<TBlock32Bits> tempBlocks(blocks);
        //step 2
        for(auto idx = 0u; idx < iterationCount; ++idx){
            tempBlocks[1] = tempBlocks[1] ^ TFfunction0::Do(roundKeys[2 * idx], tempBlocks[0]);
            tempBlocks[3] = tempBlocks[3] ^ TFfunction1::Do(roundKeys[2 * idx + 1], tempBlocks[2]);
            TranslateBlocks(&tempBlocks, /*isRightDirection*/ false);
        }
        //step 3
        TranslateBlocks(&tempBlocks, /*isRightDirection*/ true);
        return tempBlocks;
    }


    TSubBlocks<TBlock32Bits> TGfn::DoOpposite(const TSubBlocks<TBlock32Bits>& blocks, const std::vector<TBlock32Bits>& roundKeys) {
      const int roundKeysCount = roundKeys.size();
      const int iterationCount = roundKeysCount / 2;
      //step 1
      TSubBlocks<TBlock32Bits> tempBlocks(blocks);
      //step 2
      for (auto idx = 0u; idx < iterationCount; ++idx) {
        tempBlocks[1] = tempBlocks[1] ^ TFfunction0::Do(roundKeys[roundKeysCount - 2 * idx - 2], tempBlocks[0]);
        tempBlocks[3] = tempBlocks[3] ^ TFfunction1::Do(roundKeys[roundKeysCount - 2 * idx - 1], tempBlocks[2]);
        TranslateBlocks(&tempBlocks, /*isRightDirection*/ true);
      }
      //step 3
      TranslateBlocks(&tempBlocks, /*isRightDirection*/ false);
      return tempBlocks;
    }
}

