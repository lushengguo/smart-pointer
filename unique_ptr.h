#pragma once
#include <tuple>

template <typename T>
class default_deleter
{
  public:
    void operator()(T *t) { delete t; }
};

template <typename T, typename Deleter = default_deleter<T>>
class unique_ptr
{
  public:
    using ThisType = unique_ptr<T, Deleter>;
    
  public:
    unique_ptr();
    unique_ptr(T *val) : t_(val, default_deleter<T>()) {}
    unique_ptr(const ThisType &) = delete;
    unique_ptr(const ThisType &&rhs) : t_(rhs.t_)
    {
        rhs.t_ = std::tuple<T *, Deleter>();
    }

    ~unique_ptr() { reset(); }

    ThisType &operator=(const ThisType &rhs)
    {
        t_ = std::move(rhs.t_);
        return *this;
    }

    T *release()
    {
        T *ptr = get();
        std::get<0>(t_) = nullptr;
        return ptr;
    }

    T *get() { return std::get<0>(t_); }

    Deleter &get_deleter() { return std::get<1>(t_); }

    void reset()
    {
        T *ptr = std::get<0>(t_);
        if (ptr)
        {
            std::get<1>(t_)(ptr);
        }
    }

  private:
    std::tuple<T *, Deleter> t_;
};