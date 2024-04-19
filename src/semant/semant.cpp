#include <sstream>

#include "semant/semant.hpp"
#include "util/type/type_comparator.hpp"

using pascc::util::ArrayType;
using pascc::util::BasicType;
using pascc::util::BuiltInType;
using pascc::util::SubprogType;
using pascc::util::SymType;
using pascc::util::TypeComparator;
using pascc::util::VarType;

namespace pascc::semant {

auto SemantVisitor::isOk() -> bool
{
  return context_.error_msgs_.empty();
}

auto SemantVisitor::error_msgs() const -> const std::vector<std::string> &
{
  return context_.error_msgs_;
}

void SemantVisitor::visit([[maybe_unused]] ast::Block &node)
{
  // ! Block 不应该被直接访问，而是以 ProgramBlock、ProcBlock、FuncBlock 的形式出现
  throw std::runtime_error("Block should not be visited directly");
}

// do nothing
void SemantVisitor::visit([[maybe_unused]] ast::Number &node) {}

void SemantVisitor::visit(ast::Constant &node)
{
  /**
    当 type == "reference" 且 sign = -1 时查符号表(找不到则返回未定义的错误)，若为字符串类型，则返回错误，程序终止。
   */
  if (node.sign() == -1 && (node.type() != "integer" && node.type() != "real")) {
    context_.genErrorMsg(node.location(), "invaild constant type.");
  }
}

void SemantVisitor::visit(ast::StringLiteral &node)
{
  node.setType(SymType::StringType().clone());
}

void SemantVisitor::visit(ast::BoolExpr &node)
{
  /**
   * 表示式应当为 bool 类型
   */
  node.expr().accept(*this);
  if (!context_.cmp_(node.expr().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    context_.genErrorMsg(node.location(), "boolean type expected.");
    return;
  }
  node.setType(SymType::BooleanType().clone());
}

// do nothing
void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node) {}

void SemantVisitor::visitLogicBinaryExpr(ast::BinaryExpr &node)
{
  if (!context_.cmp_(node.lhs().type(), SymType::BooleanType()) || !context_.cmp_(node.rhs().type(), SymType::BooleanType())) {
    context_.genErrorMsg(node.location(), "boolean type expected");
    node.setType(SymType::NoType().clone());
  } else {
    node.setType(SymType::BooleanType().clone());
  }
}

void SemantVisitor::visitComparisonBinaryExpr(ast::BinaryExpr &node)
{
  if (
      !context_.cmp_(node.lhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.lhs().type(), SymType::RealType())
  ) {  // 左侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else if (
      !context_.cmp_(node.rhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.rhs().type(), SymType::RealType())
  ) {  // 右侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else {
    node.setType(SymType::BooleanType().clone());
  }
}

void SemantVisitor::visitArithmeticBinaryExpr(ast::BinaryExpr &node)
{
  if (
      !context_.cmp_(node.lhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.lhs().type(), SymType::RealType())
  ) {  // 左侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else if (
      !context_.cmp_(node.rhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.rhs().type(), SymType::RealType())
  ) {  // 右侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else {
    if (context_.cmp_(
            node.lhs().type(),
            SymType::IntegerType()
        ) &&
        context_.cmp_(
            node.rhs().type(),
            SymType::IntegerType()
        )) {  // 两侧都是整数
      node.setType(SymType::IntegerType().clone());
    } else {  // 至少有一个是实数
      node.setType(SymType::RealType().clone());
    }
  }
}

void SemantVisitor::visitFDivBinaryExpr(ast::BinaryExpr &node)
{
  if (
      !context_.cmp_(node.lhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.lhs().type(), SymType::RealType())
  ) {  // 左侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else if (
      !context_.cmp_(node.rhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.rhs().type(), SymType::RealType())
  ) {  // 右侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else {
    node.setType(SymType::RealType().clone());
  }
}

void SemantVisitor::visitIntOpBinaryExpr(ast::BinaryExpr &node)
{
  if (
      !context_.cmp_(node.lhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.lhs().type(), SymType::RealType())
  ) {  // 左侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else if (
      !context_.cmp_(node.rhs().type(), SymType::IntegerType()) &&
      !context_.cmp_(node.rhs().type(), SymType::RealType())
  ) {  // 右侧不是整数或实数
    context_.genErrorMsg(node.location(), "integer or real type expected");
    node.setType(SymType::NoType().clone());
  } else {
    if (context_.cmp_(
            node.lhs().type(),
            SymType::IntegerType()
        ) &&
        context_.cmp_(
            node.rhs().type(),
            SymType::IntegerType()
        )) {  // 两侧都是整数
      node.setType(SymType::IntegerType().clone());
    } else {  // 至少有一个是实数
      context_.genErrorMsg(node.location(), "integer type expected");
      node.setType(SymType::NoType().clone());
    }
  }
}

void SemantVisitor::visit(ast::BinaryExpr &node)
{
  /**
   * * 当二元运算符为 'and' 或 'or'， 
   * * 左右两侧必然同时为 bool，否则错误， type 赋值为 bool
   * 
   * * 当二元运算符为 '<' 、'>' 、'<=' 、'>=' 、'=' 、'<>'，
   * * 左右两侧必然为 integer 或 real，否则错误，type 赋值为 bool
   * 
   * * 当二元运算符为 ‘/’、'div' 、'mod' 、 '+' 、'-'、'*'，
   * * 左右两侧必须为 integer 或 real，否则返回错误，程序终止。
   * ! 只有全为 integer 时，type 赋值为 integer，其余情况均为real。
   * ! 当二元运算符为 ‘div' 或 'mod' , 右侧的 expr 若在编译时可求得其值，则其值不应为0，否则错误
   * ! 若为 'mod'，则左右两侧必须为 integer
   * ! 若为 'div'，则左右两侧必须为 integer
   */

  node.lhs().accept(*this);
  node.rhs().accept(*this);

  switch (node.op()) {
    case ast::BinOp::AND:
    case ast::BinOp::OR:
      visitLogicBinaryExpr(node);
      break;

    case ast::BinOp::EQ:
    case ast::BinOp::NE:
    case ast::BinOp::LT:
    case ast::BinOp::GT:
    case ast::BinOp::LE:
    case ast::BinOp::GE:
      visitComparisonBinaryExpr(node);
      break;

    case ast::BinOp::PLUS:
    case ast::BinOp::MINUS:
    case ast::BinOp::MUL:
      visitArithmeticBinaryExpr(node);
      break;

    case ast::BinOp::FDIV:
      visitFDivBinaryExpr(node);
      break;

    case ast::BinOp::IDIV:
    case ast::BinOp::MOD:
      visitIntOpBinaryExpr(node);
      break;
  }
}

void SemantVisitor::visit(ast::UnaryExpr &node)
{
  /**
   当一元运算符为 '-' 或 '+'，则 expr_ 必然是 integer 或 real 类型，否则返回错误，程序终止。
   当一元运算符为 'not' 时，则 expr_ 必然是 bool 类型，否则返回错误，程序终止。
   isAssignable = 0;
   按照运算规则处理value。
   */
  node.expr().accept(*this);
  if (node.op() == ast::UnaryOp::MINUS || node.op() == ast::UnaryOp::PLUS) {
    if (context_.cmp_(node.expr().type(), SymType::IntegerType()) && context_.cmp_(node.expr().type(), SymType::RealType())) {
      context_.genErrorMsg(node.location(), "integer or real type expected.");
    }
  }
  if (node.op() == ast::UnaryOp::NOT) {
    if (context_.cmp_(node.expr().type(), SymType::BooleanType())) {
      context_.genErrorMsg(node.location(), "boolean type expected.");
    }
  }
  node.setType(node.expr().type().clone());
}

void SemantVisitor::visit(ast::FuncCall &node)
{
  /**
   在符号表中查找声明，若找不到，则返回错误，程序终止。
   获取FuncCall类型表达式。
   对actuals_中的每一个参数进行类型检查，若存在不相等的情况，则返回错误，程序终止。
   type = FuncCall的return value
   isAssignable = 0
   */
  const auto *prototype = context_.subprogtab_.probe(node.funcid());
  if (prototype == nullptr) {
    context_.genErrorMsg(node.location(), "undefined identifier ", node.funcid());
    return;
  }
  if (node.actuals().size() != prototype->formalParams().size()) {
    context_.genErrorMsg(node.location(), "count of actuals doesn't match.");
  }

  for (unsigned i = 0; i < node.actuals().size(); i++) {
    const auto &actual = node.actuals()[i];
    const auto &formal = prototype->formalParams()[i].second;
    actual->accept(*this);
    if (formal->isRef() && !actual->isChangeable()) {
      context_.genErrorMsg(actual->location(), "This actual must be modifiable.");
    } else if (!context_.cmp_(formal->symType(), actual->type()) && util::TypeComparator::cast(actual->type(), formal->symType())) {
      context_.genErrorMsg(actual->location(), "actual does not match.");
    }
  }

  node.setType(prototype->returnType().clone());
}

void SemantVisitor::visit(ast::VarId &node)
{
  // TODO(张新博): 如果在类型表或者子过程表中查到了, 那么报错

  // 如果在常量表中找到了，那么它的值是不可被修改的
  const auto *consttype = context_.consttab_.lookup(node.id());
  if (consttype != nullptr) {
    node.setType(consttype->clone());
    node.setChangeable(false);
    return;
  }

  // 如果在变量表中找到了，那么它的值是可以被修改的
  const auto *vartype = context_.vartab_.lookup(node.id());
  if (vartype == nullptr) {
    // 如果在变量表中找不到，那么报错
    context_.genErrorMsg(node.location(), "undefined identifier ", node.id());
    node.setType(SymType::NoType().clone());
    return;
  }
  node.setType(vartype->symType().clone());
  node.setChangeable(true);
}

void SemantVisitor::visit(ast::IndexedVar &node)
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
  node.varAccess().accept(*this);
  const auto &periods = node.varAccess().type().arrayType().periods();
  if (node.varAccess().type().eType() != SymType::Type::ARRAY) {
    context_.genErrorMsg(node.location(), "array type expected.");
    return;
  }
  if (periods.size() != node.indices().size()) {
    context_.genErrorMsg(node.location(), "count of index doesn't match.");
  }
  for (const auto &index : node.indices()) {
    index->accept(*this);
    if (!context_.cmp_(index->type(), SymType::IntegerType())) {
      context_.genErrorMsg(node.location(), "index must be integer.");
      return;
    }
  }
  node.setType(node.varAccess().type().arrayType().baseType().clone());
}

