#include <iostream>

#include "unique_ptr.hpp"
using namespace SP;
using std::cout;
using std::endl;

struct Test {
  Test(int a, int b, int c) : sum(a + b + c) {}
  int sum;
};

void diy_delete_func(int* ptr) {
  if (ptr) delete ptr;
}

int main() {
  unique_ptr<int> ptr(new int(1));
  cout << *ptr.get() << endl;

  auto ptr2 = make_unqiue<Test>(1, 2, 3);
  cout << ptr2->sum << endl;

  Impl::Deleter<int> deleter;
  deleter.del(new int(1));

  Impl::Deleter<int> deleter2(diy_delete_func);
  deleter2.del(new int(2));
}
