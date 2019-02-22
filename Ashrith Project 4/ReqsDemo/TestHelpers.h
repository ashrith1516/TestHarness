#pragma once
///////////////////////////////////////////////////////////////////////
// TestHelpers.h - Helper code for displaying files and file lines   //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Ashrith Chandramouli								 //
// Source:	    Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

/*
* Supported Functions :
*	-showDirContents : Displays contents of a specified path
*	-showFileLines : Displays lines of code from a file
*
* Required Files :
*-------------- -
*	FileSystem.h (Static library)
*	StringUtilities.h
*
* Maintenance History :
*--------------------
* v1.1 : Migrated Helper code from Project#1
*/
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using Path = std::string;
using Message = std::string;
using Line = size_t;
using File = std::string;
using Files = std::vector<File>;

class TestHelpers
{
public:
	static bool showDirContents(const Path& path, const Message& msg = "");
	static bool showFileLines(const Path& path, Line start, Line end, const Message& msg = "");
};

//----< show all C++ source files in path >------------------------

inline bool TestHelpers::showDirContents(const Path& path, const Message& msg)
{
	if (msg.size() > 0)
		Utilities::title(msg);
	std::cout << "\n  " << FileSystem::Path::getFullFileSpec(path);
	if (!FileSystem::Directory::exists(path))
	{
		std::cout << "\n  path does not exist\n";
		return false;
	}

	Files files = FileSystem::Directory::getFiles(path);
	for (auto file : files)
	{
		std::string ext = FileSystem::Path::getExt(file);
		if (ext == "h" || ext == "cpp")
			std::cout << "\n    " << file;

	}
	std::cout << "\n";
	return true;
}
//----< show specified contiguous lines from a code file >---------

inline bool TestHelpers::showFileLines(const Path& path, Line start, Line end, const Message& msg)
{
	if (msg.size() > 0)
		Utilities::title(msg);
	std::cout << "\n  " << FileSystem::Path::getFullFileSpec(path);

	std::ifstream file(path);
	if (!file.good())
	{
		std::cout << "\n  can't open file";
		return false;
	}

	std::string line;
	size_t count = 0;
	while (std::getline(file, line))
	{
		if (++count < start) continue;
		if (count > end) break;
		std::cout << "\n  " << std::setw(4) << count << " " << line;
	}
	std::cout << "\n";
	return true;
}