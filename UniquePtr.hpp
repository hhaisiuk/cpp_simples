#pragma once

#include <functional>

template <typename Tp>
class UniquePtr
{
    static void get_rid_of(Tp* ptr) { delete ptr; }

public:
    using pointer = Tp*;
    using element_type = Tp;
    using deleter = std::function<void(Tp*)>;

    UniquePtr()
        : _ptr(nullptr),
          _dltr(std::bind(&UniquePtr::get_rid_of, std::placeholders::_1)){};

    UniquePtr(pointer ptr)
        : _ptr(ptr),
          _dltr(std::bind(&UniquePtr::get_rid_of, std::placeholders::_1)){};

    UniquePtr(Tp* ptr, deleter&& dltr)
        : _ptr(ptr), _dltr(std::forward<deleter>(dltr)){};

    UniquePtr(UniquePtr&& other)
        : _ptr(std::move(other._ptr)), _dltr(std::move(other._dltr))
    {
        other._ptr = nullptr;
    };

    UniquePtr& operator=(UniquePtr&& other)
    {
        reset(other.release());
        _dltr = std::forward<deleter>(other._dltr);
        return *this;
    };

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr operator=(const UniquePtr&) = delete;

    ~UniquePtr() { reset(nullptr); }

public:
    pointer get() const { return _ptr; }

    deleter& get_deleter() { return _dltr; }
    const deleter& get_deleter() const { return _dltr; }

    pointer release() noexcept
    {
        pointer ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    };

    void reset(pointer ptr = pointer()) noexcept
    {
        if (_ptr)
            _dltr(_ptr);
        _ptr = ptr;
    };

    void swap(UniquePtr& other) noexcept
    {
        std::swap(_ptr, other._ptr);
        std::swap(_dltr, other._dltr);
    };

    explicit operator bool() const noexcept { return get(); };

    typename std::add_lvalue_reference<element_type>::type operator*() const
        noexcept(noexcept(*std::declval<pointer>()))
    {
        return *get();
    };

    pointer operator->() const noexcept { return get(); };

private:
    pointer _ptr;
    deleter _dltr;
};