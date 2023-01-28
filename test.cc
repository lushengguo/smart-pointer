#include <iostream>
#include <tuple>

struct test
{
    test() {}
    test(test &&t) { std::cout << "move constructor"; }
};

int main()
{
    std::tuple<test> t1;
    std::tuple<test> t2 = std::move(t1);
}