#include <sstream>

#include "semant/semant.hpp"
#include "util/type/type_comparator.hpp"

namespace pascc::semant {


auto SemantVisitor::isOk() -> bool
{
  return context_.error_msgs_.empty();
}

void SemantVisitor::visit([[maybe_unused]] ast::Block &node)
{
  // ! Block 不应该被直接访问，而是以 ProgramBlock、ProcBlock、FuncBlock 的形式出现
  throw std::runtime_error("Block should not be visited directly");
}

void SemantVisitor::visit([[maybe_unused]] ast::Number &node)
{
  // do nothing
}

void SemantVisitor::visit(ast::Constant &node)
{
  /**
    当 type == "reference" 且 sign = -1 时查符号表(找不到则返回未定义的错误)，若为字符串类型，则返回错误，程序终止。
   */
  if (node.sign() == -1 && (node.type() != "integer" && node.type() != "real")) {
    context_.gen_error_msg(node.location(), "invaild constant type.");
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::StringLiteral &node)
{
  /**
   * do nothing
   */
}

void SemantVisitor::visit(ast::BoolExpr &node)
{
  /**
    表示式应当为 bool 类型
   */
  node.expr().accept(*this);
  util::TypeComparator cmp;
  if (!cmp(node.expr().type(), util::SymType(util::BuiltInType(util::BasicType::BOOLEAN)))) {
    context_.gen_error_msg(node.location(), "boolean type expected.");
  }
  auto ptr = std::make_unique<util::SymType>(util::SymType(util::BuiltInType(util::BasicType::BOOLEAN)));
  node.setType(std::move(ptr));
}

void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node)
{
  /**
   * do nothing
   */
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

void SemantVisitor::visit(ast::ConstDecl &node)
{
  /**
   判断有无重定义。
   向符号表中插入<id , constant->type()>
   */
  const auto *tmp = context_.consttab_.probe(node.constId());
  if (tmp != nullptr) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "duplicate identifier "
         << node.constId();
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }

  std::unique_ptr<util::SymType> type;

  if (node.constant().type() == "reference") {
    tmp = context_.consttab_.lookup(std::get<std::string>(node.constant().value()));
    if (tmp == nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "undefined constant identifier "
           << std::get<std::string>(node.constant().value());
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }

    type = std::make_unique<util::SymType>(tmp->built_in_type());
  }

  if (type == nullptr) {
    if (node.constant().type() == "integer") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::INTEGER});
    } else if (node.constant().type() == "real") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::REAL});
    } else if (node.constant().type() == "boolean") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::BOOLEAN});
    } else if (node.constant().type() == "char") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::CHAR});
    } else {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::STRING});
    }
  }

  context_.consttab_.insert(node.constId(), std::move(*type));
}

void SemantVisitor::visit(ast::ConstDeclPart &node)
{
  /*
   访问各个 ConstDecl
  */
  for (auto &constDecl : node.constDecls()) {
    constDecl->accept(*this);
  }
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

void SemantVisitor::visit(ast::TypeDecl &node)
{
  /**
   * 符号表重定义检测。
   * 符号表中新增一项<type_id_, type_denoter_.type()>
   */
  const auto *tmp = context_.typetab_.probe(node.typeId());
  if (tmp == nullptr) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "duplicated identifier "
         << node.typeId();
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }
  tmp = context_.consttab_.probe(node.typeId());
  if (tmp == nullptr) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "duplicated identifier "
         << node.typeId();
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }
  // TODO() : 修改
  //  node.typeDenoter().accept(typeVistor);
  //  context_.typetab_.insert(node.typeId(), typeVistor.type());
}

