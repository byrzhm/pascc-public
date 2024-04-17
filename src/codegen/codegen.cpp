#include <iostream>

#include "codegen/codegen.hpp"

namespace {

constexpr int INDENT_SIZE = 2;

class IndentGuard
{
public:
  IndentGuard(int *indent, int indent_size)
    : indent_(indent)
    , indent_size_(indent_size)
  {
    *indent += indent_size_;
  }

  ~IndentGuard()
  {
    *indent_ -= indent_size_;
  }

private:
  int *indent_ = nullptr;
  int indent_size_;
};

}  // namespace

namespace pascc::codegen {

static auto indent(int n) -> std::string
{
  return std::string(n, ' ');
}

auto CodegenVisitor::println(const std::string &str) -> void
{
  printIndent();
  print(str + "\n");
}
auto CodegenVisitor::printIndent() -> void
{
  (file_output_ ? fout_ : std::cout) << indent(indent_);
}

auto CodegenVisitor::print(const std::string &str) -> void
{
  (file_output_ ? fout_ : std::cout) << str;
}

auto CodegenVisitor::print(const int &x) -> void
{
  (file_output_ ? fout_ : std::cout) << x;
}

auto CodegenVisitor::print(const double &d) -> void
{
  (file_output_ ? fout_ : std::cout) << d;
}

auto to_string(const ast::BinOp &op) -> std::string
{
  switch (op) {
    case ast::BinOp::PLUS: return "+";
    case ast::BinOp::MINUS: return "-";
    case ast::BinOp::MUL: return "*";
    case ast::BinOp::FDIV:
    case ast::BinOp::IDIV: return "/";
    case ast::BinOp::MOD: return "%";
    case ast::BinOp::AND: return "&&";
    case ast::BinOp::OR: return "||";
    case ast::BinOp::EQ: return "==";
    case ast::BinOp::NE: return "!=";
    case ast::BinOp::LT: return "<";
    case ast::BinOp::GT: return ">";
    case ast::BinOp::LE: return "<=";
    case ast::BinOp::GE: return ">=";
  }
  return "";
}

auto to_string(const ast::UnaryOp &op) -> std::string
{
  switch (op) {
    case ast::UnaryOp::NOT: return "!";
    case ast::UnaryOp::MINUS: return "-";
    case ast::UnaryOp::PLUS: return "+";
  }
  return "";
}

void CodegenVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Block should not be visited directly");
}

