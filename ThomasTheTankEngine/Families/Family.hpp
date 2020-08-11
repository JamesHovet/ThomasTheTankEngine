//
//  Family.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef Family_hpp
#define Family_hpp
#include "typedefs.h"

// see: https://stackoverflow.com/a/3829887 and https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
template <typename T>
struct Family {
    static componentMask mask;
    entityID eID;
};

template <typename T> componentMask Family<T>::mask{};

// a bit of a hack to allow lookup of a family index using <> notation.
namespace FamilyIndexTable {
    template <typename T>
    struct FamilyComponentIndex {
        static constexpr familyID familyIndex = T::familyIndex;
    };
}

#endif /* Family_hpp */