void SemantVisitor::visit([[maybe_unused]] ast::TypeDeclPart &node)
{
  /**
   * 访问各个TypeDecl
   */
  for (auto &typeDecl : node.typeDecls()) {
    typeDecl->accept(*this);
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::VarDeclPart &node)
{
  /**
   访问各个 VarDecl 
   */
  for (auto &varDecl : node.varDecls()) {
    varDecl->accept(*this);
  }
}

void SemantVisitor::visit(ast::ValueParamSpec &node)
{
  /**
    将id_list_ , type_denoter_ 赋值给父类的对应变量。
    isreference = 0;
  */
  for (auto &id : node.idList()) {
    if (context_.vartab_.probe(id) != nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "duplicated identifier "
           << id;
      context_.error_msgs_.emplace_back(sstr.str());
      continue;
    }
    // TODO() : 修改
    // node.type().accept(typeVistor);
    // context_.vartab_.insert(id, util::VarType{false, typeVistor.type()});
  }
}

void SemantVisitor::visit(ast::VarParamSpec &node)
{
  /**
    将id_list_ , type_denoter_ 赋值给父类的对应变量。
    isreference = 1;
  */
  for (auto &id : node.idList()) {
    if (context_.vartab_.probe(id) != nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "duplicated identifier "
           << id;
      context_.error_msgs_.emplace_back(sstr.str());
      continue;
    }
    // TODO() : 修改
    // node.type().accept(typeVistor);
    // context_.vartab_.insert(id, util::VarType{true, typeVistor.type()});
  }
}

void SemantVisitor::visit(ast::VarDecl &node)
{
  /*
    查找符号表，检测重定义
    插入符号表
  */
  for (const auto &id : node.idList()) {
    const auto *tmp = context_.typetab_.probe(id);
    if (tmp == nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "duplicated identifier "
           << id;
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
    tmp = context_.consttab_.probe(id);
    if (tmp == nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "duplicated identifier "
           << id;
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
    const auto *tmp1 = context_.vartab_.probe(id);
    if (tmp1 == nullptr) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "duplicated identifier "
           << id;
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
    // TODO() : 修改
    // node.type().accept(typeVistor);
    // context_.vartab_.insert(id, util::VarType{false, typeVistor.type()});
  }
}

void SemantVisitor::visit(ast::ProcHead &node)
{
  /**
   把每一个 FormalParam 组装起来，再和id组装，插入符号表
   符号表进入下一级。
   对于每一个FormalParam 符号表判断重定义，再加入<id_list, typedenoter>
  */
  // TODO(): 过程重名的情况

  context_.formal_params_.clear();

  for (const auto &formalParam : node.formalParams()) {
    formalParam->accept(*this);  // 填充 context_.formal_params_
  }
  // TODO(): 通过returntype 是否为nullptr 判断是func 还是 proc
  // 构造 SubprogType 插入到 subprogtab
  context_.subprogtab_.insert(node.procId(), util::SubprogType{false, nullptr, std::move(context_.formal_params_)});
  // TODO(): 这下context_.formal_params没了。得想个办法。
  context_.pushfunc(node.procId());
  context_.enterScope();

  // 遍历 context_.formal_params_ 插入到 vartab_
  for (auto &[varid, vartype] : context_.formal_params_) {
    // TODO(): 每次都要检查重复声明的情况
    context_.vartab_.insert(varid, std::move(*vartype));
  }
}

void SemantVisitor::visit(ast::ProcBlock &node)
{
  /**
   顺次访问 各个part
   如果 stmt_part 为空，返回错误，程序终止。
   符号表返回上一级
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
  if (node.hasStmtPart()) {
    node.stmtPart().accept(*this);
  }
  context_.popfunc();
  context_.exitScope();
}

void SemantVisitor::visit(ast::ProcDecl &node)
{
  /*
  访问 head block
  */
  node.head().accept(*this);
  node.block().accept(*this);
}

void SemantVisitor::visit(ast::FuncHead &node)
{
  /**
   符号表进入下一级。
   对于每一个FormalParam 符号表判断重定义，再加入<id_list, typedenoter>
   把每一个 FormalParam 组装起来，再和id、return组装，插入符号表
   */

  // TODO(): 函数重名的情况

  context_.formal_params_.clear();
  // TODO() : 修改
  // node.returnType().accept(typeVisitor);
  // context_.vartab_.insert(node.funcId(), util::VarType{false, typeVisitor.type()});

  for (const auto &formalParam : node.formalParams()) {
    formalParam->accept(*this);  // 填充 context_.formal_params_
  }

  // 构造 SubprogType 插入到 subprogtab
  // TODO() : 修改
  // node.returnType().accept(typeVisitor);
  // context_.subprogtab_.insert(node.funcId(), util::SubprogType{true, typeVisitor.type(), std::move(context_.formal_params_)});

  context_.pushfunc(node.funcId());
  context_.enterScope();

  // 遍历 context_.formal_params_ 插入到 vartab_
  for (auto &[varid, vartype] : context_.formal_params_) {
    // TODO(): 每次都要检查重复声明的情况
    context_.vartab_.insert(varid, std::move(*vartype));
  }
}

void SemantVisitor::visit(ast::FuncBlock &node)
{
  /**
   顺次访问 各个part
   符号表返回上一级
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
  if (node.hasStmtPart()) {
    node.stmtPart().accept(*this);
  }
  context_.exitScope();
  context_.popfunc();
}

void SemantVisitor::visit(ast::FuncDecl &node)
{
  /**
   * 访问 head block
   */
  node.head().accept(*this);
  node.block().accept(*this);
}

void SemantVisitor::visit(ast::SubprogDeclPart &node)
{
  /*
    访问各个 subprog_decl
  */
  for (const auto &subprog_decl : node.subprogDecls()) {
    subprog_decl->accept(*this);
  }
}

void SemantVisitor::visit(ast::IfStmt &node)
{
  /**
    访问 then, 访问 else
   */
  util::TypeComparator cmp;
  if (!cmp(node.cond().type(), util::SymType(util::BuiltInType(util::BasicType::BOOLEAN)))) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "boolean type expected.";
    context_.error_msgs_.emplace_back(sstr.str());
    // 条件不是bool
  }
  node.then().accept(*this);
  if (node.hasElse()) {
    node.Else().accept(*this);
  }
}

void SemantVisitor::visit(ast::CaseStmt &node)
{
  /** 
    访问各个 CaseListElement
    如果 Expr 类型和标识条件常量的类型不一致则返回错误，程序退出。
  */
  node.expr().accept(*this);
  context_.case_stmt_type_ = &node.expr().type();
  for (const auto &eachcase : node.caseList()) {
    eachcase->stmt().accept(*this);
  }
}

void SemantVisitor::visit(ast::CaseListElement &node)
{
  /**
  访问Stmt
  */
  util::TypeComparator cmp;
  for (const auto &cons : node.constants()) {
    std::unique_ptr<util::SymType> type;
    if (cons->type() == "integer") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::INTEGER});
    } else if (cons->type() == "real") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::REAL});
    } else if (cons->type() == "boolean") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::BOOLEAN});
    } else if (cons->type() == "char") {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::CHAR});
    } else {
      type = std::make_unique<util::SymType>(util::BuiltInType{util::BasicType::STRING});
    }
    if (!cmp(*context_.case_stmt_type_, *type) && !util::TypeComparator::cast(*type, *context_.case_stmt_type_)) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "case type doesn't match expr's type.";
      context_.error_msgs_.emplace_back(sstr.str());
    }
  }
  node.stmt().accept(*this);
}

