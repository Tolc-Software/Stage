#include "Stage/stage.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <random>

namespace {
/**
* Create a unique stage in rootStage/stages
* Returns the path to the newly created stage
*/
std::filesystem::path createUniqueStage(std::filesystem::path rootStage) {
	// Use to build a random string
	std::random_device r;
	std::default_random_engine rEngine(r());
	std::uniform_int_distribution<> uniform(0, std::numeric_limits<int>::max());
	auto stages = rootStage / "stages";

	// Ensure that the new stage does not exist before
	std::filesystem::path newStage = stages / std::to_string(uniform(rEngine));
	while (std::filesystem::exists(newStage)) {
		newStage = stages / std::to_string(uniform(rEngine));
	}

	std::filesystem::create_directories(newStage);

	return newStage;
}

}    // namespace

namespace Stage {
Stage::Stage(std::filesystem::path const& baseStage,
             std::vector<std::filesystem::path> const& pathsToCopy)
    : m_stage(createUniqueStage(baseStage)) {
	// Copy the inherited files from base to this stage
	// Inherits the files:
	//      * CMakeLists.txt
	//      * Anything in the cmake directory
	//      * Anything in the build/_deps directory
	namespace fs = std::filesystem;
	for (auto const& p : pathsToCopy) {
		if (fs::exists(baseStage / p)) {
			if (fs::is_directory(baseStage / p)) {
				fs::create_directories(m_stage / p);
				fs::copy(
				    baseStage / p, m_stage / p, fs::copy_options::recursive);
			} else {
				fs::copy(baseStage / p, m_stage / p);
			}
		}
	}
}

Stage::~Stage() {
	if (m_removeOnDestruction) {
		std::filesystem::remove_all(m_stage);
	}
}

std::filesystem::path Stage::addFile(std::filesystem::path const& file,
                                     std::string const& content) {
	auto filepath = m_stage / file;
	std::ofstream f(filepath);
	f << content;
	f.close();
	return filepath;
}

int Stage::runCommand(std::string const& cmd,
                      std::optional<std::filesystem::path> workingDirectory) {
	// Save to restore after function call
	auto originalDirectory = std::filesystem::current_path();

	auto cwd = m_stage;
	if (workingDirectory) {
		cwd = m_stage / workingDirectory.value();
		std::filesystem::create_directories(cwd);
	}
	std::filesystem::current_path(cwd);

	std::cout << cmd << '\n';
	auto errorCode = std::system(cmd.c_str());

	std::filesystem::current_path(originalDirectory);
	return errorCode;
}
}    // namespace Stage