void SemantVisitor::visit(ast::FieldDesignator &node)
{
  /**
    1. 获取 assignable_ 的类型表达式。
    2. 在符号表中匹配 field_
      2.1 若结构体中不包含 field_ 打印错误，程序终止。
    3. 获取 field_ 的类型表达式并记录在父类 Expr 的 type 中。
      (这表示整个表达式的类型，将会用在后续的类型检查中)。
   */
  node.varAccess().accept(*this);
  if (node.varAccess().type().eType() != SymType::Type::RECORD) {
    context_.genErrorMsg(node.location(), "should be a record type");
    return;
  }
  if (!node.varAccess().type().recordType().fields().contains(node.field())) {
    context_.genErrorMsg(node.location(), "record does not have this field.");
    return;
  }
  node.setType(node.varAccess().type().recordType().fields().at(node.field())->clone());
}

void SemantVisitor::visit(ast::ConstDecl &node)
{
  /**
   * 判断有无重定义。
   * 向符号表中插入 <id , constant->type()>
   */
  node.constant().accept(*this);
  const auto *found = context_.consttab_.probe(node.constId());
  if (found != nullptr) {
    context_.genErrorMsg(node.location(), "duplicate identifier ", node.constId());
    return;
  }
  const SymType *type = nullptr;
  // 如果是引用别的常量，查找引用的常量的类型
  if (node.constant().type() == "reference") {
    found = context_.consttab_.lookup(std::get<std::string>(node.constant().value()));
    if (found == nullptr) {
      context_.genErrorMsg(node.location(), "undefined constant identifier ", std::get<std::string>(node.constant().value()));
      return;
    }
    type = found;
  }

  if (type == nullptr) {
    if (node.constant().type() == "integer") {
      type = &SymType::IntegerType();
    } else if (node.constant().type() == "real") {
      type = &SymType::RealType();
    } else if (node.constant().type() == "boolean") {
      type = &SymType::BooleanType();
    } else if (node.constant().type() == "char") {
      type = &SymType::CharType();
    } else {
      type = &SymType::StringType();
    }
  }
  context_.consttab_.insert(node.constId(), const_cast<SymType *>(type));
}

