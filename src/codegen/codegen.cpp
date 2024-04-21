#include <iostream>
#include <sstream>

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

class FlagGuard
{
public:
  explicit FlagGuard(bool &flag)
    : flag_(&flag)
  {
    *flag_ = true;
  }

  ~FlagGuard()
  {
    *flag_ = false;
  }

private:
  bool *flag_;
};

auto to_string(const pascc::ast::BinOp &op) -> std::string
{
  switch (op) {
    case pascc::ast::BinOp::PLUS: return "+";
    case pascc::ast::BinOp::MINUS: return "-";
    case pascc::ast::BinOp::MUL: return "*";
    case pascc::ast::BinOp::FDIV:
    case pascc::ast::BinOp::IDIV: return "/";
    case pascc::ast::BinOp::MOD: return "%";
    case pascc::ast::BinOp::AND: return "&&";
    case pascc::ast::BinOp::OR: return "||";
    case pascc::ast::BinOp::EQ: return "==";
    case pascc::ast::BinOp::NE: return "!=";
    case pascc::ast::BinOp::LT: return "<";
    case pascc::ast::BinOp::GT: return ">";
    case pascc::ast::BinOp::LE: return "<=";
    case pascc::ast::BinOp::GE: return ">=";
  }
  return "";
}

auto to_string(const pascc::ast::UnaryOp &op, const pascc::util::BasicType &type) -> std::string
{
  switch (op) {
    case pascc::ast::UnaryOp::NOT:
      if (type == pascc::util::BasicType::BOOLEAN) {
        return "!";
      }
      return "~";

    case pascc::ast::UnaryOp::MINUS: return "-";
    case pascc::ast::UnaryOp::PLUS: return "+";
  }
  return "";
}

auto placeholder(pascc::ast::Expr &expr) -> std::string
{
  const auto &type = expr.type();
  if (type.eType() != pascc::util::SymType::Type::BUILT_IN) {
    throw std::runtime_error("unexpected expr type");
  }

  switch (type.builtInType().type()) {
    case pascc::util::BasicType::INTEGER:
      return "%d";
    case pascc::util::BasicType::REAL:
      return "%lf";
    case pascc::util::BasicType::BOOLEAN:
      return "%d";
    case pascc::util::BasicType::CHAR:
      return "%c";
    case pascc::util::BasicType::STRING:
      return "%s";
    default:
      throw std::runtime_error("unexpected expr type");
  }
  return {};
}

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

template<typename T>
void CodegenVisitor::print(const T &t)
{
  (file_output_ ? fout_ : std::cout) << t;
}

template<>
void CodegenVisitor::print(const util::SymType &t)
{
  std::stringstream sstr;
  switch (t.eType()) {
    case util::SymType::Type::BUILT_IN:
      switch (t.builtInType().type()) {
        case util::BasicType::INTEGER:
          sstr << "int";
          break;
        case util::BasicType::REAL:
          sstr << "double";
          break;
        case util::BasicType::BOOLEAN:
          sstr << "bool";
          break;
        case util::BasicType::CHAR:
          sstr << "char";
          break;
        default:
          // 暂不支持 字符串 类型
          throw std::runtime_error("Unexpected type");
      }
      break;

    case util::SymType::Type::USER_DEFINED:
      sstr << t.userDefinedType();
      break;
    default:
      // 暂不支持 数组 记录 类型
      throw std::runtime_error("Not implemented");
  }
  (file_output_ ? fout_ : std::cout) << sstr.str();
}

void CodegenVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Block should not be visited directly");
}

