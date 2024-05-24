#include <iostream>
#include <exception>

#include "Pointer.hpp"

template <typename T>
void init(Pointer<T>& obj) {
    for(int i = 0; i < obj.size(); i++)
        *(obj.data() + i) = i;
};

template <typename T>
Pointer<T> init(size_t size, const T& value) {
    return Pointer<T>(size, value);
}

template <typename T>
void print(Pointer<T>& obj) {
    for(int i = 0; i < obj.size(); i++)
        std::cout << *(obj.data() + i) << " ";
    std::cout << '\n';
}

int main()
{
    {
        Pointer<int> ptr_obj(30);
        for(int i = 0; i < ptr_obj.size(); i++) {
            *(ptr_obj.data() + i) = i;
            std::cout << *(ptr_obj.data() + i) << " ";
        }
        std::cout << std::endl;
    }
    
    {
        Pointer<int> ptr_obj(0);
    }

    {
        try {
            Pointer<int> ptr_obj(-1);
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        } 
    }

    {
        // copy constructor check
        Pointer<int> src(10);
        init(src);
        Pointer<int> dst(src);
        print(src);
        print(dst);

        // change some values in object to be sure 
        // copy constructor works properly
        for(int i = 0; i < src.size() / 2 && i < dst.size() / 2; i++) {
            std::swap(*(src.data() + i), *(dst.data() + (dst.size() - i - 1)));
        }

        print(src);
        print(dst);
    }

    {
        // copy assignment
        Pointer<int> dst;
        print(dst);
        Pointer<int> src(10);
        init(src);
        print(src);
        dst = src;
        print(dst);
        print(src);

        for(int i = 0; i < src.size() / 2 && i < dst.size() / 2; i++) {
            std::swap(*(src.data() + i), *(dst.data() + (dst.size() - i - 1)));
        }
        print(dst);
        print(src);
    }

    {
        
        Pointer<char> src(10, 'a');
        print(src);

        Pointer<char> dst(std::move(src));
        print(dst);

        src = init(10, 'B');
        print(src);

        dst = std::move(src);
        print(dst);

    }

    

    return 0;
}