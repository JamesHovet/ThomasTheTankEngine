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
#include <iostream>
#include <string>

#if __APPLE__
#include <mach-o/dyld.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif

std::string FileUtils::getResourceAbsoluteFilePath(const char *resourcePathCStr){
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return boost::filesystem::path(std::wstring(buffer).substr(0, pos)).append("../ThomasTheTankEngine/Resources/").append(resourcePathCStr).string();
#elif __APPLE__
    char executablePathBuf [PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    #ifdef BUILD_CMAKE
        if(!_NSGetExecutablePath(executablePathBuf, &bufsize)){
            return boost::filesystem::path(std::string(executablePathBuf)).remove_filename().append("../ThomasTheTankEngine/Resources/").append(resourcePathCStr).string();
        }
    #else
        if(!_NSGetExecutablePath(executablePathBuf, &bufsize)){
            return boost::filesystem::path(std::string(executablePathBuf)).remove_filename().append("../Resources/").append(resourcePathCStr).string();
        }
    #endif
#else
    
#endif
    return nullptr;
}