void SemantVisitor::visit(ast::ConstDeclPart &node)
{
  /**
   * 访问各个 ConstDecl
   */
  for (auto &constDecl : node.constDecls()) {
    constDecl->accept(*this);
  }
}

void SemantVisitor::visit(ast::TypeId &node)
{
  /**
   * 检测 type 是否在符号表中
   * 赋值给父类 TypeDenoter 的 type
   */
  const auto &symType = context_.typetab_.lookup(node.id());
  if (symType == nullptr) {
    context_.genErrorMsg(node.location(), "undefined Type ", node.id());
    return;
  }
  node.setType(symType->clone());
}

void SemantVisitor::visit(ast::Period &node)
{
  /**
   检测上界是否大于下界。 
   */
  node.low().accept(*this);
  node.high().accept(*this);
  if (node.low().type() != "integer" || node.high().type() != "integer") {
    context_.genErrorMsg(node.location(), "bound must be integer.");
    return;
  }
  if (std::get<int>(node.low().value()) > std::get<int>(node.high().value())) {
    context_.genErrorMsg(node.location(), "low bound can't be larger than high bound.");
  }
}

void SemantVisitor::visit(ast::ArrayType &node)
{
  /**
   组装 type 和 periods 打包赋值给父类 TypeDenoter 的 type
   */
  node.ofType().accept(*this);
  ArrayType arrayType;
  for (const auto &period : node.periods()) {
    period->accept(*this);
    arrayType.addPeriod(std::get<int>(period->low().value()), std::get<int>(period->high().value()));
  }
  arrayType.setBaseType(&node.ofType().symType());
  SymType symType(arrayType);
  node.setSymType(std::make_unique<SymType>(symType));
}

