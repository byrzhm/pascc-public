#pragma once

#include "ast/ast.hpp"

namespace pascc::driver {

class Driver
{
public:
  /**
   * @brief 获取 Driver 单例
   */
  static auto Get() -> Driver &;

  /**
   * @brief 解析pascal源文件, 生成抽象语法树
   *
   * @param filepath 文件路径
   */
  auto parse(const std::string &filepath) -> Driver &;

  /**
   * @brief 语义分析
   */
  auto check() -> Driver &;

  /**
   * @brief 代码生成
   */
  auto codegen() -> Driver &;

  /**
   * @brief 获取抽象语法树
   */
  auto program() -> ast::Program & { return *program_; }

private:
  Driver() = default;
  std::unique_ptr<ast::Program> program_;
};

}  // namespace pascc::driver