#include "unique_ptr.h"
#include <gtest/gtest.h>

struct TestCtorDtor
{
    TestCtorDtor(bool &b) : b_(b) { b_ = true; }
    ~TestCtorDtor() { b_ = false; }
    bool &b_;
};

TEST(unique_ptr, construct)
{
    bool b = false;
    {
        unique_ptr<TestCtorDtor> u(nullptr);
        EXPECT_FALSE(u);
    }

    {
        unique_ptr<TestCtorDtor> u(new TestCtorDtor(b));
        EXPECT_TRUE(b);
        EXPECT_TRUE(u);
    }

    {
        unique_ptr<TestCtorDtor> u(new TestCtorDtor(b));
        unique_ptr<TestCtorDtor> u1(std::move(u));
        EXPECT_TRUE(b);
        EXPECT_TRUE(u1);
        EXPECT_FALSE(u);
    }
}

TEST(unique_ptr, destruct)
{
    bool b = false;
    {
        unique_ptr<TestCtorDtor> u(new TestCtorDtor(b));
        EXPECT_TRUE(b);
    }
    EXPECT_FALSE(b);
}

TEST(unique_ptr, operatorAssign)
{
    bool b = false;

    {
        unique_ptr<TestCtorDtor> u(new TestCtorDtor(b));
        EXPECT_TRUE(u);
        unique_ptr<TestCtorDtor> u2 = std::move(u);
        EXPECT_FALSE(u);
        EXPECT_TRUE(u2);
    }

    {
        unique_ptr<TestCtorDtor> u(new TestCtorDtor(b));
        EXPECT_TRUE(u);
        u = nullptr;
        EXPECT_FALSE(u);
    }
}

unique_ptr<int> pass_through(unique_ptr<int> u) { return u; }

TEST(unique_ptr, move)
{
    unique_ptr<int> u(new int);
    EXPECT_TRUE(u);
    unique_ptr<int> u2(pass_through(std::move(u)));
    EXPECT_TRUE(u2);
    EXPECT_FALSE(u);
}

void add(int *i)
{
    if (i)
        *i += 1;
}

TEST(unique_ptr, diy_deleter)
{
    int i = 0;
    {
        unique_ptr<int, void (*)(int *)> u(&i, [](int *i) {
            if (i)
                *i = 1;
        });
        EXPECT_TRUE(i == 0);
    }
    EXPECT_TRUE(i == 1);

    {
        unique_ptr<int, decltype(&add)> u(&i, &add);
        EXPECT_TRUE(i == 1);
    }
    EXPECT_TRUE(i == 2);
}

TEST(unique_ptr, make_unique)
{
    unique_ptr<int> u = make_unique<int>(1); 
    EXPECT_TRUE(u);
    EXPECT_TRUE((*u) == 1);
}