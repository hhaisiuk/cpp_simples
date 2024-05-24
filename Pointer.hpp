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
    void release() { if(_size > 0 ) delete[] _data; }
    void allocate() { if(_size > 0) _data = new T[_size]; }
    
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
    : _size(size), _data(nullptr)
{
    if(_size > 0) {
        _data = new T[_size];
        auto iter = _data;
        while (iter < _data + _size)
             *(iter++) = value;
    }
}

template <typename T>
Pointer<T>::~Pointer() 
{
    release();
}

template <typename T>
Pointer<T>::Pointer(const Pointer& other)
    : _size(other._size)
    , _data(nullptr) 
{
    allocate();
    std::memcpy(_data, other._data, (sizeof(T)  * _size));
}

template <typename T>
Pointer<T>& Pointer<T>::operator=(const Pointer& other)
{
    if(this != &other) {
        _size = other._size;
        T* new_data = new T[_size];
        std::memcpy(new_data, other._data, (sizeof(T) *_size));
        delete [] _data;
        _data = new_data;
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