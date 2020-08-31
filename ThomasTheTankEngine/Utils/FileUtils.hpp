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
#include <boost/filesystem.hpp>

namespace FileUtils
{
std::string getResourceAbsoluteFilePath(const char* resourcePathCStr);
std::string getResourceAbsoluteFilePath(std::string resourcePath);
std::string appendSuffixToPathLeaf(boost::filesystem::path path, std::string suffix);
}

#endif /* FileUtils_h */
