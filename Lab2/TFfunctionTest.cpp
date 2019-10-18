#include "pch.h"
#include "TFfunction.h"
#include <iostream>
#include "TClefiaKeyHelper.h"


using namespace NClefia;

// тестовый набор, название теста
TEST(TestFunction0, TestFunction0Calc1) {
    EXPECT_EQ(0x547a3193, TFfunction0::Do(0xf3e6cef9, 0x00010203));
}

TEST(TestFunction0, TestFunction0Calc2) {
    EXPECT_EQ(0x8c9d011c, TFfunction0::Do(0xd3f463d6, 0x1459a507));
}

TEST(TestFunction0, TestFunction0Calc3) {
    EXPECT_EQ(0xd8dfd8de, TFfunction0::Do(0x6a27e20a, 0xfd15e1b8));
}

TEST(TestFunction0, TestFunction0Calc4) {
    EXPECT_EQ(0xb5b28e96, TFfunction0::Do(0xe8c528dc, 0xc4896f29));
}
