import os
import json 
from datetime import datetime

INPUT_DIR          = "./definitions/"
OUTPUT_COMPONENTS  = "../generated/gen_Components/"
OUTPUT_FAMILIES    = "../generated/gen_Families/"
OUTPUT_CPP         = "../generated/gen_cpp/"

componentNamesToIDs = {
	"TransformComponent" : 0,
	"DebugNameComponent" : 1,
    "CameraComponent"    : 2
}

def createAndWriteForComponentDict(c, componentID):
	fullname = c['name'] + "Component"
	componentNamesToIDs[fullname] = componentID

	#allComponnets
	allComponents.write("#include \"" + fullname + ".hpp\"\n")

	#componetsEnum
	componentsEnum.write("\t" + fullname + " = " + str(componentID) + ",\n")

	#construct component pools include
	createComponentPoolsCPP.write("\t{\n")
	createComponentPoolsCPP.write("\t\tboost::object_pool<{}>* p = new boost::object_pool<{}>;\n".format(fullname, fullname))
	createComponentPoolsCPP.write("\t\tpool[{}] = p;\n".format(componentID))
	createComponentPoolsCPP.write("\t\tdestroyers[{}] = [p](void * ptr)".format(componentID) + "{ \n")
	createComponentPoolsCPP.write("\t\t\tp->free((boost::object_pool<{}>::element_type *) ptr);".format(fullname) + "\n\t\t};\n")
	createComponentPoolsCPP.write("\t\tcleanup_callbacks.push_back([p](void) { delete p; });\n")
	createComponentPoolsCPP.write("\t}\n")

	#header file
	f = open(OUTPUT_COMPONENTS + fullname + ".hpp", mode='w')
	f.write("// " + fullname + ".hpp\n")
	f.write("// generated at: " + str(datetime.now()) + "\n")
	f.write("#ifndef " + fullname + "_hpp\n") 
	f.write("#define " + fullname + "_hpp\n\n") 

	f.write("#include \"Component.hpp\"\n")
	if "includes" in c:
		for s in c['includes']:
			f.write("#include " + s + "\n")
		f.write("\n")

	f.write("struct " + fullname + " : public Component {\n")

	f.write("\tstatic constexpr int componentIndex{ " + str(componentID) + " };\n")
	componentID += 1

	for m in c['members']:
		if 'default' in m:
			f.write("\t" + m['type'] + " " + m['name'] + " = " + m['default'] + ";\n")
		else:
			f.write("\t" + m['type'] + " " + m['name'] + ";\n")

	f.write("};\n")

	f.write("\n#endif")
	f.close()

def createAndWriteForFamilyDict(c, familyID):
	fullname = c['name'] + "Family"
	
	#allFamilies
	allFamilies.write("#include \"" + fullname + ".hpp\"\n")
	
	#familiesEnum
	familiesEnum.write("\t" + fullname + " = " + str(familyID) + ",\n")
	
	#familiesVectorConstructionCPP
	createFamilyMapsCPP.write("\t{\n")
	
	createFamilyMapsCPP.write("\t\tstd::unordered_map<entityID, {}>* m = new std::unordered_map<entityID, {}>;\n".format(fullname, fullname))
	createFamilyMapsCPP.write("\t\tarray[{}] = m;\n".format(familyID))
	createFamilyMapsCPP.write("\t\tcleanup_callbacks.push_back([m](void) {delete m;});\n")
	
	for componentName in c['components']:
		fullComponentName = componentName + "Component"
		createFamilyMapsCPP.write("\t\tFamily<{}>::mask.set({});\n".format(fullname, componentNamesToIDs[fullComponentName]))
		
	createFamilyMapsCPP.write("\t}\n")

	#clear families
	clearFamiliesCPP.write("getFamilyMap<{}>().clear();\n".format(fullname))

	#family filters
	filterEntitiesIntoFamiliesCPP.write("{\n")
	
	filterEntitiesIntoFamiliesCPP.write("if(ECSUtils::doesPassFilter(mask, Family<{}>::mask))".format(fullname) + "{\n")
	filterEntitiesIntoFamiliesCPP.write("\t{} family = {}(eID".format(fullname, fullname))

	for componentName in c['components']:
		fullComponentName = componentName + "Component"
		filterEntitiesIntoFamiliesCPP.write(", getComponent<{}>(eID)".format(fullComponentName))

	filterEntitiesIntoFamiliesCPP.write(");\n")
	filterEntitiesIntoFamiliesCPP.write("\tgetFamilyMap<{}>().emplace(std::make_pair(eID, family));\n".format(fullname))

	filterEntitiesIntoFamiliesCPP.write("\t}\n")
	
	filterEntitiesIntoFamiliesCPP.write("}\n")
	
	#header files
	f = open(OUTPUT_FAMILIES + fullname + ".hpp", mode='w')
	f.write("// " + fullname + ".hpp\n")
	f.write("// generated at: " + str(datetime.now()) + "\n")
	f.write("#ifndef " + fullname + "_hpp\n") 
	f.write("#define " + fullname + "_hpp\n\n")
	f.write("#include \"Family.hpp\"\n\n")
	
	for componentName in c['components']:
		fullComponentName = componentName + "Component"
		f.write("#include \"{}.hpp\"\n".format(fullComponentName))
	
	## see: https://stackoverflow.com/a/3829887 and https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	f.write("struct {} : public Family<{}> ".format(fullname, fullname) + "{\n")
