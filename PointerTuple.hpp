#pragma once

#include <tuple>
#include <type_traits>

template <typename Type>
class deleter
{
public:
    constexpr deleter() noexcept = default;

    void operator()(Type* ptr) const { delete ptr; }
};

template <class Type, class Deleter = deleter<Type>>
class Pointer
{
public:
    /*  Member types */
    using pointer = Type*;
    using element_type = Type;
    using deleter_type = Deleter;

public: /** Member functions */
    // Constructs a Pointer that owns nothing.
    constexpr Pointer() noexcept = default;
    // constexpr Pointer(std::nullopt_t) noexcept = default;

    //  Constructs a Pointer which owns ptr, initializing the stored
    // pointer with ptr and value - initializing the stored deleter.
    explicit Pointer(pointer ptr) noexcept : _data() { m_ptr() = ptr; };

    //  Constructs a Pointer object which owns ptr, initializing the stored
    // pointer with ptr and initializing a deleter Del.
    template <typename Del>
    Pointer(pointer ptr, Del&& del) noexcept
        : _data(ptr, std::forward<Del>(del)){};

    //  Constructs a Pointer by transferring ownership from other to *this and
    // stores the null pointer in other.
    Pointer(Pointer&& other) noexcept : _data(std::move(other._data))
    {
        other.m_ptr() = nullptr;
    };

    Pointer& operator=(Pointer&& other) noexcept
    {
        reset(other.release());
        m_del() = std::forward<Deleter>(other.m_del());
        return *this;
    }

    ~Pointer() { reset(nullptr); }

    // Copy constructor and copy assignment operator are explicitly deleted.
    Pointer(const Pointer&) = delete;
    Pointer& operator=(const Pointer&) = delete;

public: /* Modifiers methods */
    // returns a pointer to the managed object and releases the ownership
    pointer release() noexcept
    {
        pointer ptr = m_ptr();
        m_ptr() = nullptr;
        return ptr;
    };

    // replaces the managed object
    void reset(pointer ptr = pointer()) noexcept
    {
        if (m_ptr())
            m_del()(m_ptr());
        m_ptr() = ptr;
    };

    // swaps the managed objects
    void swap(Pointer& other) noexcept { std::swap(_data, other._data); };

    /* Observers methods */
    // returns a pointer to the managed object
    pointer get() const noexcept { return m_ptr(); };

    // returns the deleter that is used for destruction of the managed object
    deleter_type& get_deleter() noexcept { return m_del(); };
    const deleter_type& get_deleter() const noexcept { return m_del(); };

    // checks if there is an associated managed object
    explicit operator bool() const noexcept { return get(); };

    // dereferences pointer to the managed object
    typename std::add_lvalue_reference<element_type>::type operator*() const
        noexcept(noexcept(*std::declval<pointer>()))
    {
        return *get();
    };

    pointer operator->() const noexcept { return get(); };

protected:
    pointer& m_ptr() noexcept { return std::get<0>(_data); };
    pointer m_ptr() const noexcept { return std::get<0>(_data); }
    deleter_type& m_del() noexcept { return std::get<1>(_data); }
    const deleter_type& m_del() const noexcept { return std::get<1>(_data); }

private:
    std::tuple<pointer, deleter_type> _data;
};