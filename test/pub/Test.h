#pragma once

#include <RePlex.h>

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
    int foo(int);
    extern int bar;
}

std::array<std::pair<const char *, void *>, 2> g_exports = {
    std::make_pair("foo", nullptr),
    std::make_pair("bar", nullptr)};

class TestModule : public RePlexModule<TestModule, g_exports.size()>
{
public:
    static int Foo(int x)
    {
        return GetInstance().Execute<0, int, int>(x);
    }

    static int GetBar()
    {
        return *GetInstance().GetVar<1, decltype(bar)>();
    }

    TestModule() : RePlexModule(g_exports) {}

protected:
    virtual const char *GetPath() const override
    {
#ifdef DEBUG
        return "bin/Debug/libRePlexTest.so";
#else
        return "bin/Release/libRePlexTest.so";
#endif
    }
};