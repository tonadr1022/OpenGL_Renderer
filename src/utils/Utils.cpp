//
// Created by Tony Adriansen on 3/20/24.
//

#include "Utils.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

#include "src/utils/Logger.hpp"

namespace Utils {

std::string LoadFromFile(const std::string& path) {
  std::ifstream file;
  std::stringstream lines;
  std::string line;

  file.open(path);
  if (!file.is_open()) {
    LOG_ERROR("File could not be opened: %s", path);
    return "";
  }

  while (std::getline(file, line)) {
    lines << line << '\n';
  }
  file.close();
  return lines.str();
}

std::string GetDateTimeString() {
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d_%H-%M-%S");
  return ss.str();
}
}  // namespace Utils
