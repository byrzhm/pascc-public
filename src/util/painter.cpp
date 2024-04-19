#include "util/painter.hpp"

namespace pascc::util {

void PaintVisitor::visit([[maybe_unused]] ast::Block &node)
{
  throw std::runtime_error("Block should not be visited");
}

void PaintVisitor::visit(ast::Number &node)
{
  auto number = reinterpret_cast<uint64_t>(&node);
  switch (node.type().builtInType().type()) {
    case BasicType::INTEGER:
      out_ << "   " << number << " [label=\"Number: " << std::get<int>(node.value()) << "\"]\n";
      break;
    case BasicType::REAL:
      out_ << "   " << number << " [label=\"Number: " << std::get<double>(node.value()) << "\"]\n";
      break;
    default:
      throw std::runtime_error("Unknown number type");
  }
}

void PaintVisitor::visit(ast::Constant &node)
{
  auto constant             = reinterpret_cast<uint64_t>(&node);
  const auto &constant_type = node.type();
  if (constant_type == "reference" || constant_type == "string") {
    out_ << "   " << constant << " [label=\"Constant: "
         << std::get<std::string>(node.value()) << "\"]\n";
  } else if (constant_type == "integer") {
    out_ << "   " << constant << " [label=\"Constant: "
         << std::get<int>(node.value()) << "\"]\n";
  } else if (constant_type == "real") {
    out_ << "   " << constant << " [label=\"Constant: "
         << std::get<double>(node.value()) << "\"]\n";
  } else if (constant_type == "char") {
    out_ << "   " << constant << " [label=\"Constant: "
         << std::get<char>(node.value()) << "\"]\n";
  } else {
    throw std::runtime_error("Unknown constant type");
  }
}

void PaintVisitor::visit(ast::StringLiteral &node)
{
  auto string_literal = reinterpret_cast<uint64_t>(&node);
  out_ << "   " << string_literal << " [label=\"StringLiteral: "
       << node.value() << "\"]\n";
}

/// expression
void PaintVisitor::visit(ast::BoolExpr &node)
{
  auto bool_expr = reinterpret_cast<uint64_t>(&node);
  out_ << "   " << bool_expr << " [label=\"BoolExpr\"]\n";
  out_ << "   " << bool_expr << " -> " << reinterpret_cast<uint64_t>(&node.expr()) << "\n";
  node.expr().accept(*this);
}

void PaintVisitor::visit(ast::UnsignedConstant &node)
{
  auto unsigned_constant = reinterpret_cast<uint64_t>(&node);
  const auto &type       = node.type();
  switch (type.builtInType().type()) {
    case BasicType::REAL:
      out_ << "   " << unsigned_constant << " [label=\"UnsignedConstant: "
           << std::get<double>(node.value()) << "\"]\n";
      break;

    case BasicType::INTEGER:
      out_ << "   " << unsigned_constant << " [label=\"UnsignedConstant: "
           << std::get<int>(node.value()) << "\"]\n";
      break;

    case BasicType::CHAR:
      out_ << "   " << unsigned_constant << " [label=\"UnsignedConstant: "
           << std::get<char>(node.value()) << "\"]\n";
      break;

    case BasicType::BOOLEAN:
      out_ << "   " << unsigned_constant << " [label=\"UnsignedConstant: "
           << (std::get<bool>(node.value()) ? "true" : "false") << "\"]\n";
      break;

    default:
      throw std::runtime_error("Unknown unsigned constant type");
  }
}

void PaintVisitor::visit(ast::BinaryExpr &node)
{
  auto binary_expr = reinterpret_cast<uint64_t>(&node);
  auto lhs         = reinterpret_cast<uint64_t>(&node.lhs());
  auto rhs         = reinterpret_cast<uint64_t>(&node.rhs());
  auto op          = node.op();

  out_ << "   " << binary_expr << " [label=\"BinaryExpr: " << op << "\"]\n";
  out_ << "   " << binary_expr << " -> " << lhs << "\n";
  out_ << "   " << binary_expr << " -> " << rhs << "\n";

  node.lhs().accept(*this);
  node.rhs().accept(*this);
}

void PaintVisitor::visit(ast::UnaryExpr &node)
{
  auto unary_expr = reinterpret_cast<uint64_t>(&node);
  auto expr       = reinterpret_cast<uint64_t>(&node.expr());
  auto op         = node.op();

  out_ << "   " << unary_expr << " [label=\"UnaryExpr: " << op << "\"]\n";
  out_ << "   " << unary_expr << " -> " << expr << "\n";

  node.expr().accept(*this);
}

void PaintVisitor::visit(ast::FuncCall &node)
{
  auto func_call = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << func_call << " [label=\"FuncCall: " << node.funcid() << "\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << func_call << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::VarId &node)
{
  auto assignable_id = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << assignable_id << " [label=\"AssignableId: " << node.id() << "\"]\n";
}

void PaintVisitor::visit(ast::IndexedVar &node)
{
  auto indexed_var = reinterpret_cast<uint64_t>(&node);
  auto assignable  = reinterpret_cast<uint64_t>(&node.varAccess());

  out_ << "   " << indexed_var << " [label=\"IndexedVar\"]\n";
  out_ << "   " << indexed_var << " -> " << assignable << "\n";
  node.varAccess().accept(*this);

  for (const auto &index : node.indices()) {
    auto index_ptr = reinterpret_cast<uint64_t>(index.get());
    out_ << "   " << indexed_var << " -> " << index_ptr << "\n";
    index->accept(*this);
  }
}

void PaintVisitor::visit(ast::FieldDesignator &node)
{
  auto field_designator = reinterpret_cast<uint64_t>(&node);
  auto assignable       = reinterpret_cast<uint64_t>(&node.varAccess());

  out_ << "   " << field_designator << " [label=\"FieldDesignator: " << node.field() << "\"]\n";
  out_ << "   " << field_designator << " -> " << assignable << "\n";

  node.varAccess().accept(*this);
}


/// const declaration
void PaintVisitor::visit(ast::ConstDecl &node)
{
  auto const_decl = reinterpret_cast<uint64_t>(&node);
  auto constant   = reinterpret_cast<uint64_t>(&node.constant());

  out_ << "   " << const_decl << " [label=\"ConstDecl"
       << node.constId() << "\"]\n";
  out_ << "   " << const_decl << " -> " << constant << "\n";

  node.constant().accept(*this);
}

void PaintVisitor::visit(ast::ConstDeclPart &node)
{
  auto const_decl_part = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << const_decl_part << " [label=\"ConstDeclPart\"]\n";

  for (const auto &const_decl : node.constDecls()) {
    auto const_decl_ptr = reinterpret_cast<uint64_t>(const_decl.get());
    out_ << "   " << const_decl_part << " -> " << const_decl_ptr << "\n";
    const_decl->accept(*this);
  }
}


/// type declaration
void PaintVisitor::visit(ast::TypeId &node)
{
  auto type_id = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << type_id << " [label=\"TypeId: " << node.id() << "\"]\n";
}

void PaintVisitor::visit(ast::Period &node)
{
  auto period = reinterpret_cast<uint64_t>(&node);
  auto low    = reinterpret_cast<uint64_t>(&node.low());
  auto high   = reinterpret_cast<uint64_t>(&node.high());

  out_ << "   " << period << " [label=\"Period\"]\n";
  out_ << "   " << period << " -> " << low << "\n";
  out_ << "   " << period << " -> " << high << "\n";

  node.low().accept(*this);
  node.high().accept(*this);
}

void PaintVisitor::visit(ast::ArrayType &node)
{
  auto array_type = reinterpret_cast<uint64_t>(&node);
  auto type       = reinterpret_cast<uint64_t>(&node.ofType());

  out_ << "   " << array_type << " [label=\"ArrayType\"]\n";
  out_ << "   " << array_type << " -> " << type << "\n";

  node.ofType().accept(*this);

  for (const auto &period : node.periods()) {
    auto period_ptr = reinterpret_cast<uint64_t>(period.get());
    out_ << "   " << array_type << " -> " << period_ptr << "\n";
    period->accept(*this);
  }
}

void PaintVisitor::visit(ast::RecordType &node)
{
  auto record_type = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << record_type << " [label=\"RecordType\"]\n";

  for (const auto &field : node.fields()) {
    auto field_ptr = reinterpret_cast<uint64_t>(field.get());
    out_ << "   " << record_type << " -> " << field_ptr << "\n";
    field->accept(*this);
  }
}

void PaintVisitor::visit(ast::TypeDecl &node)
{
  auto type_decl    = reinterpret_cast<uint64_t>(&node);
  auto type_denoter = reinterpret_cast<uint64_t>(&node.typeDenoter());

  out_ << "   " << type_decl << " [label=\"TypeDecl: " << node.typeId() << "\"]\n";

  out_ << "   " << type_decl << " -> " << type_denoter << "\n";
  node.typeDenoter().accept(*this);
}

void PaintVisitor::visit(ast::TypeDeclPart &node)
{
  auto type_decl_part = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << type_decl_part << " [label=\"TypeDeclPart\"]\n";

  for (const auto &type_decl : node.typeDecls()) {
    auto type_decl_ptr = reinterpret_cast<uint64_t>(type_decl.get());
    out_ << "   " << type_decl_part << " -> " << type_decl_ptr << "\n";
    type_decl->accept(*this);
  }
}


/// var declaration
void PaintVisitor::visit(ast::VarDeclPart &node)
{
  auto var_decl_part = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << var_decl_part << " [label=\"VarDeclPart\"]\n";

  for (const auto &var_decl : node.varDecls()) {
    auto var_decl_ptr = reinterpret_cast<uint64_t>(var_decl.get());
    out_ << "   " << var_decl_part << " -> " << var_decl_ptr << "\n";
    var_decl->accept(*this);
  }
}

void PaintVisitor::visit(ast::ValueParamSpec &node)
{
  auto value_param_spec = reinterpret_cast<uint64_t>(&node);
  auto type             = reinterpret_cast<uint64_t>(&node.type());

  out_ << "   " << value_param_spec << " [label=\"ValueParamSpec: ";
  for (const auto &id : node.idList()) {
    out_ << id;
    if (&id != &node.idList().back()) {
      out_ << ", ";
    }
  }
  out_ << "\"]\n";

  out_ << "   " << value_param_spec << " -> " << type << "\n";
  node.type().accept(*this);
}

void PaintVisitor::visit(ast::VarParamSpec &node)
{
  auto var_param_spec = reinterpret_cast<uint64_t>(&node);
  auto type           = reinterpret_cast<uint64_t>(&node.type());

  out_ << "   " << var_param_spec << " [label=\"VarParamSpec: ";
  for (const auto &id : node.idList()) {
    out_ << id;
    if (&id != &node.idList().back()) {
      out_ << ", ";
    }
  }
  out_ << "\"]\n";

  out_ << "   " << var_param_spec << " -> " << type << "\n";
  node.type().accept(*this);
}

void PaintVisitor::visit(ast::VarDecl &node)
{
  auto var_decl = reinterpret_cast<uint64_t>(&node);
  auto type     = reinterpret_cast<uint64_t>(&node.type());

  out_ << "   " << var_decl << " [label=\"VarDecl: ";
  for (const auto &id : node.idList()) {
    out_ << id;
    if (&id != &node.idList().back()) {
      out_ << ", ";
    }
  }
  out_ << "\"]\n";

  out_ << "   " << var_decl << " -> " << type << "\n";
  node.type().accept(*this);
}


/// subprogram declaration
void PaintVisitor::visit(ast::ProcHead &node)
{
  auto proc_head = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << proc_head << " [label=\"ProcHead: " << node.procId() << "\"]\n";

  for (const auto &formal_param : node.formalParams()) {
    auto formal_param_ptr = reinterpret_cast<uint64_t>(formal_param.get());
    out_ << "   " << proc_head << " -> " << formal_param_ptr << "\n";
    formal_param->accept(*this);
  }
}

void PaintVisitor::visit(ast::ProcBlock &node)
{
  auto proc_block = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << proc_block << " [label=\"ProcBlock\"]\n";

  if (node.hasConstDeclPart()) {
    auto const_decl_part = reinterpret_cast<uint64_t>(&node.constDeclPart());
    out_ << "   " << proc_block << " -> " << const_decl_part << "\n";
    node.constDeclPart().accept(*this);
  }

  if (node.hasTypeDeclPart()) {
    auto type_decl_part = reinterpret_cast<uint64_t>(&node.typeDeclPart());
    out_ << "   " << proc_block << " -> " << type_decl_part << "\n";
    node.typeDeclPart().accept(*this);
  }

  if (node.hasVarDeclPart()) {
    auto var_decl_part = reinterpret_cast<uint64_t>(&node.varDeclPart());
    out_ << "   " << proc_block << " -> " << var_decl_part << "\n";
    node.varDeclPart().accept(*this);
  }

  if (!node.hasStmtPart()) {
    throw std::runtime_error("ProcBlock has no StmtPart");
  }

  auto stmt_part = reinterpret_cast<uint64_t>(&node.stmtPart());
  out_ << "   " << proc_block << " -> " << stmt_part << "\n";
  node.stmtPart().accept(*this);
}

void PaintVisitor::visit(ast::ProcDecl &node)
{
  auto proc_decl  = reinterpret_cast<uint64_t>(&node);
  auto proc_head  = reinterpret_cast<uint64_t>(&node.head());
  auto proc_block = reinterpret_cast<uint64_t>(&node.block());

  out_ << "   " << proc_decl << " [label=\"ProcDecl\"]\n";
  out_ << "   " << proc_decl << " -> " << proc_head << "\n";
  out_ << "   " << proc_decl << " -> " << proc_block << "\n";

  node.head().accept(*this);
  node.block().accept(*this);
}

void PaintVisitor::visit(ast::FuncHead &node)
{
  auto func_head = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << func_head << " [label=\"FuncHead: " << node.funcId() << "\"]\n";

  for (const auto &formal_param : node.formalParams()) {
    auto formal_param_ptr = reinterpret_cast<uint64_t>(formal_param.get());
    out_ << "   " << func_head << " -> " << formal_param_ptr << "\n";
    formal_param->accept(*this);
  }

  auto type = reinterpret_cast<uint64_t>(&node.returnType());
  out_ << "   " << func_head << " -> " << type << "\n";
  node.returnType().accept(*this);
}

void PaintVisitor::visit(ast::FuncBlock &node)
{
  auto func_block = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << func_block << " [label=\"FuncBlock\"]\n";

  if (node.hasConstDeclPart()) {
    auto const_decl_part = reinterpret_cast<uint64_t>(&node.constDeclPart());
    out_ << "   " << func_block << " -> " << const_decl_part << "\n";
    node.constDeclPart().accept(*this);
  }

  if (node.hasTypeDeclPart()) {
    auto type_decl_part = reinterpret_cast<uint64_t>(&node.typeDeclPart());
    out_ << "   " << func_block << " -> " << type_decl_part << "\n";
    node.typeDeclPart().accept(*this);
  }

  if (node.hasVarDeclPart()) {
    auto var_decl_part = reinterpret_cast<uint64_t>(&node.varDeclPart());
    out_ << "   " << func_block << " -> " << var_decl_part << "\n";
    node.varDeclPart().accept(*this);
  }

  if (!node.hasStmtPart()) {
    throw std::runtime_error("FuncBlock has no StmtPart");
  }

  auto stmt_part = reinterpret_cast<uint64_t>(&node.stmtPart());
  out_ << "   " << func_block << " -> " << stmt_part << "\n";
  node.stmtPart().accept(*this);
}

void PaintVisitor::visit(ast::FuncDecl &node)
{
  auto func_decl  = reinterpret_cast<uint64_t>(&node);
  auto func_head  = reinterpret_cast<uint64_t>(&node.head());
  auto func_block = reinterpret_cast<uint64_t>(&node.block());

  out_ << "   " << func_decl << " [label=\"FuncDecl\"]\n";
  out_ << "   " << func_decl << " -> " << func_head << "\n";
  out_ << "   " << func_decl << " -> " << func_block << "\n";

  node.head().accept(*this);
  node.block().accept(*this);
}

void PaintVisitor::visit(ast::SubprogDeclPart &node)
{
  auto subprog_decl_part = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << subprog_decl_part << " [label=\"SubprogDeclPart\"]\n";

  for (const auto &subprog_decl : node.subprogDecls()) {
    auto subprog_decl_ptr = reinterpret_cast<uint64_t>(subprog_decl.get());
    out_ << "   " << subprog_decl_part << " -> " << subprog_decl_ptr << "\n";
    subprog_decl->accept(*this);
  }
}


/// statement
// conditional statement
void PaintVisitor::visit(ast::IfStmt &node)
{
  auto if_stmt = reinterpret_cast<uint64_t>(&node);
  auto cond    = reinterpret_cast<uint64_t>(&node.cond());
  auto then    = reinterpret_cast<uint64_t>(&node.then());

  out_ << "   " << if_stmt << " [label=\"IfStmt\"]\n";
  out_ << "   " << if_stmt << " -> " << cond << "\n";
  out_ << "   " << if_stmt << " -> " << then << "\n";

  node.cond().accept(*this);
  node.then().accept(*this);

  if (node.hasElse()) {
    auto else_stmt = reinterpret_cast<uint64_t>(&node.Else());
    out_ << "   " << if_stmt << " -> " << else_stmt << "\n";
    node.Else().accept(*this);
  }
}

void PaintVisitor::visit(ast::CaseStmt &node)
{
  auto case_stmt = reinterpret_cast<uint64_t>(&node);
  auto expr      = reinterpret_cast<uint64_t>(&node.expr());

  out_ << "   " << case_stmt << " [label=\"CaseStmt\"]\n";
  out_ << "   " << case_stmt << " -> " << expr << "\n";

  node.expr().accept(*this);

  for (const auto &case_list_element : node.caseList()) {
    auto case_list_element_ptr = reinterpret_cast<uint64_t>(case_list_element.get());
    out_ << "   " << case_stmt << " -> " << case_list_element_ptr << "\n";
    case_list_element->accept(*this);
  }
}

void PaintVisitor::visit(ast::CaseListElement &node)
{
  auto case_list_element = reinterpret_cast<uint64_t>(&node);
  auto stmt              = reinterpret_cast<uint64_t>(&node.stmt());

  out_ << "   " << case_list_element << " [label=\"CaseListElement\"]\n";
  out_ << "   " << case_list_element << " -> " << stmt << "\n";

  node.stmt().accept(*this);

  for (const auto &const_value : node.constants()) {
    auto const_value_ptr = reinterpret_cast<uint64_t>(const_value.get());
    out_ << "   " << case_list_element << " -> " << const_value_ptr << "\n";
    const_value->accept(*this);
  }
}

// repetitive statement
void PaintVisitor::visit(ast::RepeatStmt &node)
{
  auto repeat_stmt = reinterpret_cast<uint64_t>(&node);
  auto cond        = reinterpret_cast<uint64_t>(&node.cond());

  out_ << "   " << repeat_stmt << " [label=\"RepeatStmt\"]\n";
  out_ << "   " << repeat_stmt << " -> " << cond << "\n";

  node.cond().accept(*this);

  for (const auto &stmt : node.body()) {
    auto stmt_ptr = reinterpret_cast<uint64_t>(stmt.get());
    out_ << "   " << repeat_stmt << " -> " << stmt_ptr << "\n";
    stmt->accept(*this);
  }
}

void PaintVisitor::visit(ast::WhileStmt &node)
{
  auto while_stmt = reinterpret_cast<uint64_t>(&node);
  auto cond       = reinterpret_cast<uint64_t>(&node.cond());
  auto stmt       = reinterpret_cast<uint64_t>(&node.body());

  out_ << "   " << while_stmt << " [label=\"WhileStmt\"]\n";
  out_ << "   " << while_stmt << " -> " << cond << "\n";
  out_ << "   " << while_stmt << " -> " << stmt << "\n";

  node.cond().accept(*this);
  node.body().accept(*this);
}

void PaintVisitor::visit(ast::ForStmt &node)
{
  auto for_stmt = reinterpret_cast<uint64_t>(&node);
  auto var      = reinterpret_cast<uint64_t>(&node.ctrlVar());
  auto init     = reinterpret_cast<uint64_t>(&node.initVal());
  auto end      = reinterpret_cast<uint64_t>(&node.endVal());
  auto stmt     = reinterpret_cast<uint64_t>(&node.body());

  out_ << "   " << for_stmt << " [label=\"ForStmt\"]\n";
  out_ << "   " << for_stmt << " -> " << var << "\n";
  out_ << "   " << for_stmt << " -> " << init << "\n";
  out_ << "   " << for_stmt << " -> " << end << "\n";
  out_ << "   " << for_stmt << " -> " << stmt << "\n";

  node.ctrlVar().accept(*this);
  node.initVal().accept(*this);
  node.endVal().accept(*this);
  node.body().accept(*this);
}

// simple statement
void PaintVisitor::visit(ast::AssignStmt &node)
{
  auto assign_stmt = reinterpret_cast<uint64_t>(&node);
  auto lhs         = reinterpret_cast<uint64_t>(&node.lhs());
  auto rhs         = reinterpret_cast<uint64_t>(&node.rhs());

  out_ << "   " << assign_stmt << " [label=\"AssignStmt\"]\n";
  out_ << "   " << assign_stmt << " -> " << lhs << "\n";
  out_ << "   " << assign_stmt << " -> " << rhs << "\n";

  node.lhs().accept(*this);
  node.rhs().accept(*this);
}

void PaintVisitor::visit(ast::ProcCallStmt &node)
{
  auto proc_call_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << proc_call_stmt << " [label=\"ProcCallStmt: " << node.procId() << "\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << proc_call_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::ReadStmt &node)
{
  auto read_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << read_stmt << " [label=\"ReadStmt\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << read_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::WriteStmt &node)
{
  auto write_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << write_stmt << " [label=\"WriteStmt\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << write_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::ReadlnStmt &node)
{
  auto readln_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << readln_stmt << " [label=\"ReadlnStmt\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << readln_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::WritelnStmt &node)
{
  auto writeln_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << writeln_stmt << " [label=\"WritelnStmt\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << writeln_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::ExitStmt &node)
{
  auto exit_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << exit_stmt << " [label=\"ExitStmt\"]\n";

  for (const auto &arg : node.actuals()) {
    auto arg_ptr = reinterpret_cast<uint64_t>(arg.get());
    out_ << "   " << exit_stmt << " -> " << arg_ptr << "\n";
    arg->accept(*this);
  }
}

void PaintVisitor::visit(ast::CompoundStmt &node)
{
  auto compound_stmt = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << compound_stmt << " [label=\"CompoundStmt\"]\n";

  for (const auto &stmt : node.stmts()) {
    auto stmt_ptr = reinterpret_cast<uint64_t>(stmt.get());
    out_ << "   " << compound_stmt << " -> " << stmt_ptr << "\n";
    stmt->accept(*this);
  }
}

void PaintVisitor::visit(ast::StmtPart &node)
{
  auto stmt_part = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << stmt_part << " [label=\"StmtPart\"]\n";

  for (const auto &stmt : node.stmts()) {
    auto stmt_ptr = reinterpret_cast<uint64_t>(stmt.get());
    out_ << "   " << stmt_part << " -> " << stmt_ptr << "\n";
    stmt->accept(*this);
  }
}

/// program
void PaintVisitor::visit(ast::ProgramBlock &node)
{
  auto program_block = reinterpret_cast<uint64_t>(&node);

  out_ << "   " << program_block << " [label=\"ProgramBlock\"]\n";

  if (node.hasConstDeclPart()) {
    auto const_decl_part = reinterpret_cast<uint64_t>(&node.constDeclPart());
    out_ << "   " << program_block << " -> " << const_decl_part << "\n";
    node.constDeclPart().accept(*this);
  }

  if (node.hasTypeDeclPart()) {
    auto type_decl_part = reinterpret_cast<uint64_t>(&node.typeDeclPart());
    out_ << "   " << program_block << " -> " << type_decl_part << "\n";
    node.typeDeclPart().accept(*this);
  }

  if (node.hasVarDeclPart()) {
    auto var_decl_part = reinterpret_cast<uint64_t>(&node.varDeclPart());
    out_ << "   " << program_block << " -> " << var_decl_part << "\n";
    node.varDeclPart().accept(*this);
  }

  if (node.hasSubprogDeclPart()) {
    auto subprog_decl_part = reinterpret_cast<uint64_t>(&node.subprogDeclPart());
    out_ << "   " << program_block << " -> " << subprog_decl_part << "\n";
    node.subprogDeclPart().accept(*this);
  }

  if (!node.hasStmtPart()) {
    throw std::runtime_error("ProgramBlock has no StmtPart");
  }

  auto stmt_part = reinterpret_cast<uint64_t>(&node.stmtPart());
  out_ << "   " << program_block << " -> " << stmt_part << "\n";
  node.stmtPart().accept(*this);
}

void PaintVisitor::visit(ast::ProgramHead &node)
{
  auto program_head   = reinterpret_cast<uint64_t>(&node);
  const auto &id_list = node.idList();

  out_ << "   " << program_head << " [label=\"ProgramHead: "
       << node.programName() << "(";

  for (size_t i = 0; i < id_list.size(); ++i) {
    out_ << id_list[i];
    if (i != id_list.size() - 1) {
      out_ << ", ";
    }
  }

  out_ << ")\"]\n";
}

void PaintVisitor::visit(ast::Program &node)
{
  auto program       = reinterpret_cast<uint64_t>(&node);
  auto program_head  = reinterpret_cast<uint64_t>(&node.head());
  auto program_block = reinterpret_cast<uint64_t>(&node.block());

  out_ << "digraph G {\n";
  out_ << "   " << program << " [label=\"Program\"]\n";
  out_ << "   " << program << " -> " << program_head << "\n";
  out_ << "   " << program << " -> " << program_block << "\n";

  node.head().accept(*this);
  node.block().accept(*this);

  out_ << "}\n";
}

}  // namespace pascc::util