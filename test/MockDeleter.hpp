#pragma once

#include <gmock/gmock.h>

template <typename T>
class MockDeleter
{
public:
    MockDeleter() = default;
    MockDeleter(MockDeleter&&){};
    MockDeleter& operator=(MockDeleter&&){};

    MockDeleter(const MockDeleter&){};
    MockDeleter& operator=(const MockDeleter&){};
    ~MockDeleter(){};

public:
    MOCK_METHOD(void, deleter_call, (T*));
    void operator()(T* ptr) { return deleter_call(ptr); };
};
