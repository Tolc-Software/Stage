#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Stage {

struct Stage {
	/**
	* Create a unique stage in baseStage/stages
	* Copy files/directories in pathsToCopy from baseStage to stage
	*/
	Stage(std::filesystem::path const& baseStage,
	      std::vector<std::filesystem::path> const& pathsToCopy);

	/**
	* Removes the stage and everything in it from the filesystem
	*/
	~Stage();

	/**
	* Adds a file to the stage
	* Returns the path to the file
	* file here is relative to the root of the stage
	*/
	std::filesystem::path addFile(std::filesystem::path const& file,
	                              std::string const& content);

	/**
	* Runs a command within the test stage
	* workingDirectory is relative the stage root
	* Returns the exit code of the command
	*/
	int runCommand(
	    std::string const& cmd,
	    std::optional<std::filesystem::path> workingDirectory = std::nullopt);

	// Path to the generated stage
	std::filesystem::path m_stage;

	// If the stage should be removed when this object is destroyed
	bool m_removeOnDestruction = true;
};
}    // namespace Stage
