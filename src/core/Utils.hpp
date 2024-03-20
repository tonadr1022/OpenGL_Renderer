//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
#define OPENGL_RENDERER_SRC_CORE_UTILS_HPP_


#include <fstream>
#include <sstream>
#include <chrono>
#include "src/core/Logger.hpp"

namespace Utils {
extern std::string LoadFromFile(const std::string& path);

extern std::string GetDateTimeString();

}

#endif //OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