void CodegenVisitor::visit(ast::Number &node)
{
  /**
   * print value_ 即可
   */

  switch (node.type().builtInType().type()) {
    case util::BasicType::INTEGER:
      print(std::get<int>(node.value()));
      break;
    case util::BasicType::REAL:
      print(std::get<double>(node.value()));
      break;
    default:
      throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit(ast::Constant &node)
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
  if (node.type() == "string") {
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
  } else if (node.type() == "reference") {
    if (node.sign() == -1) {
      print("-");
    }
    print(std::get<std::string>(node.value()));
  } else {
    throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit(ast::StringLiteral &node)
{
  /*
    print value_即可
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  print('"');
  print(node.value());
  print('"');
}

void CodegenVisitor::visit(ast::UnsignedConstant &node)
{
  /* 
    1. 通过type_判断是integer，real，boolean，char
    2. print value_
          如果是bool，需要转换成C语言的true和false
          如果是char，需要转换打印''
          integer和real直接打印即可
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  switch (node.type().builtInType().type()) {
    case util::BasicType::INTEGER:
      print(std::get<int>(node.value()));
      break;

    case util::BasicType::REAL:
      print(std::get<double>(node.value()));
      break;

    case util::BasicType::BOOLEAN:
      if (std::get<bool>(node.value())) {
        print("true");
      } else {
        print("false");
      }
      break;

    case util::BasicType::CHAR:
      print("'" + std::string(1, std::get<char>(node.value())) + "'");
      break;

    default:
      throw std::runtime_error("Unexpected type");
  }
}

void CodegenVisitor::visit(ast::BoolExpr &node)
{
  /*
    布尔类型表达式
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  node.expr().accept(*this);
}

void CodegenVisitor::visit(ast::BinaryExpr &node)
{
  /*
    print lhs_左值
    print binop运算符，为BinOp枚举类型，需要转换成对应的运算符
    print rhs_右值
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  print('(');
  if (node.op() == ast::BinOp::FDIV) {
    print("(double) ");
  }

  node.lhs().accept(*this);
  print(" " + to_string(node.op()) + " ");
  node.rhs().accept(*this);
  print(')');
}

void CodegenVisitor::visit(ast::UnaryExpr &node)
{
  /*
    print unaryop运算符，为UnaryOp枚举类型，需要转换成对应的运算符
    print expr_表达式
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  print('(');
  print(to_string(node.op(), node.type().builtInType().type()));
  node.expr().accept(*this);
  print(')');
}

void CodegenVisitor::visit(ast::FuncCall &node)
{
  /*
    1. 从符号表查找func_id_，判断是否是函数名，该步由语义检查完成。
    2. 如果是函数名，print 'func_id_('
    3. 遍历actuals_，对每一个actual调用accept，做代码生成（由语义检查检查函数传参与函数定义的匹配）
    4. print ')'
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  print(node.funcid() + "(");
  auto *subprog_type  = context_.subprogtab_.lookup(node.funcid());
  const auto &formals = subprog_type->formalParams();
  const auto &actuals = node.actuals();
  auto size           = actuals.size();
  for (unsigned i = 0; i < size; i++) {
    if (i != 0) {
      print(", ");
    }
    if (formals[i].second->isRef()) {
      print('&');
    }
    actuals[i]->accept(*this);
  }
  print(')');
}

void CodegenVisitor::visit(ast::VarId &node)
{
  /*
    1. 从符号表查找id_，判断是否是函数名。
    2. 如果是函数名，表明是函数返回语句，需要print 'return'
    3. 如果不是函数名，直接print 'id_ = '
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  auto *found = context_.vartab_.lookup(node.id());
  if (found != nullptr && !context_.in_field_designator_) {
    print("*");
  } else if (found != nullptr && context_.in_field_designator_) {
    context_.in_field_designator_ = true;
  } else if (node.id() == context_.current_subprog_) {
    print("__");
  }
  print(node.id());
}

void CodegenVisitor::visit(ast::IndexedVar &node)
{
  /*
    1. 从符号表查找assignable_，判断是否是函数名。是函数名是错的，应该在语义中检查出来。
    2. 如果不是函数名，print 'assignable_['
    3. print expr，如果是expr是变量名，语义检查中会检查是否在符号表中。
    4. print ']'
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  node.varAccess().accept(*this);
  const auto &indices = node.indices();
  const auto &periods = node.varAccess().type().arrayType().periods();
  auto size           = indices.size();
  for (unsigned i = 0; i < size; i++) {
    print("[");
    indices[i]->accept(*this);
    if (periods[i].first != 0) {
      print(" - ");
      print(periods[i].first);
    }
    print("]");
  }
}

void CodegenVisitor::visit(ast::FieldDesignator &node)
{
  /*
    1. 从符号表查找assignable_，判断是指针
          如果是指针，则print 'assignable_->'
          然后print field_id_
    2. 否则print 'assignable_.'
          然后print field_id_
    3. print '=' (和Funcion的返回值的Assignable情况区分开)
  */
  if (context_.build_format_string_) {
    print(placeholder(node));
    return;
  }

  context_.in_field_designator_ = true;
  context_.field_is_ref_        = false;
  node.varAccess().accept(*this);
  if (context_.field_is_ref_) {
    print("->");
  } else {
    print('.');
  }
  print(node.field());
  context_.in_field_designator_ = false;
}

void CodegenVisitor::visit(ast::ConstDecl &node)
{
  /*
    1. 获取ConstDecl的const_中的type_，并加入符号表。如果过type_是reference，那么需要从符号表中获取reference存的type。
    2. print ConstDecl的constid
    3. 最后print ConstDecl的const_的sign_（-1才print）和value_
  */
  printIndent();
  print("const ");
  if (node.constant().type() == "string") {
    print("char *");
    context_.consttab_.insert(node.constId(), "char *");
  } else if (node.constant().type() == "integer") {
    print("int");
    context_.consttab_.insert(node.constId(), "int");
  } else if (node.constant().type() == "real") {
    print("double");
    context_.consttab_.insert(node.constId(), "double");
  } else if (node.constant().type() == "char") {
    print("char");
    context_.consttab_.insert(node.constId(), "char");
  } else if (node.constant().type() == "reference") {
    auto type = context_.consttab_.lookup(node.constId());
    print(type);
    context_.consttab_.insert(node.constId(), type);
  } else {
    throw std::runtime_error("Unexpected type");
  }
  print(' ');
  print(node.constId());
  print(" = ");
  node.constant().accept(*this);
  print(";\n");
}

void CodegenVisitor::visit(ast::ConstDeclPart &node)
{
  /*
    遍历ConstDecl vector列表
    对每一个ConstDecl调用accept，做代码生成
  */
  for (const auto &constDecl : node.constDecls()) {
    constDecl->accept(*this);
  }
}

void CodegenVisitor::visit(ast::TypeId &node)
{
  /*
    1. 如果是基本类型
      1.1 如果是integer，print 'int'
      1.2 如果是real，print 'double'
      1.3 如果是char，print 'char'
      1.4 如果是boolean，print 'bool'
    2. 如果是自定义类型，直接print类型符号即可
  */
  if (node.id() == "integer") {
    print("int");
  } else if (node.id() == "real") {
    print("double");
  } else if (node.id() == "char") {
    print("char");
  } else if (node.id() == "boolean") {
    print("bool");
  } else {
    print(node.id());
  }
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
  print(node.symType().arrayType().baseType());

  const auto &periods = node.symType().arrayType().periods();
  for (const auto &[lb, ub] : periods) {
    // print [upper_bound1 - lower_bound1 + 1][upper_bound2 - lower_bound2 + 1]...
    context_.array_bounds_.emplace_back(ub - lb + 1);
  }
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

void CodegenVisitor::visit(ast::TypeDecl &node)
{
  /*
    1. 先printf typedef
    2. 将typeid存成全局变量，供TypeDenoter使用
    3. 先对type denoter做代码生成
    4. 然后print TypeDecl里面的typeid（需要全局变量传递）
    5. 最后print ';'
    6. 类型符号表添加type id和type denoter类型

    eg. `typedef int i32;`
  */
  printIndent();
  print("typedef ");
  print(node.typeDenoter().symType());
  print(" ");
  print(node.typeId());
  print(";\n");
}

void CodegenVisitor::visit(ast::TypeDeclPart &node)
{
  /*
    遍历TypeDecl vector列表
    对每一个TypeDecl调用accept，做代码生成
  */
  for (const auto &typeDecl : node.typeDecls()) {
    typeDecl->accept(*this);
  }
}

void CodegenVisitor::visit(ast::VarDeclPart &node)
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
  throw std::runtime_error("Should not be visited by codegen visitor");
}

void CodegenVisitor::visit([[maybe_unused]] ast::VarParamSpec &node)
{
  /*
    1. 遍历id_list，对每一个id，用','隔开
    2. 对每一个id，先print ValueParamSpec的type，然后print * ，然后是id_list的该id元素。
    3. 注意：Pascal支持proc(var a,b,c : Integer)，C语言必须是proc(int* a, int* b, int* c)。
    4. 每一个id和type*对插入类型符号表中 
  */
  throw std::runtime_error("Should not be visited by codegen visitor");
}

void CodegenVisitor::visit(ast::VarDecl &node)
{
  /*
    1. 先对type denoter做代码生成。print type denoter（判断是否是基本类，如果不是，需要从类型符号表里面找）
    2. 遍历id_list，print每一个id，用','隔开
  */
  printIndent();
  context_.array_bounds_.clear();
  node.type().accept(*this);
  print(" ");
  print(node.idList().front());
  if (!context_.array_bounds_.empty()) {
    for (const auto &bound : context_.array_bounds_) {
      print('[');
      print(bound);
      print(']');
    }
  }
  for (auto iter = node.idList().begin() + 1; iter != node.idList().end(); ++iter) {
    print(", " + *iter);
    if (!context_.array_bounds_.empty()) {
      for (const auto &bound : context_.array_bounds_) {
        print('[');
        print(bound);
        print(']');
      }
    }
  }
  print(";\n");
}

void CodegenVisitor::visit(ast::ProcHead &node)
{
  /*
    1. 先print 'void'
    2. 再print proc_id_ 
    3. 如果有formal_param_list_，先print '('，然后遍历formal_param_list_
          对每一个FormalParam做代码生成，用','分隔。然后print ')'
          需要将每一个FormalParam存入符号表中，供后续代码生成使用
  */
  context_.current_subprog_      = node.procId();
  context_.current_subprog_type_ = &node.procType();
  context_.subprogtab_.insert(node.procId(), context_.current_subprog_type_);
  context_.enterScope();
  print("void " + node.procId() + "(");
  bool first = true;
  for (const auto &[id, type] : node.procType().formalParams()) {
    if (!first) {
      print(", ");
    }
    print(type->symType());
    if (type->isRef()) {
      print(" *");
      context_.vartab_.insert(id, type);
    }
    print(" " + id);
    if (first) {
      first = false;
    }
  }
  print(")");
}

void CodegenVisitor::visit(ast::ProcBlock &node)
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
  println(" {");
  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    if (node.hasConstDeclPart()) {
      node.constDeclPart().accept(*this);
    }
    if (node.hasTypeDeclPart()) {
      node.typeDeclPart().accept(*this);
    }
    if (node.hasVarDeclPart()) {
      node.varDeclPart().accept(*this);
    }
    node.stmtPart().accept(*this);
  }
  println("}");
  context_.exitScope();
}

void CodegenVisitor::visit(ast::ProcDecl &node)
{
  /*
    1. 在全局符号表中插入proc_head_的proc_id_和proc标识
    2. 开辟process的符号表，需要从proc_head_中获取proc_id_，作为标识
    3. 先对proc_head_做代码生成
    4. 再对proc_block_做代码生成
    5. 退出process的符号表
  */
  node.head().accept(*this);
  node.block().accept(*this);
}

void CodegenVisitor::visit(ast::FuncHead &node)
{
  /*
    1. 先print return_type_
    2. 再print func_id_ 
    3. 如果有formal_param_list_，先print '('，然后遍历formal_param_list_
          对每一个FormalParam做代码生成，用','分隔。然后print ')'
          需要将每一个FormalParam存入符号表中，供后续代码生成使用
  */
  context_.current_subprog_      = node.funcId();
  context_.current_subprog_type_ = &node.funcType();
  context_.subprogtab_.insert(node.funcId(), context_.current_subprog_type_);
  context_.enterScope();
  print(node.funcType().returnType());
  print(" " + node.funcId() + "(");
  bool first = true;
  for (const auto &[id, type] : node.funcType().formalParams()) {
    if (!first) {
      print(", ");
    }
    print(type->symType());
    if (type->isRef()) {
      print(" *");
      context_.vartab_.insert(id, type);
    }
    print(" " + id);
    if (first) {
      first = false;
    }
  }
  print(")");
}

void CodegenVisitor::visit(ast::FuncBlock &node)
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
  println(" {");
  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    if (node.hasConstDeclPart()) {
      node.constDeclPart().accept(*this);
    }
    if (node.hasTypeDeclPart()) {
      node.typeDeclPart().accept(*this);
    }
    if (node.hasVarDeclPart()) {
      node.varDeclPart().accept(*this);
    }

    // 声明函数返回值
    printIndent();
    print(context_.current_subprog_type_->returnType());
    print(" __" + context_.current_subprog_ + ";\n");

    node.stmtPart().accept(*this);

    // 返回函数返回值
    printIndent();
    print("return __" + context_.current_subprog_ + ";\n");
  }
  println("}");
  context_.exitScope();
}

