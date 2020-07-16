import os
import json 
from datetime import datetime

INPUT_DIR          = "./definitions/"
OUTPUT_COMPONENTS  = "../generated/gen_Components/"
OUTPUT_FAMILIES    = "../generated/gen_Families/"
OUTPUT_CPP         = "../generated/gen_cpp/"


def createAndWriteForComponentDict(c, componentID):
    fullname = c['name'] + "Component"

    #allComponnets
    allComponents.write("#include \"" + fullname + ".hpp\"\n")

    #componetsEnum
    componentsEnum.write("\t" + fullname + " = " + str(componentID) + ",\n")

    #construct component pools include
    createPoolsCPP.write("\t{\n")
    createPoolsCPP.write("\t\tboost::object_pool<{}>* p = new boost::object_pool<{}>;\n".format(fullname, fullname))
    createPoolsCPP.write("\t\tpool[{}] = p;\n".format(componentID))
    createPoolsCPP.write("\t\tdestroyers[{}] = [p](void * ptr)".format(componentID) + "{ \n")
    createPoolsCPP.write("\t\t\tp->free((boost::object_pool<{}>::element_type *) ptr);".format(fullname) + "\n\t\t};\n")
    createPoolsCPP.write("\t\tcleanup_callbacks.push_back([p](void) { delete p; });\n")
    createPoolsCPP.write("\t}\n")

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
    allFamilies.write("#include \"" + fullname + ".hpp\n")
    
    f = open(OUTPUT_FAMILIES + fullname + ".hpp", mode='w')
    f.write("// " + fullname + ".hpp\n")
    f.write("// generated at: " + str(datetime.now()) + "\n")
    f.write("#ifndef " + fullname + "_hpp\n") 
    f.write("#define " + fullname + "_hpp\n\n") 

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
    createPoolsCPP = open(OUTPUT_CPP + 'populatePoolsInclude.cpp', mode='w')

    componentID = 0
    familyID = 0

    allComponents.write("// AllComponents.hpp\n")
    allComponents.write("// generated at: " + str(datetime.now()) + "\n")
    allComponents.write("""
#ifndef AllComponents_hpp
#define AllComponents_hpp\n
""")

    componentsEnum.write("// ComponentsEnum.hpp\n")
    componentsEnum.write("// generated at: " + str(datetime.now()) + "\n")
    componentsEnum.write("""
#ifndef ComponentsEnum_hpp
#define ComponentsEnum_hpp

enum class Components {
""")
    
    allFamilies.write("""
// AllFamilies.hpp\n
#ifndef AllFamilies_hpp
#define AllFamilies_hpp\n
""")

    for fname in os.listdir(INPUT_DIR):
        if fname.endswith(".json"):
            f = open(INPUT_DIR + fname, mode='r')
            createHeadersFromJSON(json.load(f))
            f.close()

    allComponents.write("\n#define NUM_COMPONENTS " + str(componentID) + "\n")
    allFamilies.write("\n#define NUM_FAMILIES " + str(familyID) + "\n")

    componentsEnum.write("\tComponentsCount = " + str(componentID) + "\n")
    componentsEnum.write("};\n")

    allComponents.write("#endif \n")
    componentsEnum.write("#endif \n")
    allFamilies.write("#endif \n")

    allComponents.close()
    allFamilies.close()
    componentsEnum.close()
    createPoolsCPP.close()


