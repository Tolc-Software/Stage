#pragma once

#include "Stage/stage.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Stage {

/**
* Represents a CMake project that can be used to build and run tests.
* It inherits from a base project that it copies files from.
*
* It is not strictly necessary,
* but to enable some functionality assumptions about the baseStage are made:
* To enable setTargetName:
*     - The target in the base project CMakeLists.txt includes cmake/TargetName.cmake
*     - The target is set from the variable 'target_name'
* Ex: (CMakeLists.txt)
*     include(cmake/TargetName.cmake)
*     add_executable(${target_name} src/main.cpp)
* To get the source file from addSourceFile into a target:
*
*/
struct CMakeStage : public Stage {
	/**
	* Create a unique stage in baseStage/stages
	* Copy files/directories in pathsToCopy from baseStage to stage
	*/
	CMakeStage(std::filesystem::path const& baseStage,
	           std::vector<std::filesystem::path> const& pathsToCopy);

	/**
	* Set the name of the CMake target that will be built in the stage
	* Sets the variable 'target_name' in cmake/TargetName.cmake
	*/
	void setTargetName(std::string const& target);

	/**
	* Adds a file to the source directory in the stage
	* Returns the path to the file
	* Sets the variable 'sources' in cmake/Sources.cmake
	* Ex:
	*     addSourceFile("myFile.cpp", "int f() { return 5; }");
	* Results in:
	*     File m_stage/src/myfile.cpp
	*     with corresponding content
	*/
	std::filesystem::path addSourceFile(std::filesystem::path const& file,
	                                    std::string const& content);

	/*
	* Configure CMake project in stage
	* then build with reasonable defaults
	* Returns the exit code of the first command that fails or 0
	*/
	int configureAndBuild();

	// Source files
	std::vector<std::filesystem::path> m_sources = {};

	/*
	* Defines what to run on windows when running configureAndBuild
	*/
	void setWindowsCMakeBuildAndConfigureScript(std::string const& command);

private:
	/**
	* NOTE: This is not a cross platform function
	* Configure CMake project in stage
	* Provides reasonable defaults
	* Returns the exit code of the command
	*/
	int runCMakeConfigure(std::string const& compiler);

	/**
	* NOTE: This is not a cross platform function
	* Build CMake target
	* Returns the exit code of the command
	*/
	int buildCMakeProject();

	std::string m_windowsCMakeRunScript;
};
}
