//
// Created by imelker on 03.03.19.
//

#include <boost/program_options.hpp>
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

void loadOptions(int argc, char **argv, Scanner &scanner) {
    po::options_description desc{"Options"};
    desc.add_options()
        ("help,h", "Help screen")
        ("include,i", po::value<std::vector<std::string>>(&scanner.included_paths_), "Include directories for scaning")
        ("exclude,e", po::value<std::vector<std::string>>(&scanner.excluded_paths), "Exclude directories from scaning")
        ("level,l", po::value<int>(&scanner.scan_level_)->default_value(0), "Directory scan level(depth)")
        ("min_file_size", po::value<std::size_t>(&scanner.min_file_size_)->default_value(1), "Minimal file size for scaning")
        ("file_mask,m", po::value<std::string>()->default_value("")->notifier(
            std::bind(&Scanner::setFileMask, &scanner, std::placeholders::_1)), "File mask(regexp) for scaning")
        ("read_block_size,n", po::value<int>(&scanner.block_size_)->default_value(0), "Size of read file binary block. If `0` file will be full loaded")
        ("hash", po::value<std::string>()->default_value("")->notifier(
            std::bind(&Scanner::setChecksumHash, &scanner, std::placeholders::_1)), "Algorithm for hashing block");
    po::variables_map vm;
    store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
}

int main (int argc, char *argv[]) {

  // load options
  // check if all is right
  // create scanner
  // setOptions for it
  // start scaning
  Scanner duplicate_scanner();

  ScannerOptions options;
  try {
    loadOptions(argc, argv, options);
  } catch (const po::error &ex) {
    std::cout << "Wrong options: " << ex.what() << '\n';
    exit(1);
  }

  Scanner duplicate_scanner(options);
  duplicate_scanner.startScan();
  duplicate_scanner.printDuplicates();
}