void SemantVisitor::visit(ast::RepeatStmt &node)
{
  /** 
  访问body
  */
  node.cond().accept(*this);
  util::TypeComparator cmp;
  if (!cmp(node.cond().type(), util::SymType(util::BuiltInType(util::BasicType::BOOLEAN)))) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "boolean type expected.";
    context_.error_msgs_.emplace_back(sstr.str());
    // 条件不是bool
  }
  for (const auto &stmt : node.body()) {
    stmt->accept(*this);
  }
}

void SemantVisitor::visit(ast::WhileStmt &node)
{
  /** 
  访问body
  */
  node.cond().accept(*this);
  util::TypeComparator cmp;
  if (!cmp(node.cond().type(), util::SymType(util::BuiltInType(util::BasicType::BOOLEAN)))) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "boolean type expected.";
    context_.error_msgs_.emplace_back(sstr.str());
    // 条件不是bool
  }
  node.body().accept(*this);
}

void SemantVisitor::visit([[maybe_unused]] ast::ForStmt &node)
{
  /** 
  如果ctrl_var和init_val类型不一致则返回错误，程序终止。
  访问body
  */
  node.ctrlVar().accept(*this);
  node.initVal().accept(*this);
  node.endVal().accept(*this);
  util::TypeComparator cmp;
  if ((!cmp(node.ctrlVar().type(), node.initVal().type()) && !util::TypeComparator::cast(node.ctrlVar().type(), node.initVal().type())) || (!cmp(node.ctrlVar().type(), node.endVal().type()) && !util::TypeComparator::cast(node.ctrlVar().type(), node.endVal().type()))) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "ctrlVal type doesn't match initVal or endVal";
    context_.error_msgs_.emplace_back(sstr.str());
  }
  node.body().accept(*this);
}

