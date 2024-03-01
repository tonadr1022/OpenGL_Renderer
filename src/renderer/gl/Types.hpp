//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_RENDERER_GL_TYPES_HPP_
#define OPENGL_RENDERER_SRC_RENDERER_GL_TYPES_HPP_

#include <glad/glad.h>

namespace GL {

enum BufferType : int {
  ARRAY = GL_ARRAY_BUFFER,
  ELEMENT = GL_ELEMENT_ARRAY_BUFFER
};

enum DrawMode : int {
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW,
  STREAM = GL_STREAM_DRAW,
};

}
#endif //OPENGL_RENDERER_SRC_RENDERER_GL_TYPES_HPP_
