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

    // Array of all exported symbols to be reloaded
    virtual std::array<const char *, NumSymbols> &GetSymbolNames() const = 0;

    template <typename Ret, typename... Args>
    Ret Execute(const char *name, Args... args)
    {
        // Lookup function address
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end())
        {
            // Cast to function type and call it
            return reinterpret_cast<Ret (*)(Args...)>(symbol->second)(args...);
        }
        else
        {
            throw std::runtime_error(std::string("Function not found: ") + name);
        }
    }

    template <typename T>
    T *GetVar(const char *name)
    {
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end())
        {
            return reinterpret_cast<T *>(symbol->second);
        }
        else
        {
            return nullptr;
        }
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
        m_symbols.clear();
        Load();
    }

    void LoadSymbols()
    {
        for (const char *symbol : GetSymbolNames())
        {
            m_symbols[symbol] = dlsym(m_libHandle, symbol);
        }
    }

    void *m_libHandle;
    std::unordered_map<std::string, void *> m_symbols;
};