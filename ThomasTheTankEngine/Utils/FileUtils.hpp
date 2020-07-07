//
//  FileUtils.hpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#ifndef FileUtils_h
#define FileUtils_h
#include <string>
#include <limits.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <mach-o/dyld.h>


namespace FileUtils
{
    std::string getResourceAbsoluteFilePath(const char* resourcePathCStr);
}

#endif /* FileUtils_h */
