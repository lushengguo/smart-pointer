#pragma once
#include <functional>
#include <utility>  // for std::swap std::forward

namespace SP {

namespace Impl {
template <typename ObjType>
class Deleter {
 public:
  using StdFunctionDeleter = std::function<void(ObjType *)>;
  typedef void (*DeleteFuncionPtr)(ObjType *);

  Deleter() {}
  Deleter(StdFunctionDeleter &&func) : deleter_(std::forward(func)) {}
  Deleter(DeleteFuncionPtr func) : deleter_(func) {}

  void del(ObjType *ptr) { deleter_ ? deleter_(ptr) : delete ptr; }

 private:
  StdFunctionDeleter deleter_;
};
}  // namespace Impl

template <typename ObjType, class Deleter = Impl::Deleter<ObjType>>
class unique_ptr {
 public:
  using DeleteFuncionPtr = Deleter::DeleteFuncionPtr;

  unique_ptr() : obj_(nullptr) {}
  unique_ptr(ObjType *obj) : obj_(obj) {}
  unique_ptr(ObjType *obj, DeleteFuncionPtr *del) : obj_(obj), deleter_(del) {}
  unique_ptr(ObjType *obj, std::function<void(ObjType *)> del)
      : obj_(obj), deleter_(del) {}

  ~unique_ptr() { _release(); }

  unique_ptr(const unique_ptr &) = delete;
  unique_ptr &operator=(const unique_ptr &) = delete;

  operator bool() { return obj_; }
  ObjType &operator*() { return *obj_; }
  ObjType *operator->() { return obj_; }
  ObjType &operator[](size_t n) { return *(obj_ + n * sizeof(ObjType)); }
  bool operator==(const unique_ptr<ObjType> rhs) { return rhs.obj_ == obj_; }

 public:
  ObjType *get() { return obj_; }
  void release() { _release(); }
  void reset(ObjType *ptr) {
    _release();
    obj_ = ptr;
  }
  void swap(unique_ptr<ObjType> rhs) { std::swap(obj_, rhs.obj_); }

 private:
  void _release() { deleter_.del(obj_); }

 private:
  ObjType *obj_;
  Deleter deleter_;
};

// template <class T, class... Args>
// unique_ptr<T> make_unique(Args &&... args);
// (1)	(since C++14)
// (only for non-array types)
//
// template< class T >
// unique_ptr<T> make_unique( std::size_t size );
// (2)	(since C++14)
// (only for array types with unknown bound)
//
// template< class T, class... Args >
// /* unspecified */ make_unique( Args&&... args ) = delete;
// (3)	(since C++14)
// (only for array types with known bound)

template <typename ObjType, typename... Args>
unique_ptr<ObjType> make_unqiue(Args &&... args) {
  //这语法真他妈神奇
  return unique_ptr<ObjType>(new ObjType(std::forward<Args>(args)...));
}

}  // namespace SP