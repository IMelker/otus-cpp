//
// Created by imelker on 03.03.19.
//

#ifndef OTUS_CPP_SCANNER_H
#define OTUS_CPP_SCANNER_H

#include "../5_task/helpers.h"

#include <vector>
#include <string>

class Scanner {
  friend void loadOptions(int argc, char **argv, Scanner &scanner);

  enum class HashType : int { md5, sha1, none };
 public:

 private:
  void setFileMask(const std::string& mask) {
    LOG(__PRETTY_FUNCTION__, " \"", mask, "\"");
    file_mask_= mask;
  };
  void setChecksumHash(const std::string& hash) {
    LOG(__PRETTY_FUNCTION__, " \"", hash, "\"");
    if (hash == "md5") {
      checksum_ = HashType::md5;
    } else if (hash == "sha1") {
      checksum_ = HashType::sha1;
    } else {
      checksum_ = HashType::none;
    }
  };

  std::vector<std::string> included_paths_;
  std::vector<std::string> excluded_paths;
  int scan_level_;
  std::size_t min_file_size_;
  std::string file_mask_;
  int block_size_;
  HashType checksum_;
};

#endif //OTUS_CPP_SCANNER_H
