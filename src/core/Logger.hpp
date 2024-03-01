//
// Created by Tony Adriansen on 2/29/2024.
//

#ifndef OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_
#define OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_

#include <cstdio>
#include <mutex>

enum LogPriority {
  TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};
class Logger {
 private:
  LogPriority priority = InfoPriority;
  std::mutex logMutex;

 public:
  template<typename... Args>
  static void Trace(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[TRACE]\t", TracePriority, msg, args...);
  }

  template<typename... Args>
  static void Debug(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[DEBUG]\t", DebugPriority, msg, args...);
  }

  template<typename... Args>
  static void Info(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[INFO]\t", InfoPriority, msg, args...);
  }

  template<typename... Args>
  static void Warn(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[WARN]\t", WarnPriority, msg, args...);
  }

  template<typename... Args>
  static void Error(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[ERROR]\t", ErrorPriority, msg, args...);
  }

  template<typename... Args>
  static void Critical(int lineNumber, const char* source_file, const char* msg, Args... args) {
    instance().log(lineNumber, source_file, "[CRITICAL]\t", CriticalPriority, msg, args...);
  }

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

 private:
  Logger() = default;

  static Logger& instance() {
    static Logger instance;
    return instance;
  }

  template<typename... Args>
  void log(int lineNumber,
           const char* source_file,
           const char* msgPriorityStr,
           LogPriority msgPriority,
           const char* msg,
           Args... args) {
    if (priority <= msgPriority) {
      std::scoped_lock lock(logMutex);
      printf(msgPriorityStr);
      printf("LINE: %d FILE: %s\t", lineNumber, source_file);
      printf(msg, args...);
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

#ifdef DEBUG_MODE
#define LOG_TRACE(Message, ...) Logger::Trace(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)
#define LOG_DEBUG(Message, ...) Logger::Debug(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)
#define LOG_INFO(Message, ...) Logger::Info(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)
#define LOG_WARN(Message, ...) Logger::Warn(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)
#define LOG_ERROR(Message, ...) Logger::Error(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)
#define LOG_CRITICAL(Message, ...) Logger::Critical(__LINE__, FILENAME_ONLY, Message, __VA_ARGS__)

#define DEBUG_ASSERT(condition, message) \
if (!(condition)) { \
      LOG_DEBUG("Assertion Failed", message); \
      std::abort(); \
      }


#else
#define LOG_TRACE(Message, ...)
#define LOG_DEBUG(Message, ...)
#define LOG_INFO(Message, ...)
#define LOG_WARN(Message, ...)
#define LOG_ERROR(Message, ...)
#define LOG_CRITICAL(Message, ...)

#define DEBUG_ASSERT(condition, message) ((void)0)

#endif

#endif //OPENGL_RENDERER_SRC_CORE_LOGGER_HPP_