void SemantVisitor::visit(ast::RecordType &node)
{
  /**
    将fields中的所有field的 TypeDenoter,id_list_ 打包赋值给父类 TypeDenoter 的type
   */
  util::RecordType recordType;
  context_.enterScope();
  for (const auto &field : node.fields()) {
    field->accept(*this);
    for (const auto &name : field->idList()) {
      recordType.addField(name, &field->type().symType());
    }
  }
  context_.exitScope();
  SymType symType(recordType);
  node.setSymType(std::make_unique<SymType>(symType));
}

void SemantVisitor::visit(ast::TypeDecl &node)
{
  /**
   * 符号表重定义检测。
   * 符号表中新增一项<type_id_, type_denoter_.type()>
   */
  const auto *tmp = context_.typetab_.probe(node.typeId());
  if (tmp != nullptr) {
    context_.genErrorMsg(node.location(), "duplicated identifier ", node.typeId());
    return;
  }
  tmp = context_.consttab_.probe(node.typeId());
  if (tmp != nullptr) {
    context_.genErrorMsg(node.location(), "duplicated identifier ", node.typeId());
    return;
  }
  node.typeDenoter().accept(*this);
  context_.typetab_.insert(node.typeId(), &node.typeDenoter().symType());
}

void SemantVisitor::visit(ast::TypeDeclPart &node)
{
  /**
   * 访问各个TypeDecl
   */
  for (auto &typeDecl : node.typeDecls()) {
    typeDecl->accept(*this);
  }
}

void SemantVisitor::visit(ast::VarDeclPart &node)
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
  // node.type 更新
  node.type().accept(*this);
  // node.varType 更新
  node.setVarType(std::make_unique<VarType>(false, &node.type().symType()));
  for (auto &id : node.idList()) {
    if (context_.vartab_.probe(id) != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", id);
      continue;
    }
    context_.vartab_.insert(id, &node.varType());
    context_.formal_params_.emplace_back(id, &node.varType());
  }
}

void SemantVisitor::visit(ast::VarParamSpec &node)
{
  // node.type 更新
  node.type().accept(*this);
  // node.varType 更新
  node.setVarType(std::make_unique<VarType>(true, &node.type().symType()));
  for (auto &id : node.idList()) {
    if (context_.vartab_.probe(id) != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", id);
      continue;
    }
    context_.vartab_.insert(id, &node.varType());
    context_.formal_params_.emplace_back(id, &node.varType());
  }
}

