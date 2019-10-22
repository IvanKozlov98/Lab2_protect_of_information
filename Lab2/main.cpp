#include "pch.h"
#include <iostream>
#include "chifer.h"

using namespace NClefia;

int main(int argc, char* argv[]) {

    TChiferClefia::Code("rawFile.txt", "hashFile.txt");

    TChiferClefia::Decode("hashFile.txt", "rawFileHope.txt");

    TChiferClefia::Hash("rawFile.txt", "myMD5.txt");


    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}