void CodegenVisitor::visit([[maybe_unused]] ast::Number &node)
{
  /*
    print value_即可
  */
  if (node.type() == "integer") {
    print(std::get<int>(node.value()));
  } else if (node.type() == "real") {
    print(std::get<double>(node.value()));
  } else {
    throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::Constant &node)
{
  /*
    * 1. 如果sign是-1，才print '-'
    * 2. 通过type_判断是integer，real，boolean，char，例如：
    * if (type == "string" || "reference")
    *      val = std::get<std::string>(value_);
    * else if (type == "integer")
    *      val = std::get<int>(value_);
    * else if (type == "real")
    *      val = std::get<double>(value_);
    * else if (type == "char")
    *      val = std::get<char>(value_);
    * else
    *      throw std::runtime_error("Unexpected type");
    * 如果是char，加''。
    * 如果是string，加""。
  */
  if (node.type() == "string" || node.type() == "reference") {
    // todo: reference?
    print('"' + std::get<std::string>(node.value()) + '"');
  } else if (node.type() == "integer") {
    if (node.sign() == -1) {
      print("-");
    }
    print(std::get<int>(node.value()));
  } else if (node.type() == "real") {
    if (node.sign() == -1) {
      print("-");
    }
    print(std::get<double>(node.value()));
  } else if (node.type() == "char") {
    print("'" + std::string(1, std::get<char>(node.value())) + "'");
  } else {
    throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::StringLiteral &node)
{
  /*
    print value_即可
  */
  print(node.value());
}

void CodegenVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
  /* 
    1. 通过type_判断是integer，real，boolean，char
    2. print value_
          如果是bool，需要转换成C语言的true和false
          如果是char，需要转换打印''
          integer和real直接打印即可
  */
  if (node.type() == "integer") {
    print(std::get<int>(node.value()));
  } else if (node.type() == "real") {
    print(std::get<double>(node.value()));
  } else if (node.type() == "char") {
    print("'" + std::string(1, std::get<char>(node.value())) + "'");
  } else if (node.type() == "boolean") {
    if (std::get<bool>(node.value())) {
      print("true");
    } else {
      print("false");
    }
  } else {
    throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::BoolExpr &node)
{
  /*
    布尔类型表达式
  */
  node.expr().accept(*this);
}

void CodegenVisitor::visit([[maybe_unused]] ast::BinaryExpr &node)
{
  /*
    print lhs_左值
    print binop运算符，为BinOp枚举类型，需要转换成对应的运算符
    print rhs_右值
  */
  node.lhs().accept(*this);
  print(" " + to_string(node.op()) + " ");
  node.rhs().accept(*this);
}

void CodegenVisitor::visit([[maybe_unused]] ast::UnaryExpr &node)
{
  /*
    print unaryop运算符，为UnaryOp枚举类型，需要转换成对应的运算符
    print expr_表达式
  */
  print(to_string(node.op()) + " ");
  node.expr().accept(*this);
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncCall &node)
{
  /*
    1. 从符号表查找func_id_，判断是否是函数名，该步由语义检查完成。
    2. 如果是函数名，print 'func_id_('
    3. 遍历actuals_，对每一个actual调用accept，做代码生成（由语义检查检查函数传参与函数定义的匹配）
    4. print ')'
  */
  // todo end here
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::AssignableId &node)
{
  /*
    1. 从符号表查找id_，判断是否是函数名。
    2. 如果是函数名，表明是函数返回语句，需要print 'return'
    3. 如果不是函数名，直接print 'id_ = '
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::IndexedVar &node)
{
  /*
    1. 从符号表查找assignable_，判断是否是函数名。是函数名是错的，应该在语义中检查出来。
    2. 如果不是函数名，print 'assignable_['
    3. print expr，如果是expr是变量名，语义检查中会检查是否在符号表中。
    4. print ']'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FieldDesignator &node)
{
  /*
    1. 从符号表查找assignable_，判断是指针
          如果是指针，则print 'assignable_->'
          然后print field_id_
    2. 否则print 'assignable_.'
          然后print field_id_
    3. print '=' (和Funcion的返回值的Assignable情况区分开)
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ConstDecl &node)
{
  /*
    1. 获取ConstDecl的const_中的type_，并加入符号表。如果过type_是reference，那么需要从符号表中获取reference存的type。
    2. print ConstDecl的constid
    3. 最后print ConstDecl的const_的sign_（-1才print）和value_
  */
  printIndent();
  print("const ");
  print(node.constId());
  print(" = ");
  // todo: 查类型符号表 存符号表
}

void CodegenVisitor::visit([[maybe_unused]] ast::ConstDeclPart &node)
{
  /*
    遍历ConstDecl vector列表
    对每一个ConstDecl调用accept，做代码生成
  */
  for (const auto &constDecl : node.constDecls()) {
    constDecl->accept(*this);
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeId &node)
{
  /*
    1. 如果是基本类型
      1.1 如果是integer，print 'int'
      1.2 如果是real，print 'double'
      1.3 如果是char，print 'char'
      1.4 如果是boolean，print 'bool'
    2. 如果是自定义类型，直接print类型符号即可
  */
  print(node.id());
}

void CodegenVisitor::visit([[maybe_unused]] ast::Period &node)
{
  /*
    1. print '['
    2. print high_ - lower_ + 1
    3. print ']'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ArrayType &node)
{
  /*
    Pascal程序应该支持如下语法
    array[lowerBound..upperBound] of array[lowerBound..upperBound] of Integer
    上述写法可以被避免，且对于代码生成徒增麻烦。目前我们不支持该语法，只支持typedenoter是typeid情况
    1. print typedenoter（目前只能是typeid）
    2. 调用periods_的accept，做代码生成
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::RecordType &node)
{
  /*
    1. print 'struct {'
    2. 遍历field_list_，对每一个field_decl_调用accept，做代码生成
    3. print '}'
    4. print 上下文存储的typeid ';'
    5. 类型符号表添加type id和type denoter类型
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeDecl &node)
{
  /*
    1. 先printf typedef
    2. 将typeid存成全局变量，供TypeDenoter使用
    3. 先对type denoter做代码生成
    4. 然后print TypeDecl里面的typeid（需要全局变量传递）
    5. 最后print ';'
    6. 类型符号表添加type id和type denoter类型
  */
  printIndent();
  print("typedef ");
  print(node.typeId());
  print(" ");
  node.typeDenoter().accept(*this);
  print(";\n");

  // todo: 存typeid 添加符号表
}

void CodegenVisitor::visit([[maybe_unused]] ast::TypeDeclPart &node)
{
  /*
    遍历TypeDecl vector列表
    对每一个TypeDecl调用accept，做代码生成
  */
  for (const auto &typeDecl : node.typeDecls()) {
    typeDecl->accept(*this);
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarDeclPart &node)
{
  /*
    1. 遍历var_decl_list_，对每一个var_decl_调用accept，做代码生成
  */
  for (const auto &varDecl : node.varDecls()) {
    varDecl->accept(*this);
  }
}

void CodegenVisitor::visit([[maybe_unused]] ast::ValueParamSpec &node)
{
  /*
    1. 遍历id_list，对每一个id，用','隔开
    2. 对每一个id，先print ValueParamSpec的type，然后是id_list的该id元素。
    3. 注意：Pascal支持proc(a,b,c : Integer)，C语言必须是proc(int a, int b, int c)。
    4. 每一个id和type对插入类型符号表中
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarParamSpec &node)
{
  /*
    1. 遍历id_list，对每一个id，用','隔开
    2. 对每一个id，先print ValueParamSpec的type，然后print * ，然后是id_list的该id元素。
    3. 注意：Pascal支持proc(var a,b,c : Integer)，C语言必须是proc(int* a, int* b, int* c)。
    4. 每一个id和type*对插入类型符号表中 
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarDecl &node)
{
  /*
    1. 先对type denoter做代码生成。print type denoter（判断是否是基本类，如果不是，需要从类型符号表里面找）
    2. 遍历id_list，print每一个id，用','隔开
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcHead &node)
{
  /*
    1. 先print 'void'
    2. 再print proc_id_ 
    3. 如果有formal_param_list_，先print '('，然后遍历formal_param_list_
          对每一个FormalParam做代码生成，用','分隔。然后print ')'
          需要将每一个FormalParam存入符号表中，供后续代码生成使用
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcBlock &node)
{
  /* 
    print('{')
    缩进，并打印/
      如果有const_decl_part_，对此做代码生成
      如果有type_decl_part_，对此做代码生成
      如果有var_decl_part_，对此做代码生成
      不允许嵌套，所以subprog_decl_part_一定为nullptr
      一定有stmt_part_，否则语义检查报错
    print '}'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ProcDecl &node)
{
  /*
    1. 在全局符号表中插入proc_head_的proc_id_和proc标识
    2. 开辟process的符号表，需要从proc_head_中获取proc_id_，作为标识
    3. 先对proc_head_做代码生成
    4. 再对proc_block_做代码生成
    5. 退出process的符号表
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncHead &node)
{
  /*
    1. 先print return_type_
    2. 再print func_id_ 
    3. 如果有formal_param_list_，先print '('，然后遍历formal_param_list_
          对每一个FormalParam做代码生成，用','分隔。然后print ')'
          需要将每一个FormalParam存入符号表中，供后续代码生成使用
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncBlock &node)
{
  /*
    print('{')
    缩进，并打印
      如果有const_decl_part_，对此做代码生成
      如果有type_decl_part_，对此做代码生成
      如果有var_decl_part_，对此做代码生成
      不允许嵌套，所以subprog_decl_part_一定为nullptr
      一定有stmt_part_，否则语义检查报错
    print '}'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::FuncDecl &node)
{
  /*
    1. 在全局符号表中插入func_head_的func_id_和func标识
    2. 开辟func的符号表，从func_head_中获取func_id_，做为标识
    3. 先对func_head_做代码生成
    4. 再对func_block_做代码生成
    5. 退出func的符号表
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::SubprogDeclPart &node)
{
  /*
    遍历subprog_decl_list_，对每一个subprog_decl_调用accept，做代码生成
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::IfStmt &node)
{
  /*
    1. print 'if (' 
    2. print cond_
    3. print ')'
    4. print ' {' 
    5. 缩进，对then_stmt_做代码生成
    6. print '}'
    7. 如果有else_stmt_，print 'else {'，缩进，对else_stmt_做代码生成，print '}'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::CaseStmt &node)
{
  /*
    1. print 'switch('  
    2. print cond_
    3. print ')' '{'
    4. 遍历case_list_，对每一个CaseListElement调用accept，做代码生成
    5. print '}'
    6. 我们的pascal-S到C语言暂不支持case中的default语法
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::CaseListElement &node)
{
  /*
    1. 遍历constants，对constants中每一个constant，print ‘case’， print constant，print ':\n' 
    2. 缩进，对stmt_做代码生成
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::RepeatStmt &node)
{
  /*
    1. print 'do {'，换行
    2. 缩进，遍历body，对每一个stmt做代码生成。
    3. print '} while(' 
    4. print cond，然后print ');'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::WhileStmt &node)
{
  /*
    1. print 'while('
    2. print cond_
    3. print ')'
    4. print '{'，换行
    5. 缩进，对stmt_做代码生成
    6. print '}'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
  /*
    1. print 'for('
    2. print ctrl_var_ = init_val_
    3. print ';'
    4. 判断ForStmt的updown_
          如果是-1，ctrl_var_ >= end_val_ ，否则
          ctrl_var_ <= end_val_ 
    5. print ';'
    6. 判断ForStmt的updown_
          如果是-1，print ctrl_var_ -= 1，否则
          print ctrl_var_ += 1
    7. print ')'
    8. print '{'，换行
    9. 缩进，对stmt_做代码生成
    10. print '}'
  */
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit([[maybe_unused]] ast::AssignStmt &node)
{
  /*
    print AssignStmt的lhs_
    print '='
    print AssignStmt的rhs_
    print ';'
  */
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ProcCallStmt &node)
{
  /*
    1. 从符号表查找proc_id_，判断是否是过程名，该步由语义检查完成。
    2. 如果是过程名字=，print 'proc_id_('
    3. 遍历actuals_，对每一个actual调用accept，做代码生成（由语义检查检查函数传参与函数定义的匹配）
    4. print ')'
  */
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ReadStmt &node)
{
  /* 和ReadlnStmt一样 */
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  /* 去除WritelnStmt的尾部''\n即可 */
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): implement this
void CodegenVisitor::visit([[maybe_unused]] ast::ReadlnStmt &node)
{
  // 1. 首先需要遍历actuals_，对每一个actual，用一个vector存储每一个actual的类型。
  // 2. 遍历上述的类型vector, 输出scanf("%1%2%3\n", 其中%1%2%3是根据类型来的，例如%1是%d，%2是%f，%3是%c)
  // 3. 然后对每一个actual，先print '&'， 然后对该actual做codegen，并用','隔开
  // 4. 最后print ');'
  throw std::runtime_error("Not implemented");
}

// TODO(fpy&dly): writeln arguments and remove [[maybe_unused]]
void CodegenVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  // 1. 首先需要遍历actuals_，对每一个actual，用一个vector存储每一个actual的类型。
  // 2. 遍历上述的类型vector，print("%1%2%3\n", 其中%1%2%3是根据类型来的，例如%1是%d，%2是%f，%3是%c)
  // 3. 然后对每一个actual做codegen，并用','隔开
  // 4. 最后print ');'
  printIndent();
  print("printf(\"");
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
  print("\\n\");\n");
}

void CodegenVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  /*
    ExitStmt继承自ProcallStmt，直接调用accept即可。
    1. exit()，无参数，和exit(0)一个效果
    2. exit(0)或其他参数，需要在语义检查中检查参数是否是整数
  */
  node.accept(*this);
}

void CodegenVisitor::visit([[maybe_unused]] ast::CompoundStmt &node)
{
  /*
    1. 复合语句，先print '{'，并缩进
    2. 遍历语句列表，调用accept，对每一个stmt做代码生成。每一个stmt需要缩进。
    2. print '}'
  */
  println("{");
  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    for (const auto &stmt : node.stmts()) {
      stmt->accept(*this);
    }
  }
  println("}");
  throw std::runtime_error("Not implemented");
}

void CodegenVisitor::visit(ast::StmtPart &node)
{
  IndentGuard ig(&indent_, INDENT_SIZE);
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
}

void CodegenVisitor::visit(ast::ProgramBlock &node)
{
  /*
    如果有const_decl_part_，对此做代码生成
    如果有type_decl_part_，对此做代码生成
    如果有var_decl_part_，对此做代码生成
    如果有subprog_decl_part_，对此做代码生成
    ProgramBlock一定有stmt_part_，否则语义检查报错。 
  */
  if (node.hasConstDeclPart()) {
    node.constDeclPart().accept(*this);
  }
  if (node.hasTypeDeclPart()) {
    node.typeDeclPart().accept(*this);
  }
  if (node.hasVarDeclPart()) {
    node.varDeclPart().accept(*this);
  }
  if (node.hasSubprogDeclPart()) {
    node.subprogDeclPart().accept(*this);
  }
  println("int main() {");
  node.stmtPart().accept(*this);
  println("}");
}

// ? param list
void CodegenVisitor::visit(ast::ProgramHead &node)
{
  println("/**");
  println(" * Program name: " + node.programName());
  println(" */");
  println("#include <stdio.h>");
}

void CodegenVisitor::visit(ast::Program &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}
}  // namespace pascc::codegen