void SemantVisitor::visit(ast::VarDecl &node)
{
  /*
    查找符号表，检测重定义
    插入符号表
  */
  // node.type 更新
  node.type().accept(*this);
  // node.varType 更新
  node.setVarType(std::make_unique<VarType>(false, &node.type().symType()));
  for (const auto &id : node.idList()) {
    const auto *tmp = context_.typetab_.probe(id);
    if (tmp != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", id);
      return;
    }
    tmp = context_.consttab_.probe(id);
    if (tmp != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", id);
      return;
    }
    const auto *tmp1 = context_.vartab_.probe(id);
    if (tmp1 != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", id);
      return;
    }
    context_.vartab_.insert(id, &node.varType());
  }
}

void SemantVisitor::visit(ast::ProcHead &node)
{
  /**
   * 把每一个 FormalParam 组装起来，再和id组装，插入符号表
   * 符号表进入下一级。
   * 对于每一个FormalParam 符号表判断重定义，再加入<id_list, typedenoter>
   */
  if (
      context_.subprogtab_.probe(node.procId()) != nullptr || context_.vartab_.probe(node.procId()) != nullptr || context_.consttab_.probe(node.procId()) != nullptr
  ) {
    context_.genErrorMsg(node.location(), "dupilcated identify function", node.procId());
    return;
  }
  context_.formal_params_.clear();
  for (const auto &formalParam : node.formalParams()) {
    formalParam->accept(*this);  // 填充 context_.formal_params_
  }

  // 设置proc_type_
  node.setProcType(
      std::make_unique<SubprogType>(
          false,
          nullptr,
          std::move(context_.formal_params_)
      )
  );
  // 构造 SubprogType 插入到 subprogtab
  context_.subprogtab_.insert(node.procId(), &node.procType());
  context_.pushFunc(node.procId());
  context_.enterScope();
  // 遍历 context_.formal_params_ 插入到 vartab_
  for (const auto &[varid, vartype] : node.procType().formalParams()) {
    if (context_.vartab_.probe(varid) != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier ", varid);
    } else {
      context_.vartab_.insert(varid, vartype);
    }
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
  context_.popFunc();
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
  if (
      context_.subprogtab_.probe(node.funcId()) != nullptr || context_.vartab_.probe(node.funcId()) != nullptr || context_.consttab_.probe(node.funcId()) != nullptr
  )
  {
    context_.genErrorMsg(node.location(), "dupilcated identify function", node.funcId());
    return;
  }

  node.returnType().accept(*this);
  node.setFuncIdType(
      std::make_unique<VarType>(
          false,
          &node.returnType().symType()
      )
  );
  context_.formal_params_.clear();
  for (const auto &formalParam : node.formalParams()) {
    formalParam->accept(*this);  // 填充 context_.formal_params_
  }
  node.setFuncType(
      std::make_unique<SubprogType>(
          true,
          &node.returnType().symType(),
          std::move(context_.formal_params_)
      )
  );
  // 构造 SubprogType 插入到 subprogtab
  context_.subprogtab_.insert(
      node.funcId(),
      &node.funcType()
  );
  context_.pushFunc(node.funcId());
  // 作用域来到下一级
  context_.enterScope();
  // 插入 函数名字和返回值类型。
  context_.vartab_.insert(node.funcId(), &node.funcIdType());
  // 遍历 context_.formal_params_ 插入到 vartab_
  for (const auto &[varid, vartype] : node.funcType().formalParams()) {
    if (context_.vartab_.probe(varid) != nullptr) {
      context_.genErrorMsg(node.location(), "dupilcated identify", varid);
    } else {
      context_.vartab_.insert(varid, vartype);
    }
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
  context_.popFunc();
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
  
  node.cond().accept(*this);
  if (!context_.cmp_(node.cond().type(), SymType::BooleanType())) {
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
  for (const auto &cons : node.constants()) {
    cons->accept(*this);
    std::unique_ptr<SymType> type;
    if (cons->type() == "integer") {
      type = std::make_unique<SymType>(BuiltInType{BasicType::INTEGER});
    } else if (cons->type() == "real") {
      type = std::make_unique<SymType>(BuiltInType{BasicType::REAL});
    } else if (cons->type() == "boolean") {
      type = std::make_unique<SymType>(BuiltInType{BasicType::BOOLEAN});
    } else if (cons->type() == "char") {
      type = std::make_unique<SymType>(BuiltInType{BasicType::CHAR});
    } else {
      type = std::make_unique<SymType>(BuiltInType{BasicType::STRING});
    }
    if (!context_.cmp_(*context_.case_stmt_type_, *type) && !TypeComparator::cast(*type, *context_.case_stmt_type_)) {
      context_.genErrorMsg(node.location(), "case type doesn't match expr's type.");
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
  if (!context_.cmp_(node.cond().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    context_.genErrorMsg(node.location(), "boolean type expected.");
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
  if (!context_.cmp_(node.cond().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    // 条件不是bool
    context_.genErrorMsg(node.location(), "boolean type expected.");
  }
  node.body().accept(*this);
}

void SemantVisitor::visit(ast::ForStmt &node)
{
  /** 
  如果ctrl_var和init_val类型不一致则返回错误，程序终止。
  访问body
  */
  node.ctrlVar().accept(*this);
  node.initVal().accept(*this);
  node.endVal().accept(*this);
  if (
      (
          !context_.cmp_(
              node.ctrlVar().type(),
              node.initVal().type()
          ) &&
          !TypeComparator::cast(
              node.ctrlVar().type(),
              node.initVal().type()
          )
      ) ||
      (!context_.cmp_(
           node.ctrlVar().type(),
           node.endVal().type()
       ) &&
       !TypeComparator::cast(
           node.ctrlVar().type(),
           node.endVal().type()
       ))
  ) {
    context_.genErrorMsg(node.location(), "ctrlVal type doesn't match initVal or endVal");
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
  if (!node.lhs().isChangeable()) {
    context_.genErrorMsg(node.location(), "Variable identifier expected.");
    return;
  }
  if (context_.cmp_(node.lhs().type(), node.rhs().type())) {
    return;
  }
  if (TypeComparator::cast(node.rhs().type(), node.lhs().type())) {
    return;
  }
  // TODO(): 重载type的流运算符
  context_.genErrorMsg(node.location(), "type error");
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
    context_.genErrorMsg(node.location(), "undefined procedure call.");
    return;
  }
  const auto &actuals_expected = proc->formalParams();
  if (actuals_expected.size() != node.actuals().size()) {
    context_.genErrorMsg(node.location(), "actual list do not match.");
    return;
  }

  for (int i = 0; i < static_cast<int>(node.actuals().size()); i++) {
    node.actuals()[i]->accept(*this);
    if (!node.actuals()[i]->isChangeable() && actuals_expected[i].second->isRef()) {
      context_.genErrorMsg(node.location(), "actual list do not match.");
      return;
    }
    if (!context_.cmp_(actuals_expected[i].second->symType(), node.actuals()[i]->type()) && !TypeComparator::cast(node.actuals()[i]->type(), actuals_expected[i].second->symType())) {
      context_.genErrorMsg(node.location(), "actual list do not match.");
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
    if (!actual->isChangeable()) {
      context_.genErrorMsg(node.location(), "Assignable actual expected.");
    }
  }
}

void SemantVisitor::visit(ast::WriteStmt &node)
{
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
}

void SemantVisitor::visit(ast::ReadlnStmt &node)
{
  /**
  * 对actuals_中的每一个参数进行类型检查。
  */
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
    if (!actual->isChangeable()) {
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
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
}

void SemantVisitor::visit([[maybe_unused]] ast::ExitStmt &node)
{
  auto nowfunc              = context_.topFunc();
  const auto canreturn      = context_.subprogtab_.probe(nowfunc)->isFunc();
  const SymType &returntype = context_.subprogtab_.probe(nowfunc)->returnType();
  if (node.actuals().empty() && !canreturn) {
    return;
  }
  if (node.actuals().size() == 1 && canreturn) {
    node.actuals()[0]->accept(*this);
    const auto &thistype = node.actuals()[0]->type();
    if (!context_.cmp_(thistype, returntype) && !TypeComparator::cast(thistype, returntype)) {
      context_.genErrorMsg(node.location(), "return value type doesn't match.");
    }
    return;
  }
  context_.genErrorMsg(node.location(), "invaild use of exit.");
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
  context_.exitScope();
}

void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node)
{
  context_.pushFunc(node.programName());
  SubprogType basic(true, &SymType::IntegerType());
  context_.subprogtab_.insert(node.programName(), &basic);
}

void SemantVisitor::visit(ast::Program &node)
{
  // Program -> ProgramHead ';' ProgramBlock '.'
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
