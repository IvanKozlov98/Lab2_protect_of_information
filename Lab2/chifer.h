//
// Created by Ivan on 13.10.2019.
//

#ifndef LAB2_PROTECT_CHIFER_H
#define LAB2_PROTECT_CHIFER_H


#pragma once

#include <fstream>
#include <string>
#include <functional>
#include "TGfn.h"
#include "TSubBlocks.h"
#include "TClefiaKeyHelper.h"
#include "types.h"

namespace NClefia {

    class TChiferClefia {

    private:
      enum class EOperatingMode {
          CODE, DECODE
      };

      //TODO make this private  
    public:
        static void Encrypt(const TSubBlocks<TBlock32Bits>& blocks, TSubBlocks <TBlock32Bits>* resultBlocks);

        static void Decrypt(const TSubBlocks<TBlock32Bits>& blocks, TSubBlocks <TBlock32Bits>* resultBlocks);

        static TSubBlocks<TBlock32Bits> OperateImpl(const TSubBlocks<TBlock32Bits>& blocks, const EOperatingMode operatingMode);

        static void ProcessImpl(const std::string& inFileName, const std::string& outFileName, const EOperatingMode operatingMode);

    public:
        static void Code(const std::string& inFileName, const std::string& outFileName);

        static void Decode(const std::string& inFileName, const std::string& outFileName);
    };

}

#endif //LAB2_PROTECT_CHIFER_H
