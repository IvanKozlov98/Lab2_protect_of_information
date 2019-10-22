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

    void TChiferClefia::Hash(const std::string& inFileName, const std::string& outFileName) {
        ProcessImpl(inFileName, outFileName, EOperatingMode::HASH);
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

    TSubBlocks<TBlock32Bits> TChiferClefia::OperateImpl(
        const TSubBlocks<TBlock32Bits>& blocks,
        const EOperatingMode operatingMode,
        const std::unique_ptr<TSubBlocks<TBlock32Bits>> newWhiteningKeys) {
        TSubBlocks<TBlock32Bits> result;
        switch (operatingMode) {
        case EOperatingMode::CODE : {
            TChiferClefia::Encrypt(blocks, &result);
            break;
        }
        case EOperatingMode::DECODE : {
            TChiferClefia::Decrypt(blocks, &result);
            break;
        }
        case EOperatingMode::HASH : {
            if (newWhiteningKeys) {
                TClefiaKeyHelper::SetWhiteningKeys(
                    { 
                        (*newWhiteningKeys)[0],
                        (*newWhiteningKeys)[1],
                        (*newWhiteningKeys)[2],
                        (*newWhiteningKeys)[3]
                    });
                TChiferClefia::Encrypt(blocks, &result);
            }
        }
        }
        return result;
    }


    static TSubBlocks<TBlock32Bits> ReadNextBlock(std::ifstream& fin) {
        TSubBlocks<TBlock32Bits> blocks = { 0, 0, 0, 0 };
        for (int idx = 0; !fin.eof() && idx < branchCount; ++idx) {
            fin.read((char*)& blocks[idx], sizeof(TBlock32Bits));
        }
        return blocks;
    }

    void TChiferClefia::ProcessImpl(const std::string& inFileName, const std::string& outFileName, const EOperatingMode operatingMode)
    {
        std::ifstream inFile(inFileName, std::ifstream::binary);
        std::ofstream outFile(outFileName, std::ofstream::binary);
        std::unique_ptr<TSubBlocks<TBlock32Bits>> newWhiteningKeysPtr;
        TSubBlocks<TBlock32Bits> processedBlocks;
        bool isFirstIteration = true;
        if (inFile) {  // code all
            do {
                const auto blocks = ReadNextBlock(inFile);
                if (isFirstIteration) {
                    processedBlocks = OperateImpl(blocks, operatingMode);
                    isFirstIteration = false;
                }
                else {
                    processedBlocks = OperateImpl(
                        blocks,
                        operatingMode,
                        std::make_unique<TSubBlocks<TBlock32Bits>>(processedBlocks)
                    );
                }
                if (operatingMode == EOperatingMode::CODE || operatingMode == EOperatingMode::DECODE) {
                    OutBlock(outFile, processedBlocks);
                }
            } while (!inFile.eof());
            if (operatingMode == EOperatingMode::HASH) {
                OutBlock(outFile, processedBlocks);
            }
        }
        else {
            std::cout << "error: only " << inFile.gcount() << " could be read";
        }

        outFile.close();
        inFile.close();
    }

}
