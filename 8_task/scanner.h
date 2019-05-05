//
// Created by imelker on 04.05.19.
//

#ifndef OTUS_CPP_8_TASK_SCANNER_H_
#define OTUS_CPP_8_TASK_SCANNER_H_

#include <vector>
#include <string>

struct ScannerOptions {
  std::vector<std::string> included_paths_;
  std::vector<std::string> excluded_paths_;
  int scan_level_;
  uintmax_t min_file_size_;
  int block_size_;
};

class File {
  std::string filepath;
  uintmax_t filesize;
};

class FileScanner {
 public:
  FileScanner(ScannerOptions& options) {

  }

  std::vector<File> startScan();

};

class Duplicates {
  std::vector<std::string> files;
  std::vector<std::string> caches;
};

class DuplicateScanner {
 public:
  DuplicateScanner() {

  }

  std::vector<Duplicates> findDuplicates(const std::vector<File>& files);
};

#endif //OTUS_CPP_8_TASK_SCANNER_H_
