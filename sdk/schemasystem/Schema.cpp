#include "Schema.hpp"

#include "../Interfaces/Interfaces.hpp"
#include "../tools/CUtlTsHash.hpp"




void SchemaSystem::LoadSchema(std::string ModuleName) {
	
	CSchemaSystemTypeScope* TypeScope = Interfaces::SchemaSystem->FindTypeScopeForModule(ModuleName.c_str());
	
	const int nTableSize = TypeScope->m_ClassBindings.Count();
	
	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = TypeScope->m_ClassBindings.GetElements(0, nTableSize, pElements);

	for (int i = 0; i < nElements; i++) {
		const UtlTSHashHandle_t hElement = pElements[i];
		if (hElement == 0) continue;


		SchemaClassBinding* ClassBinding = TypeScope->m_ClassBindings[hElement];
		if (ClassBinding == nullptr) continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		TypeScope->FindDeclaredClass(&pDeclaredClassInfo, ClassBinding->m_szBinaryName);
		
		if (pDeclaredClassInfo == nullptr) continue;
		if (pDeclaredClassInfo->m_nFieldSize == 0) continue;

		for (int j = 0; j < pDeclaredClassInfo->m_nFieldSize; j++){
			SchemaClassFieldData_t* field = &pDeclaredClassInfo->m_pFields[j];

			SchemaData[ClassBinding->m_szBinaryName][field->m_pszName].Classname = ClassBinding->m_szBinaryName;
			SchemaData[ClassBinding->m_szBinaryName][field->m_pszName].PropertyName = field->m_pszName;
			SchemaData[ClassBinding->m_szBinaryName][field->m_pszName].Offset = field->m_nSingleInheritanceOffset;
		}
	}
}

void SchemaSystem::LoadAll() {
	for (std::string Module : SchemaList){
		LoadSchema(Module);
	}
}

int32_t SchemaSystem::GetSchema(std::string Classname, std::string PropertyName) {
	return SchemaData[Classname][PropertyName].Offset;
}