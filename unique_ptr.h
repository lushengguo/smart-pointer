#pragma once
#include <cstddef>
#include <tuple>

template <typename T>
class default_deleter
{
  public:
    void operator()(T *t)
    {
        if (t)
        {
            delete t;
        }
    }
};

template <typename T>
class default_deleter<T[]>
{
  public:
    void operator()(T *t)
    {
        if (t)
        {
            delete[] t;
        }
    }
};

template <typename T, typename Deleter = default_deleter<T>>
class unique_ptr
{
  public:
    using ThisType = unique_ptr<T, Deleter>;
    using CompressedPtr = std::tuple<T *, Deleter>;

  public:
    unique_ptr() : t_(nullptr, default_deleter<T>()) {}
    unique_ptr(T *val) : t_(val, default_deleter<T>()) {}
    unique_ptr(T *val, const Deleter &del) : t_(val, del) {}
    unique_ptr(const ThisType &) = delete;
    unique_ptr(ThisType &&rhs) : t_(rhs.t_) { rhs.t_ = CompressedPtr(); }

    ~unique_ptr() { reset(); }

    operator bool() const { return std::get<0>(t_); }
    T &operator*() { return *get(); }
    T *operator->() { return get(); }

    ThisType &operator=(const ThisType &rhs)
    {
        reset();
        t_ = std::move(rhs.t_);
        return *this;
    }

    ThisType &operator=(std::nullptr_t)
    {
        reset();
        return *this;
    }

    ThisType &operator=(ThisType &&rhs)
    {
        reset();
        std::get<0>(t_) = rhs.release();
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
            std::get<0>(t_) = nullptr;
        }
    }

    void swap(ThisType &rhs) { std::swap(t_, rhs.t_); }

  private:
    CompressedPtr t_;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// T[] 的重载版就不写了