void SemantVisitor::visit(ast::AssignStmt &node)
{
  /*
   如果左右类型不一致则返回错误，程序终止。
  */
  //左侧可以赋值
  node.lhs().accept(*this);
  node.rhs().accept(*this);
  if (!node.lhs().isLvalue()) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "Variable identifier expected ";
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }
  util::TypeComparator comparator;
  if (comparator(node.lhs().type(), node.rhs().type())) {
    return;
  }
  if (util::TypeComparator::cast(node.rhs().type(), node.lhs().type())) {
    return;
  }
  std::stringstream sstr;
  // TODO(): 重载type的流运算符
  sstr << node.location() << ": "
       << "expected type a but received type b.";
  context_.error_msgs_.emplace_back(sstr.str());
}

void SemantVisitor::visit(ast::ProcCallStmt &node)
{
  /**
   在符号表中查找声明，若找不到，则返回错误，程序终止。
   获取ProcCall类型表达式。
   对actuals_中的每一个参数进行类型检查，若存在不相等的情况，则返回错误，程序终止。
   */
  const auto *proc = context_.subprogtab_.lookup(node.procId());
  if (proc == nullptr) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "undefined procedure call.";
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }
  const auto &actuals_expected = proc->formal_params();
  if (actuals_expected.size() != node.actuals().size()) {
    std::stringstream sstr;
    sstr << node.location() << ": "
         << "actual list do not match.";
    context_.error_msgs_.emplace_back(sstr.str());
    return;
  }

  for (int i = 0; i < static_cast<int>(node.actuals().size()); i++) {
    util::TypeComparator comparator;
    node.actuals()[i]->accept(*this);
    if (!node.actuals()[i]->isLvalue() && actuals_expected[i].second->is_ref()) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "actual list do not match.";
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
    if (!comparator(actuals_expected[i].second->type(), node.actuals()[i]->type()) && !util::TypeComparator::cast(node.actuals()[i]->type(), actuals_expected[i].second->type())) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "actual list do not match.";
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
  }
}

void SemantVisitor::visit(ast::ReadStmt &node)
{
  /*
    对actuals_中的每一个参数进行类型检查。
    ! 读取的变量必须是 assignable
  */
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
    if (!actual->isLvalue()) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "Assignable actual expected.";
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::WriteStmt &node)
{
  /**
  * do nothing
  */
}

void SemantVisitor::visit(ast::ReadlnStmt &node)
{
  /**
  * 对actuals_中的每一个参数进行类型检查。
  */
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
    if (!actual->isLvalue()) {
      std::stringstream sstr;
      sstr << node.location() << ": "
           << "Assignable actual expected.";
      context_.error_msgs_.emplace_back(sstr.str());
      return;
    }
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::WritelnStmt &node)
{
  /**
  * do nothing
  */
}

void SemantVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  // TODO() : process的size是0。function的size是1
  auto nowfunc                    = context_.nowfunc();
  const auto canreturn            = context_.subprogtab_.probe(nowfunc)->is_func();
  const util::SymType &returntype = context_.subprogtab_.probe(nowfunc)->return_type();
  util::TypeComparator comparator;
  if (node.actuals().empty() && !canreturn) {
    return;
  }
  if (node.actuals().size() == 1 && canreturn) {
    node.actuals()[0]->accept(*this);
    const auto &thistype = node.actuals()[0]->type();
    if (!comparator(thistype, returntype) && !util::TypeComparator::cast(thistype, returntype)) {
      context_.gen_error_msg(node.location(), "return value type doesn't match.");
    }
    return;
  }
  context_.gen_error_msg(node.location(), "invaild use of exit.");
}

void SemantVisitor::visit(ast::CompoundStmt &node)
{
  /*
    访问各个Stmt
  */
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
}

void SemantVisitor::visit(ast::StmtPart &node)
{
  /*
    访问各个Stmt
  */
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
}

void SemantVisitor::visit(ast::ProgramBlock &node)
{
  /**
   顺次访问 各个part
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
  if (node.hasStmtPart()) {
    node.stmtPart().accept(*this);
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
  context_.pushfunc(node.programName());
  // TODO(): 上次写到这里了
  // do nothing
}

void SemantVisitor::visit(ast::Program &node)
{
  // Program -> ProgramHead ';' ProgramBlock '.'
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
