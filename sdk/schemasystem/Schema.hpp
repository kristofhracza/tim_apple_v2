#pragma once

#include <string>
#include <deque>
#include <unordered_map>



#define SCHEMA(Classname, PropertyName, Function, Type) \
__forceinline Type& Function() \
{ \
    std::uint32_t Offset = SchemaSystem::GetSchema(Classname, PropertyName);  \
    return *reinterpret_cast<Type*>(reinterpret_cast<uint64_t>(this) + Offset); \
} \

#define SCHEMA_OFFSET(Classname, Function, Type, Offset) \
__forceinline Type& Function() \
{ \
    return *reinterpret_cast<Type*>(reinterpret_cast<uint64_t>(this) + Offset); \
}

#define SCHEMA_OFFSET_PROPERTY(Classname,PropertyName, Function, Type, Offset) \
__forceinline Type& Function() \
{ \
    std::uint32_t DerivedOffset = SchemaSystem::GetSchema(Classname, PropertyName);  \
    return *reinterpret_cast<Type*>(reinterpret_cast<uint64_t>(this) + DerivedOffset + Offset); \
}


namespace SchemaSystem {
	struct schemaOffset{
		std::string Classname;
		std::string PropertyName;
		std::int32_t Offset;
	};

	inline std::vector<std::string> SchemaList = { "client.dll","engine2.dll","schemasystem.dll"};
	inline static std::unordered_map<std::string, std::unordered_map<std::string, SchemaSystem::schemaOffset>> SchemaData;

	void LoadSchema(std::string ModuleName);
	void LoadAll();
	std::int32_t GetSchema(std::string Classname, std::string PropertyName);
};