void CodegenVisitor::visit(ast::FuncDecl &node)
{
  /*
    1. 在全局符号表中插入func_head_的func_id_和func标识
    2. 开辟func的符号表，从func_head_中获取func_id_，做为标识
    3. 先对func_head_做代码生成
    4. 再对func_block_做代码生成
    5. 退出func的符号表
  */

  node.head().accept(*this);
  node.block().accept(*this);
}

void CodegenVisitor::visit(ast::SubprogDeclPart &node)
{
  /*
    遍历subprog_decl_list_，对每一个subprog_decl_调用accept，做代码生成
  */
  for (const auto &subprogDecl : node.subprogDecls()) {
    subprogDecl->accept(*this);
  }
}

void CodegenVisitor::visit(ast::IfStmt &node)
{
  /*
    1. print 'if (' 
    2. print cond_
    3. print ')'
    4. 对then_stmt_做代码生成
    5. 如果有else_stmt_，对else_stmt_做代码生成
  */

  // cond
  printIndent();
  print("if (");
  node.cond().accept(*this);
  print(")\n");

  {
    FlagGuard fg(context_.in_structure_stmt_);
    IndentGuard ig(&indent_, INDENT_SIZE);
    if (node.hasThen()) {
      node.then().accept(*this);
    } else {
      println(";");
    }
  }

  if (node.hasElse()) {
    printIndent();
    print("else\n");
    FlagGuard fg(context_.in_structure_stmt_);
    IndentGuard ig(&indent_, INDENT_SIZE);
    node.Else().accept(*this);
  }
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
  // TODO (fpy & dly): implement this
  printIndent();
  print("switch(");
  node.expr().accept(*this);
  print(")  {\n");

  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    for (const auto &caseItem : node.caseList()) {
      caseItem->accept(*this);
    }
  }

  printIndent();
  print("}\n");
}

