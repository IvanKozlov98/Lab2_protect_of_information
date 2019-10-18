//
// Created by Ivan on 14.10.2019.
//

#ifndef LAB2_PROTECT_TSUBBLOCKS_H
#define LAB2_PROTECT_TSUBBLOCKS_H


namespace NClefia {

    // use only 4 blocks
    template <typename T>
    class TSubBlocks {

    public:

        TSubBlocks() {
            Blocks.resize(4);
        }

        explicit TSubBlocks(const std::vector<T> blocks) {
            Blocks = blocks;
        }

        TSubBlocks(const std::initializer_list<T>& blocks) {
            Blocks.assign(blocks.begin(), blocks.end());
        }

        TSubBlocks(const TSubBlocks &otherSubBlocks) {
            Blocks = otherSubBlocks.Blocks;
        }

        TSubBlocks<T>& operator=(const TSubBlocks<T>& otherSubBlocks) {
            Blocks = otherSubBlocks.Blocks;
            return *this;
        }

        TSubBlocks<T>& operator=(TSubBlocks<T>&& otherSubBlocks) {
            Blocks = std::move(otherSubBlocks.Blocks);
            return *this;
        }

        const T& operator[](const size_t index) const {
            return Blocks.at(index);
        }

        T& operator[](const size_t index) {
            return Blocks.at(index);
        }

        size_t GetSize() const {
            return Blocks.size();
        }

    private:
        std::vector<T> Blocks;
    };
}


#endif //LAB2_PROTECT_TSUBBLOCKS_H
