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

/**
 * 当sign为-1且该常量的类型不为整型或者实型时，输出错误信息：违法的常量类型
 */
void SemantVisitor::visit(ast::Constant &node)
{
  if (node.sign() == -1 && (node.type() != "integer" && node.type() != "real")) {
    context_.genErrorMsg(node.location(), "invaild constant type.");
  }
}

/**
 * @brief 设置字符串类型 
 */
void SemantVisitor::visit(ast::StringLiteral &node)
{
  node.setType(SymType::StringType().clone());
}

/**
 * 如果子表达式的类型和内置的布尔类型不相同，输出错误信息：期待的是布尔类型。并退出
 * 如果子表达式的类型与内置的布尔类型相同，将当前节点类型设置为布尔类型
 */
void SemantVisitor::visit(ast::BoolExpr &node)
{
  node.expr().accept(*this);
  if (!context_.cmp_(node.expr().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    context_.genErrorMsg(node.location(), "boolean type expected.");
    node.setType(SymType::NoType().clone());
  } else {
    node.setType(SymType::BooleanType().clone());
  }
}

// do nothing
void SemantVisitor::visit([[maybe_unused]] ast::UnsignedConstant &node) {}

/**
 * 如果有任何一侧操作数不为布尔类型，输出错误信息：期待布尔类型。将当前节点类型设置为无类型
 * 如果两侧操作数都为布尔类型，将当前节点类型设置为布尔类型
 * 
 * @attention 这里进行的是LogicBinaryExpr，也就是与运算(AND),或运算(OR),非运算(NOT)
 */
void SemantVisitor::visitLogicBinaryExpr(ast::BinaryExpr &node)
{
  if (!context_.cmp_(node.lhs().type(), SymType::BooleanType()) || !context_.cmp_(node.rhs().type(), SymType::BooleanType())) {
    context_.genErrorMsg(node.location(), "boolean type expected");
    node.setType(SymType::NoType().clone());
  } else {
    node.setType(SymType::BooleanType().clone());
  }
}

/**
 * 如果二元运算符为'='或者'<>'时，如果两侧操作数的类型相同OR
 *                               左侧操作数的类型可以转换为右侧操作数的类型OR
 *                               右侧操作数的类型可以转换为左侧操作数的类型
 * 将当前节点的类型设置为布尔类型，否则输出错误信息：类型不匹配。并退出
 * 如果二元运算符不为'='或者'<>'时，如果两侧操作数类型都不为整型||实型，输出错误信息：希望得到的是整型和实型
 * 
 * @attention 这里进行的是ComparisonBinaryExpr，也就是等于(=),不等于(<>),大于(>),小于(<),大于等于(>=),小于等于(<=)
 */
void SemantVisitor::visitComparisonBinaryExpr(ast::BinaryExpr &node)
{
  // 等号或不等号需要特殊处理
  if (node.op() == ast::BinOp::EQ || node.op() == ast::BinOp::NE) {
    bool eq       = context_.cmp_(node.lhs().type(), node.rhs().type());
    bool castable = TypeComparator::cast(
                        node.lhs().type(),
                        node.rhs().type()
                    ) ||
                    TypeComparator::cast(
                        node.rhs().type(),
                        node.lhs().type()
                    );
    if (!eq && !castable) {
      context_.genErrorMsg(node.location(), "type mismatch");
      node.setType(SymType::NoType().clone());
    } else {
      node.setType(SymType::BooleanType().clone());
    }
    return;
  }

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

/**
 * 如果两侧操作数的类型有任意一侧不为整型或者实型，输出错误信息：希望得到的是整型和实型。将当前节点类型设置为无类型
 * 否则，如果两侧操作数的类型两侧均为整型，将当前节点类型设置为整型
 *       如果两侧操作数的类型至少有一侧为实型，将当前节点类型设置为实型
 *
 * @attention 这里进行的是ArithmeticBinaryExpr，也就是加法运算符(+),减法运算符(-),乘法运算符(*)
 */
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

/**
 * 如果两侧操作数的类型有任意一侧不为整型或者实型，输出错误信息：希望得到的是整型或者实型。将当前节点类型设置为无类型
 * 否则，将当前节点的类型设置为实型
 *
 * @attention 这里进行的是FDivBinaryExpr，也就是浮点数除法运算符(/)
 */
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

/**
 * 如果两侧操作数的类型有任意一侧不为整型或者实型，输出错误信息：希望得到的是整型或者实型。将当前节点类型设置为无类型
 * 否则，如果两侧操作数的类型均为整型，将当前节点类型设置为整型
 * 否则，如果两侧操作数的类型至少有一侧为实型，输出错误消息：希望得到的是实型。将当前节点类型设置为无类型
 *
 * @attention 这里进行的是IntOpBinaryExpr，也就是整数除法运算符(DIV)和取模运算符(MOD)
 */
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

/**
 * @brief 根据二元运算符判断采用何种函数进行分析
 * 
 * @attention    只有全为 integer 时，type 赋值为 integer，其余情况均为real。 \n 
 *               当二元运算符为 ‘div' 或 'mod' , 右侧的 expr 若在编译时可求得其值，则其值不应为0，否则错误 \n 
 *               若为 'mod'，则左右两侧必须为 integer \n 
 *               若为 'div'，则左右两侧必须为 integer
 */
void SemantVisitor::visit(ast::BinaryExpr &node)
{
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

/**
 * 如果当前一元运算符为'+'或者'-'时，如果表达式的类型不为整型或者实型，输出错误信息：希望得到的是整型和实型
 * 如果当前一元运算符为'NOT'时，如果表达式的类型不为布尔类型或者整型，输出错误信息：希望得到的是布尔类型和整型
 * 当前节点类型保持不变
 */
void SemantVisitor::visit(ast::UnaryExpr &node)
{
  node.expr().accept(*this);
  if (node.op() == ast::UnaryOp::MINUS || node.op() == ast::UnaryOp::PLUS) {
    if (!context_.cmp_(node.expr().type(), SymType::IntegerType()) && !context_.cmp_(node.expr().type(), SymType::RealType())) {
      context_.genErrorMsg(node.location(), "integer or real type expected.");
    }
  } else if (node.op() == ast::UnaryOp::NOT) {
    if (!context_.cmp_(node.expr().type(), SymType::BooleanType()) && !context_.cmp_(node.expr().type(), SymType::IntegerType())) {
      context_.genErrorMsg(node.location(), "expected boolean or integer.");
    }
  } else {
    context_.genErrorMsg(node.location(), "unexpected unary operator.");
  }
  node.setType(node.expr().type().clone());
}

/**
 * 如果在当前作用域中查找不到函数声明，输出错误信息：未定义的标识符。并退出
 * 如果当前节点的实参个数不等于函数声明中形参个数，输出错误信息：参数个数不匹配
 * 如果函数声明中的形参是按引用传递的，但是当前节点的实参是不可修改量，输出错误信息：实参必须可以修改
 * 否则，如果当前节点实参和函数声明中的形参类型不能一一匹配并且对于不匹配的参数来说实参的类型无法转换为形参的类型，输出错误信息：参数类型不匹配
 */
void SemantVisitor::visit(ast::FuncCall &node)
{
  const auto *prototype = context_.subprogtab_.lookup(node.funcid());
  if (prototype == nullptr) {
    context_.genErrorMsg(node.location(), "undefined identifier " + node.funcid());
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
    } else if (!context_.cmp_(formal->symType(), actual->type()) && !util::TypeComparator::cast(actual->type(), formal->symType())) {
      context_.genErrorMsg(actual->location(), "actual does not match.");
    }
  }

  node.setType(prototype->returnType().clone());
}

/**
 * 如果引用变量的名字在常量表中找到了，将节点类型设置为常量表中已有声明的类型，并且设置为值不可修改
 * 如果引用变量的名字在变量表中找到了，将节点类型设置为变量表中已有声明的类型，并且设置为值可以修改
 * 若引用变量的名字在两张表中都找不到，输出错误信息：标识符未定义。将节点类型设置为无类型
 */
void SemantVisitor::visit(ast::VarId &node)
{
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
    // 如果在变量表和常量表中都找不到，那么报错
    context_.genErrorMsg(node.location(), "undefined identifier " + node.id());
    node.setType(SymType::NoType().clone());
    return;
  }
  node.setType(vartype->symType().clone());
  node.setChangeable(true);
}

/**
 * 如果节点类型不为数组类型，输出错误信息：希望得到数组类型。并退出
 * 如果节点的索引数量大于数组维度，输出错误信息：索引数量不匹配。并退出
 * 如果节点的索引类型有不为整型的，输出错误信息：索引类型必须是整型。并退出
 * 如果节点的索引数量小于数组维度，设置节点类型为数组元素的类型
 */
void SemantVisitor::visit(ast::IndexedVar &node)
{
  node.varAccess().accept(*this);
  const auto &periods = node.varAccess().type().arrayType().periods();
  if (node.varAccess().type().eType() != SymType::Type::ARRAY) {
    context_.genErrorMsg(node.location(), "array type expected.");
    return;
  }
  if (periods.size() < node.indices().size()) {
    context_.genErrorMsg(node.location(), "count of index doesn't match.");
    return;
  }
  unsigned i = 0;
  for (; i < node.indices().size(); i++) {
    const auto &index = node.indices()[i];
    index->accept(*this);
    if (!context_.cmp_(index->type(), SymType::IntegerType())) {
      context_.genErrorMsg(node.location(), "index must be integer.");
      return;
    }
  }
  if (node.indices().size() < periods.size()) {
    ArrayType arraytype;
    arraytype.setBaseType(const_cast<SymType *>(&node.varAccess().type().arrayType().baseType()));
    for (; i < periods.size(); i++) {
      const auto &[lb, ub] = periods[i];
      arraytype.addPeriod(lb, ub);
    }
    node.setType(std::make_unique<SymType>(arraytype));
    return;
  }
  node.setType(node.varAccess().type().arrayType().baseType().clone());
}

/**
 * 如果节点类型不为记录类型，输出错误信息：此处应当是一条记录类型。并退出
 * 如果节点字段不存在于结构体中，输出错误信息：记录中不存在该字段。并退出
 * 将节点类型设置为结构体字段类型
 */
void SemantVisitor::visit(ast::FieldDesignator &node)
{
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

/**
 * 如果节点已经在常量符号表中出现，输出错误信息：标识符重定义。并退出
 * 如果节点的类型为引用类型，如果在常量符号表找不到节点引用的对象，输出错误信息：未定义的常量标识符。并退出
 * 如果节点的类型为引用类型，并且在常量符号表找到了节点引用的对象，将节点类型设置为引用的对象的类型
 * 如果节点不是引用类型，向常量符号表中插入该节点
 */
void SemantVisitor::visit(ast::ConstDecl &node)
{
  node.constant().accept(*this);
  const auto *found = context_.consttab_.probe(node.constId());
  if (found != nullptr) {
    context_.genErrorMsg(node.location(), "duplicate identifier " + node.constId());
    return;
  }
  const SymType *type = nullptr;
  // 如果是引用别的常量，查找引用的常量的类型
  if (node.constant().type() == "reference") {
    found = context_.consttab_.lookup(std::get<std::string>(node.constant().value()));
    if (found == nullptr) {
      context_.genErrorMsg(node.location(), "undefined constant identifier " + std::get<std::string>(node.constant().value()));
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

/**
 * @brief 访问各个 ConstDecl
 */
void SemantVisitor::visit(ast::ConstDeclPart &node)
{
  for (auto &constDecl : node.constDecls()) {
    constDecl->accept(*this);
  }
}

/**
 * 如果节点没有出现在类型符号表中，输出错误信息：未定义的类型。并退出
 * 将节点类型设置为在类型符号表中找到的类型
 */
void SemantVisitor::visit(ast::TypeId &node)
{
  const auto &symType = context_.typetab_.lookup(node.id());
  if (symType == nullptr) {
    context_.genErrorMsg(node.location(), "undefined Type " + node.id());
    return;
  }
  node.setType(symType->clone());
}

/**
 * 检测节点上下界的类型，如果有一方不为整型，输出错误信息：边界必须是整数。并退出
 * 如果节点下界的值大于节点上界的值，输出错误信息：下界值不能大于上界值
 */
void SemantVisitor::visit(ast::Period &node)
{
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

/**
 * 对每一维度的数组维度信息进行记录，包括下界值和上界值
 * 将数组基本类型和数组维度信息打包赋值给节点类型
 */
void SemantVisitor::visit(ast::ArrayType &node)
{
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

/**
 * 对记录类型的每个字段进行记录，其中每个字段可能包含多个标识符
 * 将每一字段类型和字段包含的标识符信息打包赋值给节点类型
 */
void SemantVisitor::visit(ast::RecordType &node)
{
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

/**
 * 如果节点符号已经在当前类型符号表中存在，输出错误信息：标识符重定义。并退出
 * 如果节点符号已经在当前常量符号表中存在，输出错误信息：标识符重定义。并退出
 * 将节点的类型标识符和类型表达式打包添加到类型符号表中
 */
void SemantVisitor::visit(ast::TypeDecl &node)
{
  const auto *tmp = context_.typetab_.probe(node.typeId());
  if (tmp != nullptr) {
    context_.genErrorMsg(node.location(), "duplicated identifier " + node.typeId());
    return;
  }
  tmp = context_.consttab_.probe(node.typeId());
  if (tmp != nullptr) {
    context_.genErrorMsg(node.location(), "duplicated identifier " + node.typeId());
    return;
  }
  node.typeDenoter().accept(*this);
  context_.typetab_.insert(node.typeId(), &node.typeDenoter().symType());
}

/**
   * @brief 访问各个类型声明
   */
void SemantVisitor::visit(ast::TypeDeclPart &node)
{
  for (auto &typeDecl : node.typeDecls()) {
    typeDecl->accept(*this);
  }
}

/**
 * @brief 访问各个 VarDecl
 */
void SemantVisitor::visit(ast::VarDeclPart &node)
{
  for (auto &varDecl : node.varDecls()) {
    varDecl->accept(*this);
  }
}

/**
 * @brief node.type 和node.varType 更新
 * 
 * @attention 这里不要检查符号表，填充到 context_.formal_params_ 中，交由上层检查
 */
void SemantVisitor::visit(ast::ValueParamSpec &node)
{
  node.type().accept(*this);
  node.setVarType(
      std::make_unique<VarType>(false, &node.type().symType())
  );

  for (auto &id : node.idList()) {
    context_.formal_params_.emplace_back(id, &node.varType());
  }
}

/**
 * @brief node.type 和 node.varType 更新
 * 
 * @attention 这里不要检查符号表，填充到 context_.formal_params_ 中，交由上层检查
 */
void SemantVisitor::visit(ast::VarParamSpec &node)
{
  node.type().accept(*this);
  node.setVarType(
      std::make_unique<VarType>(true, &node.type().symType())
  );

  for (auto &id : node.idList()) {
    context_.formal_params_.emplace_back(id, &node.varType());
  }
}

/**
 * 如果节点符号已经在当前类型符号表中存在，输出错误信息：标识符重定义。并退出
 * 如果节点符号已经在当前常量符号表中存在，输出错误信息：标识符重定义。并退出
 * 如果节点符号已经在当前变量符号表中存在，输出错误信息：标识符重定义。并退出
 * 节点id和节点变量信息打包向变量符号表中插入
 */
void SemantVisitor::visit(ast::VarDecl &node)
{
  node.type().accept(*this);
  node.setVarType(std::make_unique<VarType>(false, &node.type().symType()));
  for (const auto &id : node.idList()) {
    const auto *tmp = context_.typetab_.probe(id);
    if (tmp != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier " + id);
      return;
    }
    tmp = context_.consttab_.probe(id);
    if (tmp != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier " + id);
      return;
    }
    const auto *tmp1 = context_.vartab_.probe(id);
    if (tmp1 != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier " + id);
      return;
    }
    context_.vartab_.insert(id, &node.varType());
  }
}

/**
 * 如果节点符号已经在当前子程序符号表或者变量符号表或者常量符号表中存在，输出错误信息：重复定义的过程。并退出
 * 对于每个节点参数，填充到形参列表中，参考这个列表进行初始化proc_type_
 * 向子程序符号表中插入本节点信息(包括id和data)
 * 在进行形式参数的插入前，进入新的作用域，遍历形参列表中的每个参数是否存在与变量符号表中，存在则输出错误：标识符重定义
 * 向变量符号表中插入每个形参信息(name和data)
 */
void SemantVisitor::visit(ast::ProcHead &node)
{
  if (
      context_.subprogtab_.probe(node.procId()) != nullptr || context_.vartab_.probe(node.procId()) != nullptr || context_.consttab_.probe(node.procId()) != nullptr
  ) {
    context_.genErrorMsg(node.location(), "dupilcated identify procedure" + node.procId());
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
  context_.subprogtab_.insert(
      node.procId(),
      &node.procType()
  );
  context_.pushFunc(node.procId());
  context_.enterScope();
  // 遍历 context_.formal_params_ 插入到 vartab_
  for (const auto &[varid, vartype] : node.procType().formalParams()) {
    if (context_.vartab_.probe(varid) != nullptr) {
      context_.genErrorMsg(node.location(), "duplicated identifier " + varid);
    } else {
      context_.vartab_.insert(varid, vartype);
    }
  }
}

/**
 * 顺序访问过程块的各个部分，按顺序访问过程块中的常量声明部分、类型声明部分、变量声明部分、子过程声明部分和语句部分
 * 访问全部完成后，从函数调用栈中弹出过程，退出当前作用域
 */
void SemantVisitor::visit(ast::ProcBlock &node)
{
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

/**
 * @brief 访问过程头部和过程块
 */
void SemantVisitor::visit(ast::ProcDecl &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}

/**
 * 如果节点符号已经在当前子程序符号表或者变量符号表或者常量符号表中存在，输出错误信息：重复定义的过程。并退出
 * 对于每个节点参数，填充到形参列表中，参考这个列表进行初始化func_type_
 * 向子程序符号表中插入本节点信息(包括id和data)
 * 在进行形式参数的插入前，进入新的作用域，遍历形参列表中的每个参数是否存在与变量符号表中，存在则输出错误：标识符重定义
 * 向变量符号表中插入每个形参信息(name和data)
 */
void SemantVisitor::visit(ast::FuncHead &node)
{
  if (
      context_.subprogtab_.probe(node.funcId()) != nullptr || context_.vartab_.probe(node.funcId()) != nullptr || context_.consttab_.probe(node.funcId()) != nullptr
  )
  {
    context_.genErrorMsg(node.location(), "dupilcated identify function " + node.funcId());
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
      context_.genErrorMsg(node.location(), "dupilcated identify " + varid);
    } else {
      context_.vartab_.insert(varid, vartype);
    }
  }
}

/**
 * 顺序访问函数块的各个部分，按顺序访问过程块中的常量声明部分、类型声明部分、变量声明部分、子过程声明部分和语句部分
 * 访问全部完成后，从函数调用栈中弹出函数，退出当前作用域
 */
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

/**
 * @brief 访问函数头部和函数块
 */
void SemantVisitor::visit(ast::FuncDecl &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}

/**
 * @brief 访问子程序声明
 */
void SemantVisitor::visit(ast::SubprogDeclPart &node)
{
  for (const auto &subprog_decl : node.subprogDecls()) {
    subprog_decl->accept(*this);
  }
}

/**
 * 如果条件表达式类型不为布尔类型，输出错误信息：这里希望得到布尔类型
 * 访问then和else模块
 */
void SemantVisitor::visit(ast::IfStmt &node)
{
  node.cond().accept(*this);
  if (node.hasThen()) {
    node.then().accept(*this);
  }

  if (node.hasElse()) {
    node.Else().accept(*this);
  }
}

/**
 * 访问case语句中的表达式
 * 访问caselist列表中各个情况分支
 */
void SemantVisitor::visit(ast::CaseStmt &node)
{
  node.expr().accept(*this);
  context_.case_stmt_type_ = &node.expr().type();
  for (const auto &eachcase : node.caseList()) {
    eachcase->stmt().accept(*this);
  }
}

/**
 * 访问情况分支中的每个常量
 * 如果情况分支中的常量类型和case语句类型不匹配并且不能转换为case语句类型，输出错误信息：情况分支类型不能匹配case表达式类型
 */
void SemantVisitor::visit(ast::CaseListElement &node)
{
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

/**
 * 如果循环条件表达式类型不为布尔类型，输出错误信息：希望得到布尔类型
 * 访问重复语句中的循环体部分
 */
void SemantVisitor::visit(ast::RepeatStmt &node)
{
  context_.inloop_ = true;
  node.cond().accept(*this);
  if (!context_.cmp_(node.cond().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    context_.genErrorMsg(node.location(), "boolean type expected.");
  }
  for (const auto &stmt : node.body()) {
    stmt->accept(*this);
  }
  context_.inloop_ = false;
}

/**
 * 如果循环条件表达式类型不为布尔类型，输出错误信息：希望得到布尔类型
 * 访问while语句body部分
 */
void SemantVisitor::visit(ast::WhileStmt &node)
{
  context_.inloop_ = true;
  node.cond().accept(*this);
  if (!context_.cmp_(node.cond().type(), SymType(BuiltInType(BasicType::BOOLEAN)))) {
    // 条件不是bool
    context_.genErrorMsg(node.location(), "boolean type expected.");
  }
  node.body().accept(*this);
  context_.inloop_ = false;
}

/**
 * 如果控制变量和初始值类型不匹配或者控制变量和结束值类型不匹配，输出错误信息：控制变量类型与初始值或结束值类型不匹配
 * 访问for语句body部分
 */
void SemantVisitor::visit(ast::ForStmt &node)
{
  context_.inloop_ = true;
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
  context_.inloop_ = false;
}

/**
 * 如果表达式左侧的可赋值属性为false，输出错误信息：希望得到的是变量标识符。并退出
 */
void SemantVisitor::visit(ast::AssignStmt &node)
{
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

  context_.genErrorMsg(node.location(), "type error");
}

/**
 * 如果遇到break语句时程序处于循环外，输出错误信息：预期之外的中断
 */
void SemantVisitor::visit(ast::BreakStmt &node)
{
  if (!context_.inloop_) {
    context_.genErrorMsg(node.location(), "Unexpected break.");
  }
}

/**
 * 如果在子程序符号表中找不到节点声明，输出错误信息：未定义的过程调用。并退出
 * 如果节点实参和声明形参个数不一致，输出错误信息：实参不匹配参数列表。并退出
 * 如果形参的属性是引用类型并且实参的属性是不可变的，输出错误信息：实参不匹配。并退出
 * 如果实参和形参类型不匹配并且实参类型不能转换为形参类型，输出错误消息：实参不匹配。并退出
 */
void SemantVisitor::visit(ast::ProcCallStmt &node)
{
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
      context_.genErrorMsg(node.location(), "Variable identifier expected.");
      return;
    }
    if (!context_.cmp_(actuals_expected[i].second->symType(), node.actuals()[i]->type()) && !TypeComparator::cast(node.actuals()[i]->type(), actuals_expected[i].second->symType())) {
      context_.genErrorMsg(node.location(), "actual list do not match.");
      return;
    }
  }
}

/**
 * 对于参数列表中的每个实参，如果实参的属性为不可改变，输出错误信息：希望得到可以赋值的实参
 * 
 * @attention 读取的变量必须是 assignable
 */
void SemantVisitor::visit(ast::ReadStmt &node)
{
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
    if (!actual->isChangeable()) {
      context_.genErrorMsg(node.location(), "Assignable actual expected.");
    }
  }
}

/**
 * @brief 访问写语句中的每个实参
 */
void SemantVisitor::visit(ast::WriteStmt &node)
{
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
}

/**
 * 如果读语句中的实参存在属性为不可被修改，输出错误信息：希望得到可以赋值的实参
 */
void SemantVisitor::visit(ast::ReadlnStmt &node)
{
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

/**
 * @brief 访问写语句中的每个实参 
 */
void SemantVisitor::visit(ast::WritelnStmt &node)
{
  for (const auto &actual : node.actuals()) {
    actual->accept(*this);
  }
}

/**
 * 如果exit没有返回值并且不包括实参，说明合法，退出
 * 如果exit有一个实参并且没有返回值，如果实参与返回值类型不相同并且实参类型不能转换为返回值类型，输出错误信息：返回值类型不匹配。并退出
 * 以上都不满足，输出错误信息：非法使用exit
 */
void SemantVisitor::visit(ast::ExitStmt &node)
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

/**
 * @brief 访问各个复合语句
 */
void SemantVisitor::visit(ast::CompoundStmt &node)
{
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
}

/**
 * @brief 访问各个语句
 */
void SemantVisitor::visit(ast::StmtPart &node)
{
  for (const auto &stmt : node.stmts()) {
    stmt->accept(*this);
  }
}

/**
 * @brief 顺序访问程序中各个part
 */
void SemantVisitor::visit(ast::ProgramBlock &node)
{
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

// do nothing
void SemantVisitor::visit([[maybe_unused]] ast::ProgramHead &node) {}

/**
 * @brief 访问程序头和程序块
 */
void SemantVisitor::visit(ast::Program &node)
{
  node.head().accept(*this);
  node.block().accept(*this);
}

}  // namespace pascc::semant
