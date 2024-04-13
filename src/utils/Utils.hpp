//
// Created by Tony Adriansen on 3/2/24.
//

#ifndef OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
#define OPENGL_RENDERER_SRC_CORE_UTILS_HPP_

#include <string>

namespace Utils {
extern std::string LoadFromFile(const std::string& path);

extern std::string GetDateTimeString();

}  // namespace Utils

#endif  // OPENGL_RENDERER_SRC_CORE_UTILS_HPP_