void CodegenVisitor::visit([[maybe_unused]] ast::CaseListElement &node)
{
  /*
    1. 遍历constants，对constants中每一个constant，print ‘case’， print constant，print ':\n' 
    2. 缩进，对stmt_做代码生成
  */
  // TODO (fpy & dly): implement this
  for (const auto &constant : node.constants()) {
    printIndent();
    print("case ");
    constant->accept(*this);
    print(": ");
    if (constant != node.constants().back()) {
      print("\n");
    }
  }
  node.stmt().accept(*this);
}

void CodegenVisitor::visit(ast::RepeatStmt &node)
{
  /*
    1. print 'do {'，换行
    2. 缩进，遍历body，对每一个stmt做代码生成。
    3. print '} while(' 
    4. print cond，然后print ');'
  */
  printIndent();
  print("do {\n");
  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    for (const auto &stmt : node.body()) {
      stmt->accept(*this);
    }
  }
  printIndent();
  print("} while (");
  node.cond().accept(*this);
  print(")\n");
}

void CodegenVisitor::visit(ast::WhileStmt &node)
{
  /*
    1. print 'while('
    2. print cond_
    3. print ')'
    4. print '{'，换行
    5. 缩进，对stmt_做代码生成
    6. print '}'
  */
  printIndent();
  print("while (");
  node.cond().accept(*this);
  print(") ");
  node.body().accept(*this);
}

