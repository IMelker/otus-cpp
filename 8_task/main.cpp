//
// Created by imelker on 03.03.19.
//

#include <boost/program_options.hpp>
#include <logger/logger.h>
#include <arg_parser/arg_parser.h>
#include <vector>
#include <string>
#include <iostream>
#include "scanner.h"

/*Утилита должна иметь возможность через параметры командной строки
указывать:
- директории для сканирования (может быть несколько)
- директории для исключения из сканирования (может быть несколько)
- уровень сканирования (один на все директории, 0 - только указанная
директория без вложенных)
- минимальный размер файла, по умолчанию проверяются все файлы
    больше 1 байта.

- маски имен файлов разрешенных для сравнения (не зависят от
регистра)

- размер блока, которым производится чтения файлов, в задании этот
    размер упоминается как S
- один из имеющихся алгоритмов хэширования (crc32, md5 -
конкретные варианты определить самостоятельно), в задании
эта функция упоминается как H*/

namespace po = boost::program_options;

template <typename T>
std::ostream& operator <<(std::ostream &os, const std::vector<T>& v) {
  std::copy(v.cbegin(), v.cend(), std::ostream_iterator<T>(os, ";"));
  return os;
}

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
  auto logger = GlobalLogger{};

  auto args = ArgParser{
      Header("Generic"),
      Option("help,h", "Help screen"),
      Header("File scanner"),
      Option("include,i", "Include directories for scaning", std::vector<std::string>()),
      Option("exclude,e", "Exclude directories from scaning", std::vector<std::string>()),
      Option("level,l", "Directory scan level(depth)", int()),
      Option("min_file_size,s", "Minimal file size for scaning", uintmax_t()),
      Header("Duplicate scanner"),
      Option("read_block_size,n", "Size of read file binary block. If `0` file will be full loaded", int())
  };
  args.parse(argc, argv);

  auto included_paths = args.get<std::vector<std::string>>("include");
  auto excluded_paths = args.get<std::vector<std::string>>("exclude");
  auto scan_level = args.get<int>("level");
  auto min_file_size = args.get<uintmax_t>("min_file_size");
  auto block_size = args.get<int>("read_block_size");

  logger.logInfo("included_paths(" + std::to_string(included_paths.size()) + ") = " + std::to_string(included_paths));
  logger.logInfo("excluded_paths(" + std::to_string(excluded_paths.size()) + ") = " + std::to_string(excluded_paths));
  logger.logInfo("scan_level = " + std::to_string(scan_level));
  logger.logInfo("min_file_size = " + std::to_string(min_file_size));
  logger.logInfo("block_size = " + std::to_string(block_size));

  //auto file_scanner = FileScanner(opt);
  //auto filelist = file_scanner.startScan();
  //auto duplicate_scanner = DuplicateScanner();
  //auto duplicates_list = duplicate_scanner.findDuplicates(filelist);
  //logger.logInfo(std::to_string(duplicates_list));
}