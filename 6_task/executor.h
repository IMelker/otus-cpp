//
// Created by imelker on 2/7/19.
//

#ifndef OTUS_CPP_EXECUTOR_H
#define OTUS_CPP_EXECUTOR_H

#include "bulk_composer.h"
class Bulk;
class BulkComposer;

class Executor: public std::enable_shared_from_this<Executor> {
 public:
  virtual void execute(std::shared_ptr<Bulk> bulk) = 0;
  void subscribe(BulkComposer &composer);
};

class BulkWriter : public Executor  {
 public:
  void execute(std::shared_ptr<Bulk> bulk) override;
};

class BulkPrinter : public Executor {
 public:
  void execute(std::shared_ptr<Bulk> bulk) override;
};

#endif //OTUS_CPP_EXECUTOR_H
