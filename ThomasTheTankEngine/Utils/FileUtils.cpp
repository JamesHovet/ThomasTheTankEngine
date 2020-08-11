//
//  FileUtils.cpp
//  ThomasTheTankEngine
//
//  Created by James Hovet on 3/26/20.
//  Copyright © 2020 James Hovet. All rights reserved.
//

#include "FileUtils.hpp"
#include <limits.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <mach-o/dyld.h>

std::string FileUtils::getResourceAbsoluteFilePath(const char *resourcePathCStr){
    char executablePathBuf [PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if(!_NSGetExecutablePath(executablePathBuf, &bufsize)){
        return boost::filesystem::path(std::string(executablePathBuf)).remove_filename().append("../Resources/").append(resourcePathCStr).string();
    }
    return nullptr;
}
