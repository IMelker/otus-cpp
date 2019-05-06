//
// Created by imelker on 04.05.19.
//

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>
#include "scanner.h"

namespace ba = boost::adaptors;

FileScanner::FileScanner(int scan_level, uintmax_t min_file_size)
 : scan_level_(scan_level)
 , min_file_size_(min_file_size) {
  auto logger = TrivialLogger{};
  logger.logInfo("FileScanner init");
}

void can_do() {
  const std::string target_path( "/my/directory/" );
  const boost::regex my_filter( "somefiles.*\.txt" );
  boost::smatch what;

  for (auto &entry: boost::make_iterator_range(fs::directory_iterator(target_path), {})
      | ba::filtered(static_cast<bool (*)(const fs::path &)>(&fs::is_regular_file))
      | ba::filtered([&](const fs::path &path){ return boost::regex_match(path.filename().string(), what, my_filter); })
  )
  {
  // There are only files matching defined pattern "somefiles*.txt".
  std::cout << entry.path().filename() << std::endl;
  }
}

void FileScanner::recursiveScan(const fs::path& path,
                                const std::vector<fs::path> &excluded_paths,
                                const boost::regex &file_mask,
                                std::vector<File>& files,
                                int scan_level) {
  static auto logger = TrivialLogger{};
  logger.logInfo(fs::is_directory(path));
  boost::smatch what;
  if (scan_level == scan_level_) {
    for (auto &entry: boost::make_iterator_range(fs::directory_iterator(path), {})
        | ba::filtered(static_cast<bool (*)(const fs::path &)>(&fs::is_regular_file))
        | ba::filtered([&](const fs::path &path){ return fs::file_size(path) > min_file_size_;})
        | ba::filtered([&](const fs::path &path){ return boost::regex_match(path.filename().string(), what, file_mask); })
        ) {
      std::cout << entry.path().filename() << std::endl;
    }
  } else {

  }
}

std::vector<File> FileScanner::startScan(const std::vector<fs::path> &included_paths,
                                         const std::vector<fs::path> &excluded_paths,
                                         const boost::regex &file_mask) {
  auto files = std::vector<File>();
  int depth = 0;

  if (!file_mask.empty()) {
    for (auto&& path : included_paths) {
      if (fs::is_regular_file(path)) {

      } else {
        recursiveScan(path, excluded_paths, file_mask, files, ++depth);
      }

      /*if (fs::is_directory(path)) {

        for (fs::directory_entry& x : fs::directory_iterator(path)) {
          if (fs::is_directory(x)) {
            logger.logInfo(x.path());
          } else if (fs::is_regular_file(x)) {
            auto file_size = fs::file_size(x);
            logger.logWarning(x.path().string() + " = " + std::to_string(file_size));
            files.push_back({x.path().string(), file_size});
          }
        }
      }*/
    }
  }
  return files;
}

DuplicateScanner::DuplicateScanner(uintmax_t block_size, const std::string &hash_type)
  : block_size_(block_size)
  , hash_type_(resolveHashType(hash_type)) {
    auto logger = TrivialLogger{};
    logger.logInfo("DuplicateScanner init");
}

DuplicateScanner::HashType DuplicateScanner::resolveHashType(const std::string &hash_str) {
  auto hash_str_lower = boost::algorithm::to_lower_copy(hash_str);
  if (hash_str_lower == "md5") {
    return HashType::MD5;
  } else if (hash_str_lower == "crc32") {
    return HashType::CRC32;
  } else
    return HashType::NONE;
}

std::vector<Duplicates> DuplicateScanner::findDuplicates(const std::vector<File> &files) {



  return std::vector<Duplicates>();
}

