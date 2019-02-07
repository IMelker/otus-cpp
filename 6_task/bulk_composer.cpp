//
// Created by imelker on 2/7/19.
//

#include "bulk_composer.h"

Command::Command(std::string&& name)
  : command_name_(std::forward<std::string>(name)) {
}

const std::string &Command::name() {
  return command_name_;
}

Bulk::Bulk() : create_time_(std::time(nullptr)) {
}

std::string Bulk::generateFilename() {
  return "bulk" + std::to_string(create_time_) + ".log";
}

BulkComposer::BulkComposer(int bulk_max_size)
  : bulk_max_size_(bulk_max_size)
  , commands_bulk(std::make_shared<Bulk>())
  , brackets_mode_(false)
  , brackets_count_(0) {
}

void BulkComposer::subscribe(std::shared_ptr<Executor> obs) {
  executors.emplace_back(std::forward<std::shared_ptr<Executor>>(obs));
}

void BulkComposer::appendCommand(std::string command) {
  if (command == "{") {
    if (!brackets_count_ && commands_bulk && !commands_bulk->empty()) {
      executeCommandsBulk();
    }
    brackets_mode_ = true;
    brackets_count_++;
  } else if (command == "}") {
    if (brackets_count_ != 0) {
      brackets_count_--;
    }
  } else {
    if (!commands_bulk) {
      commands_bulk = std::make_shared<Bulk>();
    }
    commands_bulk->emplace_back(std::move(command));
  }

  if (brackets_mode_) {
    if (brackets_count_ == 0) {
      brackets_mode_ = false;
      executeCommandsBulk();
    }
  } else {
    if (commands_bulk->size() >= static_cast<size_t >(bulk_max_size_)) {
      executeCommandsBulk();
    }
  }
}

void BulkComposer::executeCommandsBulk() {
  for (auto& e : executors) {
    e->execute(commands_bulk);
  }
  commands_bulk.reset();
}

BulkComposer::~BulkComposer() {
  if (!brackets_mode_ && commands_bulk) {
    executeCommandsBulk();
  }
}
