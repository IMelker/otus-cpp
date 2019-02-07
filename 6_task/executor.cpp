//
// Created by imelker on 2/7/19.
//

#include <iostream>
#include <fstream>
#include "executor.h"

static void pushToStream(std::ostream &stream, const std::shared_ptr<Bulk>& bulk) {
  if (bulk->size() == 0)
    return;

  stream << "bulk: ";
  for (auto it = bulk->begin(); it != bulk->end(); ++it) {
    if (it != bulk->begin())
      stream << ", ";
    stream << it->name();
  }
  stream << std::endl;
}

void Executor::subscribe(BulkComposer &composer) {
  composer.subscribe(shared_from_this());
}

void BulkWriter::execute(std::shared_ptr<Bulk> bulk) {
  std::ofstream bulk_log;
  bulk_log.open(bulk->generateFilename());
  pushToStream(bulk_log, bulk);
  bulk_log.close();
}

void BulkPrinter::execute(std::shared_ptr<Bulk> bulk) {
  pushToStream(std::cout, bulk);
}