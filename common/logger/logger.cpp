//
// Created by imelker on 03.05.19.
//


#include "logger.h"

TrivialLogger::TrivialLogger(std::ostream& stream)
  : core_(logging::core::get()) {
  logging::add_console_log(stream);
}

TrivialLogger::TrivialLogger(const std::string& log_filepath)
  : core_(logging::core::get()) {
  logging::add_file_log(log_filepath);
}

void TrivialLogger::setMinLogLevel(LogLevel lvl) {
  core_->set_filter(logging::trivial::severity >= static_cast<logging::trivial::severity_level>(lvl));
}