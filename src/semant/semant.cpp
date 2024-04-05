#include "semant/semant.hpp"

namespace pascc::semant {

auto SemantVisitor::isOk() -> bool
{
  return context_.error_count() == 0;
}

void SemantVisitor::visit([[maybe_unused]] ast::Block &node)
{
  /**
   顺次访问 各个part
   如果如果 stmt_part 为空，返回错误，程序终止。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Number &node)
{
  /*
  为父类 Expr 赋值 type = type, value = value , isassignable = 0;
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Constant &node)
{
  /**
  当 type == "reference" 且 sign = -1 时查符号表(找不到则返回未定义的错误)，若为字符串类型，则返回错误，程序终止。
 */

  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::StringLiteral &node)
{
  /**
    为父类Expr 赋值 type = string , value = NAN , isassignable = 0; 
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Expr &node)
{
  /**
   1. 应该具有变量 type value isAssignable
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
  /*
    为父类type, value, isAssignable = 0;赋值
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::BinaryExpr &node)
{
  /**
    (type 判等时忽略 const)
    当二元运算符为 'and' 或 'or', 左右两侧必然同时为 bool，否则返回错误，程序终止。
      type 赋值为 bool
    当二元运算符为 ‘/’ 、'mod' 、 '+' 、'-'、'*'，左右两侧必须为integer或real，否则返回错误，程序终止。
      只有全为 integer 时，type 赋值为 integer，其余情况均为real。
    当二元运算符为 ‘/’或'mod' , 右侧的Exp若为 const，则不能为0，否则返回错误，程序终止。
    当二元运算符为 '=' ，左侧的Exp必然是可赋值的，否则返回错误，程序终止。
      当左右两侧类型表达式不相等，且不满足左边是real，右边是int，返回错误，程序终止。
      当左侧是 real，右侧是 int， type 赋值为 real。
      否则 type 赋值为左侧的类型表达式
    
    只有当二元运算符为 '=' 时，isAssignable = 1 其余情况为 0;
    按照运算规则处理value。
   */

  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::UnaryExpr &node)
{
  /**
   当一元运算符为 '-' 或 '+'，则 expr_ 必然是 integer 或 real 类型，否则返回错误，程序终止。
   当一元运算符为 'not' 时，则 expr_ 必然是 bool 类型，否则返回错误，程序终止。
   isAssignable = 0;
   按照运算规则处理value。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncCall &node)
{
  /**
   在符号表中查找声明，若找不到，则返回错误，程序终止。
   获取FuncCall类型表达式。
   对actuals_中的每一个参数进行类型检查，若存在不相等的情况，则返回错误，程序终止。
   type = FuncCall的return value
   isAssignable = 0
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Assignable &node)
{
  /**
   父类 Expr 的 isAssignable = 1;
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::AssignableId &node)
{
  /*
   在符号表中查找声明，若找不到，则返回错误，程序终止。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::IndexedVar &node)
{
  /**
    1. 获取 assignable 的类型表达式。
    2. 获取每一个维度上的上下界
      2.1 若无法获取上下界，说明这个变量不是数组，返回错误，程序终止。
      2.2 若对于第 i 个 indices 
          * 没有它对应的上下界，
          * 或它为常量，且超出了第 i 个 indices 对应的上下界，
          * 或它不为整数类型。
          则返回错误，程序终止。
    2. 获取 assignable 数组元素的类型表达式，记录在父类 Expr 的 type 中。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FieldDesignator &node)
{
  /**
    1. 获取 assignable_ 的类型表达式。
    2. 在符号表中匹配 field_
      2.1 若结构体中不包含 field_ 打印错误，程序终止。
    3. 获取 field_ 的类型表达式并记录在父类 Expr 的 type 中。
      (这表示整个表达式的类型，将会用在后续的类型检查中)。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ConstDecl &node)
{
  /**
   判断有无重定义。
   向符号表中插入<id , constant->type()>
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ConstDeclPart &node)
{
  /*
   访问各个 ConstDecl
  */

  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeId &node)
{
  /**
   检测 type 是否在符号表中
   赋值给父类 TypeDenoter 的 type
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Period &node)
{
  /**
   检测上界是否大于下界。 
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ArrayType &node)
{
  /**
   组装 type 和 periods 打包赋值给父类 TypeDenoter 的 type
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::RecordType &node)
{
  /**
    将fields中的所有field的 TypeDenoter,id_list_ 打包赋值给父类 TypeDenoter 的type
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDecl &node)
{
  /**
    符号表重定义检测。
    符号表中新增一项<type_id_, type_denoter_.type()>
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDeclPart &node)
{
  /*
  访问各个TypeDecl
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDenoter &node)
{
  /**
    需要一个变量 type
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarDeclPart &node)
{
  /**
   访问各个 VarDecl 
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ValueParamSpec &node)
{
  /**
    将id_list_ , type_denoter_ 赋值给父类的对应变量。
    isreference = 0;
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarParamSpec &node)
{
  /**
    将id_list_ , type_denoter_ 赋值给父类的对应变量。
    isreference = 1;
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::VarDecl &node)
{
  /*
    查找符号表，检测重定义
    插入符号表
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcHead &node)
{
  /**
   把每一个 FormalParam 组装起来，再和id组装，插入符号表
   符号表进入下一级。
   对于每一个FormalParam 符号表判断重定义，再加入<id_list, typedenoter>
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcBlock &node)
{
  /**
   顺次访问 各个part
   如果 stmt_part 为空，返回错误，程序终止。
   符号表返回上一级
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcDecl &node)
{
  /*
  访问 head block
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncHead &node)
{
  /**
   把每一个 FormalParam 组装起来，再和id、return组装，插入符号表
   符号表进入下一级。
   对于每一个FormalParam 符号表判断重定义，再加入<id_list, typedenoter>
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncBlock &node)
{
  /**
   顺次访问 各个part
   如果 stmt_part 为空，返回错误，程序终止。
   符号表返回上一级
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FuncDecl &node)
{
  /*
  访问 head block
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::FormalParam &node)
{
  /**
   应该具有变量 idlist , type , isreference
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::SubprogDecl &node)
{
  /*
    应该具有变量 type
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::SubprogDeclPart &node)
{
  /*
    访问各个 subprog_decl
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Stmt &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::IfStmt &node)
{
  /**
    访问 then, 访问 else
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CaseStmt &node)
{
  /** 
    访问各个 CaseListElement
    如果 Expr 类型和标识条件常量的类型不一致则返回错误，程序退出。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CaseListElement &node)
{
  /**
  访问Stmt
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::RepeatStmt &node)
{
  /** 
  访问body
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WhileStmt &node)
{
  /** 
  访问body
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
  /** 
  如果ctrl_var和init_val类型不一致则返回错误，程序终止。
  访问body
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::AssignStmt &node)
{
  /*
   如果左右类型不一致则返回错误，程序终止。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
  /**
   在符号表中查找声明，若找不到，则返回错误，程序终止。
   获取ProcCall类型表达式。
   对actuals_中的每一个参数进行类型检查，若存在不相等的情况，则返回错误，程序终止。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
  /*
  对actuals_中的每一个参数进行类型检查。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  /*
  对actuals_中的每一个参数进行类型检查。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
  /*
  对actuals_中的每一个参数进行类型检查。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  /*
  对actuals_中的每一个参数进行类型检查。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  /*
  对actuals_中的每一个参数进行类型检查。
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
  /*
    访问各个Stmt
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::StmtPart &node)
{
  /*
    访问各个Stmt
  */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramBlock &node)
{
  /**
   顺次访问 各个part
   如果 stmt_part 为空，返回错误，程序终止。
   */
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
  throw std::runtime_error("Not implemented");
}

void SemantVisitor::visit([[maybe_unused]] ast::Program &node)
{
  // Program -> ProgramHead ';' ProgramBlock '.'
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
