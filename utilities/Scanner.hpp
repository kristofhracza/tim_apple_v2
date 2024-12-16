#pragma once

#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

#include <Psapi.h>
#include <string>
#include <vector>



#define SCHEMASYSTEM_TYPE_SCOPES_OFFSET 0x188
#define SCHEMASYSTEMTYPESCOPE_OFF1 0x3F8
#define SCHEMASYSTEMTYPESCOPE_OFF2 0x8

#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define _CS_INTERNAL_STRINGIFY(NAME) #NAME
#define _CS_INTERNAL_UNPARENTHESIZE(...) __VA_ARGS__

#define CS_STRINGIFY(NAME) _CS_1INTERNAL_STRINGIFY(NAME)
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)

#define MEM_PAD(SIZE)                                        \
private:                                                  \
    char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE];       \
public:



class Scanner {
public:
    static MODULEINFO GetModuleInfo(std::string ModuleName) {
        MODULEINFO ModInfo = { 0 };
        HMODULE HModule = GetModuleHandleA(ModuleName.c_str());
        if (HModule != 0)
            GetModuleInformation(GetCurrentProcess(), HModule, &ModInfo, sizeof(MODULEINFO));
        return ModInfo;
    }

    static std::vector<std::uint32_t> PatternToByte(const char* Pattern) {
        std::vector<std::uint32_t> Bytes;
        char* Start = const_cast<char*>(Pattern);
        char* End = const_cast<char*>(Pattern) + std::strlen(Pattern);

        for (char* Current = Start; Current < End; Current++) {
            if (*Current == '?') {
                Current++;

                if (*Current == '?') Current++;
                Bytes.push_back(-1);
            }
            else {
                Bytes.push_back(std::strtoul(Current, &Current, 16));
            }
        }

        return Bytes;
    }

    static std::uint8_t* PatternScan(const char* ModuleName, const char* Signature) {
        HMODULE ModuleHandle = GetModuleHandleA(ModuleName);

        if (!ModuleHandle) return nullptr;

        PIMAGE_DOS_HEADER DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(ModuleHandle);
        PIMAGE_NT_HEADERS NtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(ModuleHandle) + DosHeader->e_lfanew);

        std::size_t SizeOfImage = NtHeaders->OptionalHeader.SizeOfImage;
        std::vector<std::uint32_t> PatternBytes = PatternToByte(Signature);
        std::uint8_t* ImageBase = reinterpret_cast<std::uint8_t*>(ModuleHandle);

        std::size_t PatternSize = PatternBytes.size();
        std::uint32_t* ArrayOfBytes = PatternBytes.data();

        for (std::size_t i = 0; i < SizeOfImage - PatternSize; i++) {
            bool Found = true;

            for (std::size_t j = 0; j < PatternSize; j++) {
                if (ImageBase[i + j] != ArrayOfBytes[j] && ArrayOfBytes[j] != -1) {
                    Found = false;
                    break;
                }
            }

            if (Found) return &ImageBase[i];
        }
        return nullptr;
    }

    static [[nodiscard]] __forceinline std::uint8_t* ResolveRelativeAddress(std::uint8_t* AddressBytes, std::uint32_t RVAOffset, std::uint32_t RIPOffset) {
        std::uint32_t RVA = *reinterpret_cast<std::uint32_t*>(AddressBytes + RVAOffset);
        std::uint64_t RIP = reinterpret_cast<std::uint64_t>(AddressBytes) + RIPOffset;

        return reinterpret_cast<std::uint8_t*>(RVA + RIP);
    }

    template <typename T = std::uint8_t>
    static [[nodiscard]] T* GetAbsoluteAddress(T* RelativeAddress, int PreOffset = 0x0, int PostOffset = 0x0) {
        RelativeAddress += PreOffset;
        RelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(RelativeAddress);
        RelativeAddress += PostOffset;
        return RelativeAddress;
    }
};
