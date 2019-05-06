//
// Created by imelker on 04.05.19.
//

#ifndef OTUS_CPP_8_TASK_SCANNER_H_
#define OTUS_CPP_8_TASK_SCANNER_H_

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <logger/logger.h>

namespace fs = boost::filesystem;

struct File {
  std::string filepath;
  uintmax_t filesize;
};

struct Duplicates {
  std::vector<File> files;
  std::vector<std::string> caches;
};

class FileScanner {
 public:
  FileScanner(int scan_level, uintmax_t min_file_size);
  std::vector<File> startScan(const std::vector<fs::path> &included_paths,
                              const std::vector<fs::path> &excluded_paths,
                              const boost::regex &file_mask);

 private:
  void recursiveScan(const fs::path& path,
                     const std::vector<fs::path> &excluded_paths,
                     const boost::regex &file_mask,
                     std::vector<File>& files,
                     int scan_level);

  int scan_level_;
  uintmax_t min_file_size_;
};

class DuplicateScanner {
  enum class HashType {
    MD5,
    CRC32,
    NONE
  };
 public:
  DuplicateScanner(uintmax_t block_size, const std::string& hash_type);

  HashType resolveHashType(const std::string& hash_str);
  std::vector<Duplicates> findDuplicates(const std::vector<File>& files);
 private:
  uintmax_t block_size_;
  HashType hash_type_;
};

#endif //OTUS_CPP_8_TASK_SCANNER_H_
