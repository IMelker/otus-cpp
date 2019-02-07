//
// Created by imelker on 2/7/19.
//

#ifndef OTUS_CPP_BULK_COMPOSER_H
#define OTUS_CPP_BULK_COMPOSER_H

#include <memory>
#include <vector>
#include <ctime>
#include "executor.h"
class Executor;

class Command {
 public:
  Command(std::string&& name);
  const std::string& name();
 private:
  std::string command_name_;
};

class Bulk: public std::vector<Command> {
 public:
  Bulk();
  std::string generateFilename();

 private:
  std::time_t create_time_;
};

class BulkComposer {
 public:
  BulkComposer(int bulk_max_size);
  ~BulkComposer();

  void subscribe(std::shared_ptr<Executor> obs);
  void appendCommand(std::string command);
  void executeCommandsBulk();
 private:
  std::vector<std::shared_ptr<Executor>> executors;
  int bulk_max_size_;
  std::shared_ptr<Bulk> commands_bulk;
  bool brackets_mode_;
  int brackets_count_;
};
#endif //OTUS_CPP_BULK_COMPOSER_H
