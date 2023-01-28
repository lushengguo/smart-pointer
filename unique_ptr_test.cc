#include "unique_ptr.h"
#include <gtest/gtest.h>

TEST(unique_ptr, constructor)
{
    unique_ptr<int> u(new int);
}
