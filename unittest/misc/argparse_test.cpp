#include "argparse/argparse.hpp"
#include "gtest/gtest.h"

/**
 * @brief 测试输入参数
 */
TEST(ArgparseTest, input)
{
  // 1: ./pascc -i input.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-i", "--input")
        .help("input file")
        .required();

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("-i"),
        static_cast<const char *>("input.txt")
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-i"));
    EXPECT_TRUE(program.is_used("--input"));
    EXPECT_EQ("input.txt", program.get<std::string>("-i"));
  }

  // 2: ./pascc --input input.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-i", "--input")
        .help("input file")
        .required();

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("--input"),
        static_cast<const char *>("input.txt")
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-i"));
    EXPECT_TRUE(program.is_used("--input"));
    EXPECT_EQ("input.txt", program.get<std::string>("-i"));
  }

  // 3: ./pascc --input=input.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-i", "--input")
        .help("input file")
        .required();

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("--input=input.txt"),
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-i"));
    EXPECT_TRUE(program.is_used("--input"));
    EXPECT_EQ("input.txt", program.get<std::string>("-i"));
  }
}

/**
 * @brief 测试输出参数
 */
TEST(ArgparseTest, output)
{
  // 1: ./pascc -o output.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-o", "--output")
        .help("output file");

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("-o"),
        static_cast<const char *>("output.txt")
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-o"));
    EXPECT_TRUE(program.is_used("--output"));
    EXPECT_EQ("output.txt", program.get<std::string>("-o"));
  }

  // 2: ./pascc --output output.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-o", "--output")
        .help("output file");

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("--output"),
        static_cast<const char *>("output.txt")
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-o"));
    EXPECT_TRUE(program.is_used("--output"));
    EXPECT_EQ("output.txt", program.get<std::string>("-o"));
  }

  // 3: ./pascc --output=output.txt
  {
    argparse::ArgumentParser program("pascc");

    program.add_argument("-o", "--output")
        .help("output file");

    std::vector<const char *> argv = {
        static_cast<const char *>("./pascc"),
        static_cast<const char *>("--output=output.txt"),
    };

    try {
      program.parse_args(static_cast<int>(argv.size()), argv.data());
    } catch (const std::exception &err) {
      std::cerr << err.what() << '\n';
      std::cerr << program;
      exit(EXIT_FAILURE);
    }

    EXPECT_TRUE(program.is_used("-o"));
    EXPECT_TRUE(program.is_used("--output"));
    EXPECT_EQ("output.txt", program.get<std::string>("-o"));
  }
}