void CodegenVisitor::visit(ast::ForStmt &node)
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
  printIndent();
  print("for (");
  node.ctrlVar().accept(*this);
  print(" = ");
  node.initVal().accept(*this);
  print("; ");
  node.ctrlVar().accept(*this);
  if (node.updown()) {
    print(" <= ");
  } else {
    print(" >= ");
  }
  node.endVal().accept(*this);
  print("; ");
  node.ctrlVar().accept(*this);
  if (node.updown()) {
    print("++");
  } else {
    print("--");
  }
  print(")\n");

  {
    FlagGuard fg(context_.in_structure_stmt_);
    IndentGuard ig(&indent_, INDENT_SIZE);
    node.body().accept(*this);
  }
}

void CodegenVisitor::visit(ast::AssignStmt &node)
{
  /*
    print AssignStmt的lhs_
    print '='
    print AssignStmt的rhs_
    print ';'
  */
  printIndent();
  node.lhs().accept(*this);
  print(" = ");
  node.rhs().accept(*this);
  print(";\n");
}

void CodegenVisitor::visit([[maybe_unused]] ast::BreakStmt &node)
{
  /*
    print 'break;'
  */
  printIndent();
  print("break;\n");
}

void CodegenVisitor::visit(ast::ProcCallStmt &node)
{
  /*
    1. 从符号表查找proc_id_，判断是否是过程名，该步由语义检查完成。
    2. 如果是过程名字=，print 'proc_id_('
    3. 遍历actuals_，对每一个actual调用accept，做代码生成（由语义检查检查函数传参与函数定义的匹配）
    4. print ')'
  */
  printIndent();
  print(node.procId() + "(");
  auto *subprog_type  = context_.subprogtab_.lookup(node.procId());
  const auto &formals = subprog_type->formalParams();
  const auto &actuals = node.actuals();
  auto size           = actuals.size();
  for (unsigned i = 0; i < size; i++) {
    if (i != 0) {
      print(", ");
    }
    if (formals[i].second->isRef()) {
      print('&');
    }
    actuals[i]->accept(*this);
  }
  print(");\n");
}

