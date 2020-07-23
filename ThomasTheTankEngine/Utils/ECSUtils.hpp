//
//  ECSUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 7/17/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef ECSUtils_hpp
#define ECSUtils_hpp

#include "typedefs.h"
namespace ECSUtils {
inline bool doesPassFilter(componentMask component, componentMask filter){
    return (component & filter) == filter;
}
}
#endif /* ECSUtils_hpp */
