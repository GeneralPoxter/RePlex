#pragma once

#include <array>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <dlfcn.h>

template <typename E, size_t NumSymbols>
class RePlexModule
{
public:
    static void LoadLibrary() { GetInstance().Load(); }
    static void ReloadLibrary() { GetInstance().Reload(); }

protected:
    // Enforce singleton pattern
    static E &GetInstance()
    {
        static E instance;
        return instance;
    }

    // Path to library on disk
    virtual const char *GetPath() const = 0;

    using SymbolArray = std::array<std::pair<const char *, void *>, NumSymbols>;
    RePlexModule(SymbolArray &symbols) : m_symbols(symbols) {}

    template <size_t Index, typename Ret, typename... Args>
    Ret Execute(Args... args)
    {
        // Lookup function address
        static_assert(Index >= 0 && Index < NumSymbols, "Out of bounds symbol index");
        auto symbol = m_symbols[Index];
        // Cast to function type and call it
        return reinterpret_cast<Ret (*)(Args...)>(symbol.second)(args...);
    }

    template <size_t Index, typename T>
    T *GetVar()
    {
        static_assert(Index >= 0 && Index < NumSymbols, "Out of bounds symbol index");
        auto symbol = m_symbols[Index];
        return reinterpret_cast<T *>(symbol.second);
    }

private:
    void Load()
    {
        m_libHandle = dlopen(GetPath(), RTLD_NOW);
        LoadSymbols();
    }

    void Reload()
    {
        dlclose(m_libHandle);
        Load();
    }

    void LoadSymbols()
    {
        for (auto &&symbol : m_symbols)
        {
            symbol.second = dlsym(m_libHandle, symbol.first);
        }
    }

    void *m_libHandle;
    SymbolArray &m_symbols;
};