void CodegenVisitor::visit(ast::ReadStmt &node)
{
  /* 和ReadlnStmt一样 */
  printIndent();
  print("scanf(\"");
  context_.build_format_string_ = true;
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
  context_.build_format_string_ = false;
  print("\"");
  for (const auto &actual : node.actuals()) {
    print(", &");
    actual->accept(*this);
  }
  print(");\n");
}

void CodegenVisitor::visit(ast::WriteStmt &node)
{
  /* 去除WritelnStmt的尾部''\n即可 */
  printIndent();
  print("printf(\"");
  context_.build_format_string_ = true;
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
  context_.build_format_string_ = false;
  print("\"");
  for (const auto &actual : node.actuals()) {
    print(", ");
    actual->accept(*this);
  }
  print(");\n");
}

void CodegenVisitor::visit(ast::ReadlnStmt &node)
{
  // 1. 首先需要遍历actuals_，对每一个actual，用一个vector存储每一个actual的类型。
  // 2. 遍历上述的类型vector, 输出scanf("%1%2%3\n", 其中%1%2%3是根据类型来的，例如%1是%d，%2是%f，%3是%c)
  // 3. 然后对每一个actual，先print '&'， 然后对该actual做codegen，并用','隔开
  // 4. 最后print ');'
  printIndent();
  print("scanf(\"");
  context_.build_format_string_ = true;
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
  context_.build_format_string_ = false;
  print("\\n\"");
  for (const auto &actual : node.actuals()) {
    print(", &");
    actual->accept(*this);
  }
  print(");\n");
}

void CodegenVisitor::visit(ast::WritelnStmt &node)
{
  // 1. 首先需要遍历actuals_，对每一个actual，用一个vector存储每一个actual的类型。
  // 2. 遍历上述的类型vector，print("%1%2%3\n", 其中%1%2%3是根据类型来的，例如%1是%d，%2是%f，%3是%c)
  // 3. 然后对每一个actual做codegen，并用','隔开
  // 4. 最后print ');'
  printIndent();
  print("printf(\"");
  context_.build_format_string_ = true;
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
  context_.build_format_string_ = false;
  print("\\n\"");
  for (const auto &actual : node.actuals()) {
    print(", ");
    actual->accept(*this);
  }
  print(");\n");
}

void CodegenVisitor::visit(ast::ExitStmt &node)
{
  /*
    ExitStmt继承自ProcallStmt，直接调用accept即可。
    1. exit()，无参数，和exit(0)一个效果
    2. exit(0)或其他参数，需要在语义检查中检查参数是否是整数
  */
  print("return");
  if (!node.actuals().empty()) {
    print(" ");
    node.actuals().front()->accept(*this);
  }
  print(";\n");
}

void CodegenVisitor::visit(ast::CompoundStmt &node)
{
  /*
    1. 复合语句，先print '{'，并缩进
    2. 遍历语句列表，调用accept，对每一个stmt做代码生成。每一个stmt需要缩进。
    2. print '}'
  */
  auto printCompound = [&]() {
    println("{");
    {
      IndentGuard ig(&indent_, INDENT_SIZE);
      for (const auto &stmt : node.stmts()) {
        stmt->accept(*this);
      }
    }
    println("}");
  };

  if (context_.in_structure_stmt_) {
    IndentGuard ig(&indent_, -INDENT_SIZE);
    printCompound();
  } else {
    printCompound();
  }
}

void CodegenVisitor::visit(ast::StmtPart &node)
{
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
  {
    IndentGuard ig(&indent_, INDENT_SIZE);
    node.stmtPart().accept(*this);
  }
  println("}");
}

// ? param list
void CodegenVisitor::visit(ast::ProgramHead &node)
{
  println("/**");
  println(" * Program name: " + node.programName());
  println(" */");
  println("#include <stdio.h>");
  println("#include <stdlib.h>");
  println("#include <stdbool.h>");
}

void CodegenVisitor::visit(ast::Program &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}
}  // namespace pascc::codegen