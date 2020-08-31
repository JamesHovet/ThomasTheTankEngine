//
//  FileUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#pragma once
#ifndef FileUtils_h
#define FileUtils_h
#include <string>

namespace FileUtils
{
    std::string getResourceAbsoluteFilePath(const char* resourcePathCStr);
    std::string getResourceAbsoluteFilePath(std::string resourcePath);
}

#endif /* FileUtils_h */
