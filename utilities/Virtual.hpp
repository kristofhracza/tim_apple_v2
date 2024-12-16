#pragma once

#include <cstdint>
#include <cstddef>



namespace Virtual {
    template <typename T>
    inline T Get(void* instance, const unsigned int index) {
        return (*static_cast<T**>(static_cast<void*>(instance)))[index];
    }

    template <typename T = void*>
    inline T Get(const void* instance, const unsigned int index) {
        return (*static_cast<T* const*>(static_cast<const void*>(instance)))[index];
    }

    template <typename T>
    inline T Read(const std::uintptr_t location) {
        return *reinterpret_cast<T*>(location);
    }

    template <typename T>
    inline void Write(const std::uintptr_t location, const T& data) {
        *reinterpret_cast<T*>(location) = data;
    }

    template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
    static __forceinline T Call(CBaseClass* thisptr, Args_t... argList)
    {
        using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
        return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
    }

    template <typename T, class CBaseClass, typename... Args_t>
    static __forceinline T Call(CBaseClass* thisptr, std::size_t nIndex, Args_t... argList)
    {
        using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
        return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
    }
}