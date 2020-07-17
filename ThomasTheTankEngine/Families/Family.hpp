//
//  Family.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef Family_hpp
#define Family_hpp

struct Family {
    
};

// a bit of a hack to allow lookup of a family index using <> notation.
namespace FamilyIndexTable {
    template <typename T>
    struct FamilyComponentIndex {
        static constexpr familyID familyIndex = T::familyIndex;
    };
}


#endif /* Family_hpp */
