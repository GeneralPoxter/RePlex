#pragma once

// This line prevents C++ name mangling which would prevent dlsym from retrieving
// the correct symbols.
extern "C"
{
    void foo();

    extern int bar;
}