#    f.write("struct " + fullname + " {\n")

	f.write("\tstatic constexpr int familyIndex{ " + str(familyID) + " };\n")
#    f.write("\tstatic")
	familyID += 1

	
	for componentName in c['components']:
		fullComponentName = componentName + "Component"
		f.write("\t{}& m_{};\n".format(fullComponentName, fullComponentName))
	
	f.write("\n\t{}(entityID _eID".format(fullname))
	
	for idx, componentName in enumerate(c['components']):
		fullComponentName = componentName + "Component"
		f.write(", {}& _{}".format(fullComponentName, fullComponentName))
	
	f.write(")\n\t: ")
	
	for idx, componentName in enumerate(c['components']):
		fullComponentName = componentName + "Component"
		if (idx == 0):
			f.write("m_{} (_{})".format(fullComponentName, fullComponentName))
		else:
			f.write(",\n\tm_{} (_{})\n".format(fullComponentName, fullComponentName))
	f.write("\t{eID = _eID;}\n")
	
	
	f.write("};")

	f.write("\n#endif")
	f.close()



def createHeadersFromJSON(j):
	global componentID, familyID
	if j['components']:
		for c in j['components']:
			createAndWriteForComponentDict(c, componentID)
			componentID += 1
	if j['families']:
		for f in j['families']:
			createAndWriteForFamilyDict(f, familyID)
			familyID += 1
	

if __name__ == "__main__":

	allComponents = open(OUTPUT_COMPONENTS + 'allComponents.h', mode='w')
	componentsEnum = open(OUTPUT_COMPONENTS + 'ComponentsEnum.hpp', mode='w')
	allFamilies = open(OUTPUT_FAMILIES + 'allFamilies.h', mode='w')
	familiesEnum = open(OUTPUT_FAMILIES + 'familiesEnum.hpp', mode='w')
	createComponentPoolsCPP = open(OUTPUT_CPP + 'populateComponentPoolsInclude.cpp', mode='w')
	createFamilyMapsCPP = open(OUTPUT_CPP + 'populateFamilyMapsInclude.cpp', mode='w')
	filterEntitiesIntoFamiliesCPP = open(OUTPUT_CPP + 'filterEntitiesIntoFamiliesInclude.cpp', mode='w')
	clearFamiliesCPP = open(OUTPUT_CPP + 'clearFamiliesInclude.cpp', mode='w')

	componentID = 3 ## transform, DebugName, Camera components built into the engine
	familyID = 0

	allComponents.write("// AllComponents.hpp\n")
	allComponents.write("// generated at: " + str(datetime.now()) + "\n")
	allComponents.write("""
#ifndef AllComponents_hpp
#define AllComponents_hpp\n

#include "TransformComponent.hpp"
#include "DebugNameComponent.hpp"
""")

	componentsEnum.write("// ComponentsEnum.hpp\n")
	componentsEnum.write("// generated at: " + str(datetime.now()) + "\n")
	componentsEnum.write("""
#ifndef ComponentsEnum_hpp
#define ComponentsEnum_hpp

enum class Components {
	TransformComponent = 0,
	DebugNameComponent = 1,
""")
	
	allFamilies.write("""
// AllFamilies.hpp\n
#ifndef AllFamilies_hpp
#define AllFamilies_hpp\n
""")

	familiesEnum.write("// familiesEnum.hpp\n")
	familiesEnum.write("// generated at: " + str(datetime.now()) + "\n")
	familiesEnum.write("""
#ifndef familiesEnum_hpp
#define familiesEnum_hpp\n

enum class Families {
""")

	for fname in os.listdir(INPUT_DIR):
		if fname.endswith(".json"):
			f = open(INPUT_DIR + fname, mode='r')
			createHeadersFromJSON(json.load(f))
			f.close()

#    allComponents.write("\n#define NUM_COMPONENTS " + str(componentID) + "\n")
#    allFamilies.write("\n#define NUM_FAMILIES " + str(familyID) + "\n")

	componentsEnum.write("\tComponentsCount = " + str(componentID) + "\n")
	componentsEnum.write("};\n")
	componentsEnum.write("\n#define NUM_COMPONENTS " + str(componentID) + "\n")
	
	familiesEnum.write("\tFamiliesCount = " + str(familyID) + "\n")
	familiesEnum.write("};\n")
	familiesEnum.write("\n#define NUM_FAMILIES " + str(familyID) + "\n")
	
	allComponents.write("#endif \n")
	componentsEnum.write("#endif \n")
	allFamilies.write("#endif \n")
	familiesEnum.write("#endif \n")

	allComponents.close()
	allFamilies.close()
	componentsEnum.close()
	createComponentPoolsCPP.close()


