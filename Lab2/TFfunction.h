//
// Created by Ivan on 13.10.2019.
//

#ifndef LAB2_PROTECT_TFFUNCTION_H
#define LAB2_PROTECT_TFFUNCTION_H

#include "types.h"

namespace NClefia {

    struct TFfunctionHelper {
    private:

        static TMatrixType<TBlock8Bits> MatrixS0;
        static TMatrixType<TBlock8Bits> MatrixS1;

    private:
        static TBlock8Bits GetValueFromTable(const TMatrixType<TBlock8Bits>& tableFrom, const TBlock8Bits block);

    protected:
        static TBlock8Bits Sbox0(const TBlock8Bits block);

        static TBlock8Bits Sbox1(const TBlock8Bits block);
    };

    struct TFfunction0 : private TFfunctionHelper {
        static TBlock32Bits Do(const TBlock32Bits roundKey, const TBlock32Bits block);

    private:
        static TMatrixType<TBlock8Bits> MatrixM0;
    };

    struct TFfunction1 : private TFfunctionHelper {
        static TBlock32Bits Do(const TBlock32Bits roundKey, const TBlock32Bits block);

    private:
        static TMatrixType<TBlock8Bits> MatrixM1;
    };

}



#endif //LAB2_PROTECT_TFFUNCTION_H
