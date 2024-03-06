//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
#define OPENGL_RENDERER_SRC_CORE_UTILS_HPP_


#include <fstream>
#include <sstream>
#include "src/core/Logger.hpp"

namespace Utils {
std::string LoadFromFile(const std::string& path) {
  std::ifstream file;
  std::stringstream lines;
  std::string line;

  file.open(path);
  if (!file.is_open()) {
    LOG_ERROR("File could not be opened: %s", path);
    // TODO: handle better
    return "";
  }

  while (std::getline(file, line)) {
    lines << line << '\n';
  }
  file.close();
  return lines.str();
}
}

#endif //OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
