#pragma once


#include "location.hh"
#include "util/symtab.hpp"
#include "util/type/type.hpp"
#include "util/type/type_comparator.hpp"
#include <stack>


namespace pascc::semant {

/**
* @brief 语义分析的上下文
*/
class SemantContext
{
  friend class SemantVisitor;  // 友元类

public:
  /**
   * @brief 构建一个新的 SemantContext 对象
   */
  SemantContext();

  /**
    * @brief 进入局部作用域
    */
  void enterScope();

  /**
   * @brief 退出局部作用域
   */
  void exitScope();

  /**
   * 返回当前作用域的顶层函数名
   * 
   * @return std::string 当前作用域的顶层函数名
   */
  auto topFunc() -> std::string;

  /**
   * 向函数调用栈中压入函数
   * 
   * @param func_name 函数名
   */
  void pushFunc(const std::string &func_name);

  /**
   * @brief 从函数调用栈中弹出函数
   */
  void popFunc();

  /**
   * 生成错误消息
   * 
   * @param loc 位置信息
   * @param error_msg 错误信息
   */
  void genErrorMsg(const parse::location &loc, const std::string &error_msg);

private:
  std::vector<std::string> error_msgs_;                       ///< 错误信息存储向量

  util::SymTab<std::string, util::SymType *> typetab_;        ///< 类型符号表
  util::SymTab<std::string, util::SymType *> consttab_;       ///< 常量符号表
  util::SymTab<std::string, util::VarType *> vartab_;         ///< 变量符号表
  util::SymTab<std::string, util::SubprogType *> subprogtab_;  ///< 子程序符号表

  util::SubprogType::FormalVector formal_params_;              ///< 子程序的形式参数
  std::stack<std::string> functions_;                          ///< 函数调用栈
  util::SymType *case_stmt_type_{nullptr};                     ///< 存储case表达式类型信息

  util::TypeComparator cmp_;                                   ///<比较标志位
  bool inloop_ = false;                                        ///<标记是否处于循环内部
};

}  // namespace pascc::semant