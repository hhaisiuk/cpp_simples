#include <utility>
#include <cstring>
#include <iostream>

template <typename T>
class Pointer
{
public:
    explicit Pointer(size_t size = 0);
    explicit Pointer(size_t size, const T& value);
    ~Pointer();

    Pointer(const Pointer&);
    Pointer& operator=(const Pointer&);

    Pointer(Pointer&&);
    Pointer& operator=(Pointer&&);

public:
    inline unsigned int size() const { return _size; };
    inline T* data() { return _data; };

private:
    inline void allocate() { if(_size > 0) _data = new T[_size]; }
    inline void delocate() { if(_data) delete[] _data; }
    
private:
    size_t _size;
    T* _data;
};

template <typename T>
Pointer<T>::Pointer(size_t size)
    :  _size(size), _data(nullptr) 
{
    allocate();
}

template <typename T>
Pointer<T>::Pointer(size_t size, const T& value)
    : Pointer(size)
{
    auto iter = _data;
    while (iter != _data + _size)
        *(iter++) = value;
}

template <typename T>
Pointer<T>::~Pointer() {
    delocate();
}

template <typename T>
Pointer<T>::Pointer(const Pointer& other)
    : Pointer(other._size)
{
    if(this != &other && _size > 0)
        std::memcpy(_data, other._data, (sizeof(T)  * _size));
}

template <typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer& other)
{
    if(this != &other) {
        delocate();
        _size = other._size;
        allocate();
        std::memcpy(_data, other._data, (sizeof(T) *_size));
    }

    return *this;
}

template <typename T>
Pointer<T>::Pointer(Pointer&& other)
    : _size(std::move(other._size))                 
    , _data(std::move(other._data)) 
{
    other._data = nullptr;
}

template <typename T>
Pointer<T>& Pointer<T>::operator=(Pointer&& other)
{
    if(this != &other) {
        _size = std::exchange(other._size, 0);
        _data = std::exchange(other._data, nullptr);
    }
        
    return *this;
}