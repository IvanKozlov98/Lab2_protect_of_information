//
// Created by Ivan on 13.10.2019.
//
#include "pch.h"
#include "chifer.h"
#include <iostream>
#include <array>

namespace NClefia {

    void OutBlock(std::ofstream& fout, const TSubBlocks<TBlock32Bits>& blocks, size_t blocksCount = 0) {
        if (blocksCount == 0) {
            blocksCount = blocks.GetSize();
        }
        for(auto i = 0u; i < blocksCount; ++i) {
            fout.write((char*)&blocks[i], sizeof(TBlock32Bits));
        }
    }

    void TChiferClefia::Code(const std::string &inFileName, const std::string &outFileName) {
        ProcessImpl(inFileName, outFileName, EOperatingMode::CODE);
    }

    void TChiferClefia::Decode(const std::string& inFileName, const std::string& outFileName) {
        ProcessImpl(inFileName, outFileName, EOperatingMode::DECODE);
    }

    static inline void ApplyWhiteningKeys(
        const TBlock32Bits whiteningKey1,
        const TBlock32Bits whiteningKey2,
        TSubBlocks<TBlock32Bits>* resultBlocks) {
        (*resultBlocks)[1] ^= whiteningKey1;
        (*resultBlocks)[3] ^= whiteningKey2;
    }

    // TODO maybe this method move to helper
    void TChiferClefia::Encrypt(
            const TSubBlocks<TBlock32Bits>& blocks,
            TSubBlocks<TBlock32Bits>* resultBlocks) {
        *resultBlocks = blocks;
        const auto& whiteningKeys = TClefiaKeyHelper::GetWhiteningKeys();
        // step 1
        ApplyWhiteningKeys(whiteningKeys[0], whiteningKeys[1], resultBlocks);
        //step 2
        *resultBlocks = TGfn::Do(*resultBlocks, TClefiaKeyHelper::GetRoundKeys());
        //step 3
        ApplyWhiteningKeys(whiteningKeys[2], whiteningKeys[3], resultBlocks);
    }
    
    void TChiferClefia::Decrypt(const TSubBlocks<TBlock32Bits>& blocks, TSubBlocks<TBlock32Bits>* resultBlocks)
    {
        *resultBlocks = blocks;
        const auto& whiteningKeys = TClefiaKeyHelper::GetWhiteningKeys();
        // step 1
        ApplyWhiteningKeys(whiteningKeys[2], whiteningKeys[3], resultBlocks);
        //step 2
        *resultBlocks = TGfn::DoOpposite(*resultBlocks, TClefiaKeyHelper::GetRoundKeys());
        //step 3
        ApplyWhiteningKeys(whiteningKeys[0], whiteningKeys[1], resultBlocks);
    }

    TSubBlocks<TBlock32Bits> TChiferClefia::OperateImpl(const TSubBlocks<TBlock32Bits>& blocks, const EOperatingMode operatingMode) {
        TSubBlocks<TBlock32Bits> result;
        switch (operatingMode) {
        case EOperatingMode::CODE: {
            TChiferClefia::Encrypt(blocks, &result);
            break;
        }
        case EOperatingMode::DECODE: {
            TChiferClefia::Decrypt(blocks, &result);
            break;
        }
        }
        return result;
    }

    void TChiferClefia::ProcessImpl(const std::string& inFileName, const std::string& outFileName, const EOperatingMode operatingMode)
    {
        std::ifstream inFile(inFileName, std::ifstream::binary);
        std::ofstream outFile(outFileName, std::ofstream::binary);

        if (inFile) {  // code all
            TSubBlocks<TBlock32Bits> blocks = { 0, 0, 0, 0 };
            uint8_t ind = 0;
            do {
                inFile.read((char*)& blocks[ind], sizeof(TBlock32Bits));
                ++ind;
                if (ind % branchCount == 0) {
                    ind = 0;
                    OutBlock(outFile, OperateImpl(blocks, operatingMode));
                }
            } while (!inFile.eof());
            // if number of bytes is not a multiple of 4
            if (ind % branchCount) {
                // append zeros to blocks
                for (auto i = ind; i < 4; ++i) {
                    blocks[i] = 0;
                }
                OutBlock(outFile, OperateImpl(blocks, operatingMode), ind);
            }

        }
        else {
            std::cout << "error: only " << inFile.gcount() << " could be read";
        }

        outFile.close();
        inFile.close();
    }

}
