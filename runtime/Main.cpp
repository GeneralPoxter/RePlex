#include <RePlex.h>
#include <iostream>

#ifdef DEBUG
const char* g_libPath = "bin/Debug/libRePlexTest.so";
#else
const char* g_libPath = "bin/Release/libRePlexTest.so";
#endif

void (*foo)();

int main()
{
    void* handle = Load(g_libPath);
    if (handle)
    {
        foo = reinterpret_cast<void (*)()>(LoadSymbol(handle, "foo"));
        foo();

        int bar = *reinterpret_cast<int*>(LoadSymbol(handle, "bar"));
        std::cout << "bar == " << bar << std::endl;

        std::cout << "Make some changes, recompile, and press enter." << std::flush;
        while (std::cin.get() != '\n') {}

        Reload(handle, g_libPath);

        foo = reinterpret_cast<void (*)()>(LoadSymbol(handle, "foo"));
        foo();

        bar = *reinterpret_cast<int*>(LoadSymbol(handle, "bar"));
        std::cout << "bar == " << bar << std::endl;
    } else
    {
        PrintError();
        return 1;
    }
    return 0;
}