#include <filesystem>

#include "argparse/argparse.hpp"
#include "driver/driver.hpp"

using pascc::driver::Driver;
namespace fs = std::filesystem;

auto main(int argc, char *argv[]) -> int
{
  argparse::ArgumentParser program("pascc");

  program.add_argument("-i", "--input")
      .help("input file")
      .required();

  program.add_argument("-o", "--output")
      .help("output file");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
    std::cerr << program;
    exit(EXIT_FAILURE);
  }

  auto input_file = program.get<std::string>("-i");
  fs::path path(input_file);
  path.replace_extension(".c");
  auto output_file{path.string()};

  if (program.is_used("-o")) {
    output_file = program.get<std::string>("-o");
  }

  Driver::get()
      .parse(input_file)
      .check()
      .codegen(output_file);
  return 0;
}