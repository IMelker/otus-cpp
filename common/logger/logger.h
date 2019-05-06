//
// Created by imelker on 03.05.19.
//

#ifndef OTUS_CPP_COMMON_LOGGER_LOGGER_H_
#define OTUS_CPP_COMMON_LOGGER_LOGGER_H_

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;

// copy definition of boost trivial log level
enum class LogLevel : std::underlying_type<logging::trivial::severity_level>::type {
  Trace = logging::trivial::severity_level::trace,
  Debug = logging::trivial::severity_level::debug,
  Info = logging::trivial::severity_level::info,
  Warning = logging::trivial::severity_level::warning,
  Error = logging::trivial::severity_level::error,
  Fatal = logging::trivial::severity_level::fatal
};

class TrivialLogger {
 public:
  TrivialLogger(std::ostream& stream = std::clog);
  TrivialLogger(const std::string& log_filepath);
  ~TrivialLogger() = default;

  void setMinLogLevel(LogLevel lvl);

  template <typename T>
  void logTrace(T&& value) {
    log(LogLevel::Trace, std::forward<T>(value));
  }
  template <typename T>
  void logDebug(T&& value) {
    log(LogLevel::Debug, std::forward<T>(value));
  }
  template <typename T>
  void logInfo(T&& value) {
    log(LogLevel::Info, std::forward<T>(value));
  }
  template <typename T>
  void logWarning(T&& value) {
    log(LogLevel::Warning, std::forward<T>(value));
  }
  template <typename T>
  void logError(T&& value) {
    log(LogLevel::Error, std::forward<T>(value));
  }
  template <typename T>
  void logFatal(T&& value) {
    log(LogLevel::Fatal, std::forward<T>(value));
  }

 private:
  template <typename T>
  void log(LogLevel lvl, T&& value);

  logging::core_ptr core_;
};

template <typename T>
void TrivialLogger::log(LogLevel lvl, T&& value) {
  switch (lvl) {
    case LogLevel::Trace:
      BOOST_LOG_TRIVIAL(trace) << "[Trace] " << value;
      break;
    case LogLevel::Debug:
      BOOST_LOG_TRIVIAL(debug) << "[Debug] " << value;
      break;
    case LogLevel::Error:
      BOOST_LOG_TRIVIAL(error) << "[Error] " << value;
      break;
    case LogLevel::Fatal:
      BOOST_LOG_TRIVIAL(fatal) << "[Fatal] " << value;
      break;
    case LogLevel::Warning:
      BOOST_LOG_TRIVIAL(trace) << "[Warning] " << value;
      break;
    case LogLevel::Info:
    default:
      BOOST_LOG_TRIVIAL(info) << "[Info] " << value;
  }
}

#endif //OTUS_CPP_COMMON_LOGGER_LOGGER_H_
