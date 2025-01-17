#pragma once

#include <RePlex.h>

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
    void foo();
    extern int bar;
}

std::array<const char *, 2> g_exports = {"foo", "bar"};

class TestModule : public RePlexModule<TestModule, g_exports.size()>
{
public:
    static void Foo()
    {
        GetInstance().Execute<void>("foo");
    }

    static int GetBar()
    {
        return *GetInstance().GetVar<decltype(bar)>("bar");
    }

protected:
    virtual const char *GetPath() const override
    {
#ifdef DEBUG
        return "bin/Debug/libRePlexTest.so";
#else
        return "bin/Release/libRePlexTest.so";
#endif
    }

    virtual std::array<const char *, g_exports.size()> &GetSymbolNames() const override
    {
        return g_exports;
    }
};