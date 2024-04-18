//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_
#define OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_

#include <cstring>
#include <iostream>
#include <mutex>

#include "src/Common.hpp"

enum LogPriority {
  TracePriority,
  DebugPriority,
  InfoPriority,
  WarnPriority,
  ErrorPriority,
  CriticalPriority
};
class Logger {
 private:
  LogPriority m_priority = InfoPriority;
  std::mutex m_logMutex;

 public:
  template <typename... Args>
  static void Trace(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[TRACE]\t", TracePriority, msg, args...);
  }

  template <typename... Args>
  static void Debug(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[DEBUG]\t", DebugPriority, msg, args...);
  }

  template <typename... Args>
  static void Info(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[INFO]\t", InfoPriority, msg, args...);
  }

  template <typename... Args>
  static void Warn(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[WARN]\t", WarnPriority, msg, args...);
  }

  template <typename... Args>
  static void Error(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[ERROR]\t", ErrorPriority, msg, args...);
  }

  template <typename... Args>
  static void Critical(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, "[CRITICAL]\t", CriticalPriority, msg, args...);
  }

  template <typename... Args>
  static void LogError(int lineNumber, const char *sourceFile, const char *msg, Args... args) {
    instance().log(lineNumber, sourceFile, ErrorPriority, msg, args...);
  }

  static std::string GetGLError() {
    GLenum error_code = GL_NO_ERROR;
    std::string error;

    while ((error_code = glGetError()) != GL_NO_ERROR) {
      switch (error_code) {
        case GL_INVALID_ENUM:
          error += "GL_INVALID_ENUM";
          break;
        case GL_INVALID_VALUE:
          error += "GL_INVALID_VALUE";
          break;
        case GL_INVALID_OPERATION:
          error += "GL_INVALID_OPERATION";
          break;
        case GL_STACK_OVERFLOW:
          error += "GL_STACK_OVERFLOW";
          break;
        case GL_STACK_UNDERFLOW:
          error += "GL_STACK_UNDERFLOW";
          break;
        case GL_OUT_OF_MEMORY:
          error += "GL_OUT_OF_MEMORY";
          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
          error += "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:
          break;
      }
    }
    return error;
  }

  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

 private:
  Logger() = default;

  static Logger &instance() {
    static Logger instance;
    return instance;
  }

  // Function to print formatted message using recursion
  template <typename T, typename... Rest>
  static void printFormattedMessage(const char *format, T value, Rest... rest) {
    for (; *format != '\0'; format++) {
      if (*format == '%') {
        std::cout << value;
        printFormattedMessage(format + 2, rest...);
        return;
      }
      std::cout << *format;
    }
  }

  // Base case of the recursion when no more arguments left
  static void printFormattedMessage(const char *format) {
    for (; *format != '\0'; format++) {
      std::cout << *format;
    }
  }

  template <typename... Args>
  void log(int lineNumber, const char *sourceFile, const char *msgPriorityStr,
           LogPriority msgPriority, const char *msg, Args... args) {
    if (m_priority <= msgPriority) {
      std::scoped_lock lock(m_logMutex);
      std::cout << msgPriorityStr << lineNumber << " " << sourceFile << "\t";
      //      printf(msgPriorityStr);
      //      printf("LINE: %d FILE: %s\t", lineNumber, sourceFile);
      //      printf(msg, args...);
      printFormattedMessage(msg, args...);
      printf("\n");
    }
  }
};

// Macro to extract filename from path
#ifdef WINDOWS
#define FILENAME_ONLY (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define FILENAME_ONLY (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LOG_TRACE(Message, ...) Logger::Trace(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)
#define LOG_DEBUG(Message, ...) Logger::Debug(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)
#define LOG_INFO(Message, ...) Logger::Info(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)
#define LOG_WARN(Message, ...) Logger::Warn(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)
#define LOG_ERROR(Message, ...) Logger::Error(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)
#define LOG_CRITICAL(Message, ...) Logger::Critical(__LINE__, FILENAME_ONLY, Message, ##__VA_ARGS__)

#define GL_LOG_ERROR(...)                                                                        \
  do {                                                                                           \
    std::string error_##__LINE__##__FILE__ = Logger::GetGLError();                               \
    if (strcmp(error_##__LINE__##__FILE__.c_str(), "") != 0)                                     \
      Logger::Error(__LINE__, FILENAME_ONLY, error_##__LINE__##__FILE__.c_str(), ##__VA_ARGS__); \
  } while (0)

// #define GL_LOG_ERROR(...)

#define ASSERT(condition, message, ...)                    \
  if (!(condition)) {                                      \
    LOG_DEBUG("Assertion Failed", message, ##__VA_ARGS__); \
    std::abort();                                          \
  }

#endif  // OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_
