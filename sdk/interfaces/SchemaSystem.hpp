#pragma once

#include <cstdint>

#include "../../utilities/Virtual.hpp"
#include "../../utilities/Scanner.hpp"

#include "../tools//CUtlTsHash.hpp"



class CSchemaType;
class CSchemaSystemTypeScope;
class CSchemaType;
class CSchemaClassInfo;
struct SchemaMetadataEntryData_t;



struct SchemaClassFieldData_t {
    const char* m_pszName; 
    CSchemaType* m_pSchemaType; 
    std::uint32_t m_nSingleInheritanceOffset;
    std::int32_t m_nMetadataSize;
    SchemaMetadataEntryData_t* m_pMetaData;
};

struct SchemaClassInfoData_t;

struct SchemaBaseClassInfoData_t {
    std::int32_t m_nOffset;
    SchemaClassInfoData_t* m_pClass;
};

struct SchemaClassInfoData_t{
private:
    void* pVtable; 
public:
    const char* m_szName; 
    char* szDescription; 

    int m_nSize; 
    std::int16_t m_nFieldSize; 
    std::int16_t m_nStaticSize;
    std::int16_t m_nMetadataSize;

    std::uint8_t m_nAlignOf;
    std::uint8_t m_nBaseClassesCount;
    char pad2[0x4]; 
    SchemaClassFieldData_t* m_pFields; 
    char pad3[0x8]; 
    SchemaBaseClassInfoData_t* m_pBaseClasses; 
    char pad4[0x28]; 

    bool InheritsFrom(SchemaClassInfoData_t* m_pClassInfo) {
        if (m_pClassInfo == this && m_pClassInfo != nullptr)
            return true;
        else if (m_pBaseClasses == nullptr || m_pClassInfo == nullptr)
            return false;

        for (int i = 0; i < m_nBaseClassesCount; i++){
            auto& baseClass = m_pBaseClasses[i];
            if (baseClass.m_pClass->InheritsFrom(m_pClassInfo))
                return true;
        }

        return false;
    };
};


struct SchemaClassBinding{
    SchemaClassBinding* m_pParent;
    const char* m_szBinaryName;
    const char* m_szModuleName;
    const char* m_szClassName;
    void* m_pClassInfoOldSynthesized;
    void* m_pClassInfo;
    void* m_pThisModuleBindingPointer;
    CSchemaType* m_pSchemaType;
};


struct SchemaType_t {
    std::uintptr_t* m_vtable;
    const char* m_szName;
    CSchemaSystemTypeScope* m_pTypeScope; 
};




class CSchemaSystem {
public:
    [[nodiscard]] CSchemaSystemTypeScope* FindTypeScopeForModule(const char* m_szName) {
        return Virtual::Get<CSchemaSystemTypeScope* (__thiscall*)(void*, const char*, void*)>(this, 13)(this, m_szName, nullptr);
    };
};


class CSchemaSystemTypeScope
{
public:
    void FindDeclaredClass(SchemaClassInfoData_t** m_pReturnClass, const char* m_szClassName){
        return Virtual::Call<void, 2U>(this, m_pReturnClass, m_szClassName);
    }

    CSchemaType* FindSchemaTypeByName(const char* m_szName, std::uintptr_t* m_pSchema){
        return Virtual::Call<CSchemaType*, 4U>(this, m_szName, m_pSchema);
    }

    CSchemaType* FindTypeDeclaredClass(const char* m_szName){
        return Virtual::Call<CSchemaType*, 5U>(this, m_szName);
    }

    CSchemaType* FindTypeDeclaredEnum(const char* m_szName){
        return Virtual::Call<CSchemaType*, 6U>(this, m_szName);
    }

    SchemaClassBinding* FindRawClassBinding(const char* m_szName){
        return Virtual::Call<SchemaClassBinding*, 7U>(this, m_szName);
    }
    void* m_pVtable; 
    char m_szName[256U]; 
    MEM_PAD(SCHEMASYSTEMTYPESCOPE_OFF1); 
    CUtlTSHash<SchemaClassBinding*, 256, unsigned int> m_ClassBindings;
    MEM_PAD(SCHEMASYSTEMTYPESCOPE_OFF2); 
};




class CSchemaType {
public:
    uintptr_t* m_vtable;
    const char* m_pszName;
    CSchemaSystemTypeScope* m_pTypeScope;
    std::uint8_t m_TypeCategory;
    std::uint8_t m_AtomicCategory;

    [[nodiscard]] CSchemaType* GetRefClass() {
        if (m_TypeCategory != 1) return nullptr;

        CSchemaType* thisPointer = this->m_Schematype;

        while (thisPointer && thisPointer->m_TypeCategory == 1) {
            thisPointer = thisPointer->m_Schematype;
        }

        return thisPointer;
    }

    struct Atomic_T {
        int m_nAtomicID;
        CSchemaType* m_pTemplateType;
        CSchemaType* m_pTemplateType2;
    };

    struct AtomicTypeInfo_TT_t {
        int m_nAtomicID;
        CSchemaType* m_pTemplateType;
        CSchemaType* m_pTemplateType2;
    };

    struct AtomicTypeInfo_I_t{
        uint64_t m_nAtomicID;
        uint64_t m_nInteger;
    };

    union
    {
        CSchemaType* m_Schematype;
    };
};


class CSchemaClassInfo : public SchemaClassInfoData_t { public:};
