//
// Created by imelker on 05.05.19.
//

#ifndef OTUS_CPP_COMMON_ARG_PARSER_ARG_PARSER_H_
#define OTUS_CPP_COMMON_ARG_PARSER_ARG_PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

/*
 * helpers
 */
template <typename T>
struct is_multitoken {
  static const bool value = false;
};

template <typename T, typename Alloc>
struct is_multitoken<std::vector<T,Alloc>> {
  static const bool value = true;
};

/*
 * options
 */
class Header {
 public:
  // кажется без rvalue при оптимизации компилятор сделает тот же эффект
  explicit Header(std::string&& name) : name_(std::move(name)) {};
  ~Header() = default;

  auto getName() const { return name_.c_str(); }
 private:
  std::string name_;
};

template <class T = std::nullptr_t>
class Option {
 public:
  Option(std::string&& name, std::string&& description, T&& type)
      : name_(std::move(name))
      , description_(std::move(description))
      , default_value_(std::move(type)) {
  }
  Option(std::string&& name, std::string&& description)
      : name_(std::move(name))
      , description_(std::move(description)) {
  }
  ~Option() = default;

  auto getName() const { return name_.c_str(); }
  auto getDescription() const { return description_.c_str(); }
  auto getDefault() const { return default_value_; }

 private:
  std::string name_;
  std::string description_;
  T default_value_;
};

/*
 * Argument parser
 */
namespace po = boost::program_options;

class ArgParser {
 public:
  template <typename ...Args>
  ArgParser(Args... args) : exit_on_help(true) {
    staticDescFill(cmdline_options_, args...);
  }

  void setExitOnHelp(bool exit) {
    exit_on_help = exit;
  }

  void parse(int argc, char *argv[]) {
    store(po::parse_command_line(argc, argv, cmdline_options_), vm_);
    if (exit_on_help && vm_.count("help")) {
      std::cout << cmdline_options_ << "\n";
      std::exit(0);
    }
    po::notify(vm_);
  }

  template <typename T>
  T get(const std::string& key) const {
    return vm_.at(key).as<T>();
  }

 private:
  void staticDescFill(po::options_description& desc) { cmdline_options_.add(desc); };
  template <typename Arg, typename ...Args>
  void staticDescFill(po::options_description& desc, Arg arg, Args... args);

  po::options_description cmdline_options_;
  po::variables_map vm_;
  bool exit_on_help;
};

template <typename Arg, typename ...Args>
void ArgParser::staticDescFill(po::options_description& desc, Arg option, Args... args) {
  if constexpr (std::is_same_v<Header, Arg>) {
    if (&cmdline_options_ != &desc) {
      cmdline_options_.add(desc);
    }
    auto new_desc = po::options_description{option.getName()};
    staticDescFill(new_desc, args...);
  } else {
    using v_type = decltype(option.getDefault());
    auto name = option.getName();
    auto description = option.getDescription();
    if constexpr (is_multitoken<v_type>::value) {
      desc.add_options()(name, po::value<v_type>()->multitoken(), description);
    } else if constexpr (std::is_same_v<std::nullptr_t, v_type>/*empty v*/) {
      desc.add_options()(name, description);
    } else {
      desc.add_options()(name, po::value<v_type>()->default_value(option.getDefault()), description);
    }
    staticDescFill(desc, args...);
  }
}

#endif //OTUS_CPP_COMMON_ARG_PARSER_ARG_PARSER_H_
