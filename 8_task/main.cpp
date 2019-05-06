//
// Created by imelker on 03.03.19.
//

#include <vector>
#include <string>
#include <iostream>
#include <logger/logger.h>
#include <arg_parser/arg_parser.h>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "scanner.h"

namespace po = boost::program_options;

namespace std {
  // for educational purposes
  template <typename T>
  string to_string(const vector<T>& v) {
    stringstream stream;
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<T>(stream, ";"));
    return stream.str();
  }
}

int main (int argc, char *argv[]) {
  auto logger = TrivialLogger{};

  auto args = ArgParser{
      Header("Generic"),
      Option("help,h", "Help screen"),
      Option("version,v", "Program version"),
      Header("File scanner"),
      Option("include,i", "Include directories for scanning", std::vector<boost::filesystem::path>()),
      Option("exclude,e", "Exclude directories from scanning", std::vector<boost::filesystem::path>()),
      Option("level,l", "Directory scan level(depth)", -1),
      Option("min_file_size,s", "Minimal file size for scanning. `0` = full size", uintmax_t(0)),
      Option("file_mask,m", "File mask for scanning", boost::regex(".*.")),
      Header("Duplicate scanner"),
      Option("read_block_size,n", "Read binary block size. `0` = full size", uintmax_t(0)),
      Option("hash_type,t", "Hashing algorithm type: md5, crc32", std::string("md5"))
  };
  args.parse(argc, argv);

  auto scan_level = args.get<int>("level");
  auto min_file_size = args.get<uintmax_t>("min_file_size");
  auto included_paths = args.get<std::vector<boost::filesystem::path>>("include");
  auto excluded_paths = args.get<std::vector<boost::filesystem::path>>("exclude");
  auto file_mask = args.get<boost::regex>("file_mask");
  auto block_size = args.get<uintmax_t>("read_block_size");
  auto hash_type = args.get<std::string>("hash_type");

  logger.logDebug("included_paths(" + std::to_string(included_paths.size()) + ") = " + std::to_string(included_paths));
  logger.logDebug("excluded_paths(" + std::to_string(excluded_paths.size()) + ") = " + std::to_string(excluded_paths));
  logger.logDebug("scan_level = " + std::to_string(scan_level));
  logger.logDebug("min_file_size = " + std::to_string(min_file_size));
  logger.logDebug("block_size = " + std::to_string(block_size));
  logger.logDebug("file_mask = " + file_mask.str());
  logger.logDebug("hash_type = " + hash_type);

  auto file_scanner = FileScanner(scan_level, min_file_size);
  auto filelist = file_scanner.startScan(included_paths, excluded_paths, file_mask);

  auto duplicate_scanner = DuplicateScanner(block_size, hash_type);
  auto duplicates_list = duplicate_scanner.findDuplicates(filelist);

  //std::cout << duplicates_list << std::endl;
}