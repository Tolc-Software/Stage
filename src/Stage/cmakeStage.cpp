#include "Stage/cmakeStage.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Stage {

CMakeStage::CMakeStage(std::filesystem::path const& baseStage,
                       std::vector<std::filesystem::path> const& pathsToCopy)
  : Stage::Stage(baseStage, pathsToCopy) {}

void CMakeStage::setTargetName(std::string const& target) {
  // Make sure it exists
  std::filesystem::create_directories(m_stage / "cmake");

  std::ofstream file(m_stage / "cmake" / "TargetName.cmake");
  file << "set(target_name \"" + target + "\")";
  file.close();
}

std::filesystem::path
CMakeStage::addSourceFile(std::filesystem::path const& file,
                          std::string const& content) {
  auto src = m_stage / "src";
  std::filesystem::create_directories(src);
  auto filepath = src / file;
  addFile(filepath, content);
  m_sources.push_back(filepath);

  // Add the sources variable
  std::filesystem::create_directories(m_stage / "cmake");
  std::ofstream cmakeFile(m_stage / "cmake" / "Sources.cmake");
  cmakeFile << "set(sources ";
  for (auto const& source : m_sources) {
    // This is to converts Windows strings (\) to CMake native ones (/)
    std::string s = source.string();
    std::replace(s.begin(), s.end(), '\\', '/');
    cmakeFile << s << '\n';
  }
  cmakeFile << ")";
  cmakeFile.close();

  return filepath;
}

int CMakeStage::configureAndBuild() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  // This has to be handled differently on Windows since it has to run the vcvars64.bat file to setup the environment
  if (auto error = runCommand(m_windowsCMakeRunScript); error != 0) {
    std::cerr << "Failed to run CMake configure or to build the project.\n";
    return error;
  }
#else
  // Assume it's available on Linux/MacOs
  std::string compiler = "clang++";

  // On Linux/MacOs
  if (auto configureError = runCMakeConfigure(compiler); configureError != 0) {
    std::cerr << "Failed to run CMake configure.\n";
    return configureError;
  }

  if (auto buildError = buildCMakeProject(); buildError != 0) {
    std::cerr << "Failed to build the project.\n";
    return buildError;
  }
#endif

  return 0;
}

int CMakeStage::runCMakeConfigure(std::string const& compiler) {
  return runCommand(
      "cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=\"" +
      compiler + "\"");
}

int CMakeStage::buildCMakeProject() {
  return runCommand("cmake --build build -j1");
}

void CMakeStage::setWindowsCMakeBuildAndConfigureScript(
    std::string const& command) {
  m_windowsCMakeRunScript = command;
}

}    // namespace Stage
