%skeleton "lalr1.cc"
%require "3.8"
%header

%define api.token.raw
%define api.token.constructor
%define api.token.prefix {TOK_}
%define api.value.type variant
%define api.parser.class {Parser}
%define api.namespace {pascc::parse}

%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

%verbose

%locations

%code requires {
  #include "ast/ast.hpp"
  namespace pascc::parse {
    class ParserDriver;
  } // namespace pascc::parse
  using namespace pascc::ast;
}

%code{
  #include "util/log.hpp"
  #include "parse/parser_driver.hpp"
  #include <sstream>
  using namespace pascc::parse;
}

%param { ParserDriver& drv }

%token EOF 0

%token
  AND       "and"
  ARRAY     "array"
  BEGIN     "begin"
  CASE      "case"
  CONST     "const"
  IDIV      "div"
  DO        "do"
  DOWNTO    "downto"
  ELSE      "else"
  END       "end"
  FILE      "file"
  FOR       "for"
  FUNCTION  "function"
  GOTO      "goto"
  IF        "if"
  IN        "in"
  LABEL     "label"
  MOD       "mod"
  NIL       "nil"
  NOT       "not"
  OF        "of"
  OR        "or"
  PACKED    "packed"
  PROCEDURE "procedure"
  PROGRAM   "program"
  RECORD    "record"
  REPEAT    "repeat"
  SET       "set"
  THEN      "then"
  TO        "to"
  TYPE      "type"
  UNTIL     "until"
  VAR       "var"
  WHILE     "while"
  WITH      "with"
  BREAK     "break"

  READ      "read"
  READLN    "readln"
  WRITE     "write"
  WRITELN   "writeln"
  TRUE      "true"
  FALSE     "false"
  EXIT      "exit"

  PLUS      "+"
  MINUS     "-"
  MUL       "*"
  FDIV      "/"
  EQ        "="
  GT        ">"
  LT        "<"
  NE        "<>"
  LE        "<="
  GE        ">="
  ASSIGN    ":="

  LSB       "["
  RSB       "]"
  LPAREN    "("
  RPAREN    ")"
  PERIOD    "."
  COMMA     ","
  COLON     ":"
  SEMICOLON ";"
  RANGE     ".."
;

%token <std::string> ID "identifier"
%token <std::string> STR_LIT "string_literal"
%token <int> INT_NUM "integer_number"
%token <double> REAL_NUM "float_number"
%token <char> CHAR "character"

/* Program */
%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<ProgramHead>> program_head
%nterm <std::unique_ptr<ProgramBlock>> program_block
%nterm <std::vector<std::string>> id_list

/* Block */
%nterm <std::unique_ptr<Block>> block

/* const decl */
%nterm <std::unique_ptr<ConstDeclPart>> constant_declaration_part
%nterm <std::vector<std::unique_ptr<ConstDecl>>> constant_declarations
%nterm <std::unique_ptr<Constant>> constant
%nterm <std::unique_ptr<StringLiteral>> string_literal
%nterm <std::unique_ptr<Number>> num

/* type decl */
%nterm <std::unique_ptr<TypeDeclPart>> type_declaration_part
%nterm <std::vector<std::unique_ptr<TypeDecl>>> type_declarations
%nterm <std::unique_ptr<TypeDenoter>> type_denoter
%nterm <std::unique_ptr<TypeId>> type_identifier

/* var decl */
%nterm <std::unique_ptr<VarDeclPart>> variable_declaration_part
%nterm <std::vector<std::unique_ptr<VarDecl>>> variable_declarations
%nterm <std::vector<std::unique_ptr<VarDecl>>> field_list
%nterm <std::vector<std::unique_ptr<Period>>> periods
%nterm <std::unique_ptr<Period>> period

/* subprogram */
%nterm <std::unique_ptr<SubprogDeclPart>> subprogram_declaration_part
%nterm <std::vector<std::unique_ptr<SubprogDecl>>> subprogram_declarations
%nterm <std::unique_ptr<SubprogDecl>> subprogram_declaration
%nterm <std::unique_ptr<ProcDecl>> procedure_declaration
%nterm <std::unique_ptr<ProcHead>> procedure_head
%nterm <std::vector<std::unique_ptr<FormalParam>>> formal_parameter_list
%nterm <std::unique_ptr<FormalParam>> formal_parameter
%nterm <std::unique_ptr<ValueParamSpec>> value_parameter_specification
%nterm <std::unique_ptr<VarParamSpec>> variable_parameter_specification
%nterm <std::unique_ptr<ProcBlock>> procedure_block
%nterm <std::unique_ptr<FuncDecl>> function_declaration
%nterm <std::unique_ptr<FuncHead>> function_head
%nterm <std::unique_ptr<FuncBlock>> function_block

/* statement */
%nterm <std::unique_ptr<StmtPart>> statement_part
%nterm <std::unique_ptr<CompoundStmt>> compound_statement
%nterm <std::vector<std::unique_ptr<Stmt>>> statement_list
%nterm <std::unique_ptr<Stmt>> statement
%nterm <std::unique_ptr<Stmt>> simple_statement
%nterm <std::unique_ptr<Stmt>> empty_statement
%nterm <std::unique_ptr<Stmt>> assignment_statement
%nterm <std::unique_ptr<Stmt>> break_statement
%nterm <std::unique_ptr<Expr>> var_access
%nterm <std::unique_ptr<Expr>> indexed_variable
%nterm <std::unique_ptr<Expr>> field_designator
%nterm <std::unique_ptr<ProcCallStmt>>procedure_call_statement
%nterm <std::unique_ptr<WriteStmt>> write_statement
%nterm <std::unique_ptr<WritelnStmt>> writeln_statement
%nterm <std::unique_ptr<ReadStmt>> read_statement
%nterm <std::unique_ptr<ReadlnStmt>> readln_statement
%nterm <std::unique_ptr<ExitStmt>> exit_statement
%nterm <std::vector<std::unique_ptr<Expr>>> var_access_list
%nterm <std::unique_ptr<Stmt>> structured_statement
%nterm <std::unique_ptr<Stmt>> conditional_statement
%nterm <std::unique_ptr<IfStmt>> if_statement
%nterm <std::unique_ptr<BoolExpr>> bool_expr
%nterm <std::unique_ptr<Stmt>> else_part
%nterm <std::unique_ptr<CaseStmt>> case_statement
%nterm <std::vector<std::unique_ptr<CaseListElement>>> case_list_elements
%nterm <std::unique_ptr<CaseListElement>> case_list_element
%nterm <std::vector<std::unique_ptr<Constant>>> case_constant_list
%nterm <std::unique_ptr<Stmt>> repetitive_statement
%nterm <std::unique_ptr<RepeatStmt>> repeat_statement
%nterm <std::unique_ptr<WhileStmt>> while_statement
%nterm <std::unique_ptr<ForStmt>> for_statement
%nterm <bool> updown
%nterm <std::vector<std::unique_ptr<Expr>>> expr_list
%nterm <std::unique_ptr<Expr>> expr
%nterm <std::unique_ptr<Expr>> string_expr
%nterm <std::unique_ptr<Expr>> simple_expr
%nterm <std::unique_ptr<Expr>> term
%nterm <std::unique_ptr<Expr>> factor
%nterm <std::unique_ptr<Expr>> signed_factor
%nterm <std::unique_ptr<FuncCall>> function_designator
%nterm <std::unique_ptr<UnsignedConstant>> unsigned_constant
%nterm <BinOp> relop
%nterm <BinOp> addop
%nterm <BinOp> mulop

%start program

%%

program: 
  program_head program_block {
    // std::make_unique<T>(...) 将会生成 std::unique_ptr<T>, 其中 T 会使用 ... 提供的参数进行构造 
    $$ = std::make_unique<Program>(std::move($1), std::move($2));
    // std::cout << @1.begin.line << ", " << @1.begin.column << ", "
    //           << @1.end.line << ", " << @1.end.column << '\n';
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
    drv.program_ = std::move($$);
  }
  ;

program_head:
  PROGRAM ID LPAREN id_list RPAREN SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @6.end;
  }
  | PROGRAM ID LPAREN RPAREN SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @5.end;
  }
  | PROGRAM ID SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

program_block:
  block PERIOD {
    $$ = std::make_unique<ProgramBlock>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

block:
  constant_declaration_part type_declaration_part variable_declaration_part
   subprogram_declaration_part statement_part
  {
    $$ = std::make_unique<Block>(
      std::move($1),
      std::move($2),
      std::move($3),
      std::move($4),
      std::move($5)
    );
    $$->location().begin = @1.begin;
    $$->location().end = @5.end;
  }
  ;

id_list:
  id_list COMMA ID {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | ID {
    $$.emplace_back(std::move($1));
  }
  ;

constant_declaration_part:
  %empty {
    $$ = nullptr;
  }
  | CONST constant_declarations SEMICOLON {
    $$ = std::make_unique<ConstDeclPart>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

constant_declarations:
  constant_declarations SEMICOLON ID EQ constant {
    $$.swap($1);
    $$.emplace_back(std::make_unique<ConstDecl>(std::move($3), std::move($5)));
  }
  | ID EQ constant {
    $$.emplace_back(std::make_unique<ConstDecl>(std::move($1), std::move($3)));
  }
  ;
                       
constant:
  PLUS ID {
    $$ = std::make_unique<Constant>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | MINUS ID {
    $$ = std::make_unique<Constant>(std::move($2), -1);
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | ID {
    $$ = std::make_unique<Constant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | num {
    $$ = std::make_unique<Constant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | PLUS num {
    $$ = std::make_unique<Constant>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | MINUS num {
    $$ = std::make_unique<Constant>(std::move($2), -1);
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | CHAR {
    $$ = std::make_unique<Constant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | string_literal {
    $$ = std::make_unique<Constant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;
          
string_literal:
  STR_LIT {
    $$ = std::make_unique<StringLiteral>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;
          
num:
  INT_NUM {
    $$ = std::make_unique<Number>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | REAL_NUM {
    $$ = std::make_unique<Number>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;                  

type_declaration_part:
  %empty {
    $$ = nullptr;
  
  }
  | TYPE type_declarations SEMICOLON {
    $$ = std::make_unique<TypeDeclPart>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

type_declarations:
  type_declarations SEMICOLON ID EQ type_denoter {
    $$.swap($1);
    $$.emplace_back(std::make_unique<TypeDecl>(std::move($3), std::move($5)));
  }
  | ID EQ type_denoter {
    $$.emplace_back(std::make_unique<TypeDecl>(std::move($1), std::move($3)));
  }
  ;
                       
type_denoter:
  type_identifier {
    $$ = std::move($1);
  }
  | ARRAY LSB periods RSB OF type_denoter {
    $$ = std::make_unique<ArrayType>(std::move($6), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @6.end;
  }
  | RECORD field_list END {
    $$ = std::make_unique<RecordType>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;
              
type_identifier:
  ID {
    $$ = std::make_unique<TypeId>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;
              
field_list:
  %empty {
    /* nothing */
  }
  | variable_declarations opt_semicolon {
    $$.swap($1);
  }
  ;
            
periods:
  periods COMMA period {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | period {
    $$.emplace_back(std::move($1));
  }
  ;
         
period:
  constant RANGE constant {
    $$ = std::make_unique<Period>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

variable_declaration_part:
  %empty {
    $$ = nullptr;
  }
  | VAR variable_declarations SEMICOLON {
    $$ = std::make_unique<VarDeclPart>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;
                           
variable_declarations:
  variable_declarations SEMICOLON id_list COLON type_denoter {
    $$.swap($1);
    $$.emplace_back(std::make_unique<VarDecl>(std::move($3), std::move($5)));
    $$.back()->location().begin = @3.begin;
    $$.back()->location().end = @5.end;
  }
  | id_list COLON type_denoter {
    $$.emplace_back(std::make_unique<VarDecl>(std::move($1), std::move($3)));
    $$.back()->location().begin = @1.begin;
    $$.back()->location().end = @3.end;
  }
  ;
                       
subprogram_declaration_part:
  %empty  {
    $$ = nullptr;
  }
  | subprogram_declarations SEMICOLON {
    $$ = std::make_unique<SubprogDeclPart>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

subprogram_declarations:
  subprogram_declarations SEMICOLON subprogram_declaration {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | subprogram_declaration {
    $$.emplace_back(std::move($1));
  }
  ;
                          
subprogram_declaration:
  procedure_declaration {
    $$ = std::move($1);
  }
  | function_declaration {
    $$ = std::move($1);
  }
  ;

procedure_declaration:
  procedure_head procedure_block {
    $$ = std::make_unique<ProcDecl>(std::move($1), std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

procedure_head:
  PROCEDURE ID SEMICOLON {
    $$ = std::make_unique<ProcHead>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | PROCEDURE ID LPAREN RPAREN SEMICOLON {
    $$ = std::make_unique<ProcHead>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @5.end;
  }
  | PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMICOLON {
    $$ = std::make_unique<ProcHead>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @6.end;
  }
  ;

formal_parameter_list:
  formal_parameter_list SEMICOLON formal_parameter {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | formal_parameter {
    $$.emplace_back(std::move($1));
  }
  ;
                       
formal_parameter:
  value_parameter_specification {
    $$ = std::move($1);
  }
  | variable_parameter_specification {
    $$ = std::move($1);
  }
  ;

value_parameter_specification:
  id_list COLON type_identifier {
    $$ = std::make_unique<ValueParamSpec>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

variable_parameter_specification:
  VAR id_list COLON type_identifier {
    $$ = std::make_unique<VarParamSpec>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

procedure_block:
  block {
    $$ = std::make_unique<ProcBlock>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;

function_declaration:
  function_head function_block {
    $$ = std::make_unique<FuncDecl>(std::move($1), std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

function_head:
  FUNCTION ID COLON type_denoter SEMICOLON {
    drv.addFunction($2);
    drv.pushCurrentFunction($2);
    $$ = std::make_unique<FuncHead>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @5.end;
  }
  | FUNCTION ID LPAREN RPAREN COLON type_denoter SEMICOLON {
    drv.addFunction($2);
    drv.pushCurrentFunction($2);
    $$ = std::make_unique<FuncHead>(std::move($2), std::move($6));
    $$->location().begin = @1.begin;
    $$->location().end = @7.end;
  }
  | FUNCTION ID LPAREN formal_parameter_list RPAREN COLON type_denoter SEMICOLON {
    drv.addFunction($2);
    drv.pushCurrentFunction($2);
    $$ = std::make_unique<FuncHead>(std::move($2), std::move($4), std::move($7));
    $$->location().begin = @1.begin;
    $$->location().end = @8.end;
  }
  ;

function_block:
  block {
    $$ = std::make_unique<FuncBlock>(std::move($1));
    drv.popCurrentFunction();
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;

statement_part:
  compound_statement {
    $$ = std::make_unique<StmtPart>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;

compound_statement:
  BEGIN statement_list END {
    $$ = std::make_unique<CompoundStmt>(std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

statement_list:
  statement_list SEMICOLON statement {
    $$.swap($1);
    if ($3 != nullptr) {
      $$.emplace_back(std::move($3));
    }
  }
  | statement {
    if ($1 != nullptr) {
      $$.emplace_back(std::move($1));
    }
  }
  ;
                
statement:
  simple_statement {
    $$ = std::move($1);
  }
  | structured_statement {
    $$ = std::move($1);
  }
  ;

simple_statement:
  empty_statement {
    $$ = std::move($1);
  }
  | assignment_statement {
    $$ = std::move($1);
  }
  | break_statement {
    $$ = std::move($1);
  }
  | procedure_call_statement {
    $$ = std::move($1);
  }
  ;

empty_statement:
  %empty {
    $$ = nullptr;
  }
  ;

assignment_statement:
  var_access ASSIGN expr {
    $$ = std::make_unique<AssignStmt>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

break_statement:
  BREAK {
    $$ = std::make_unique<BreakStmt>();
    $$->location() = @1;
  }
  ;

var_access:
  ID {
    if (drv.isFunction($1) && $1 != drv.currentFunction()) {
      // var_access 可能不能被赋值的情况, foo()是函数调用，那么foo也是函数调用
      $$ = std::make_unique<FuncCall>(std::move($1));
    } else {
      $$ = std::make_unique<VarId>(std::move($1));
    }
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | indexed_variable {
    $$ = std::move($1);
  }
  | field_designator {
    $$ = std::move($1);
  }
  ;
                     
indexed_variable:
  var_access LSB expr_list RSB {
    $$ = std::make_unique<IndexedVar>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

field_designator:
  var_access PERIOD ID {
    $$ = std::make_unique<FieldDesignator>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

procedure_call_statement:
  ID {
    $$ = std::make_unique<ProcCallStmt>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | ID LPAREN RPAREN {
    $$ = std::make_unique<ProcCallStmt>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | ID LPAREN expr_list RPAREN {
    $$ = std::make_unique<ProcCallStmt>(std::move($1),std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  | write_statement {
    $$ = std::move($1);
  }
  | writeln_statement {
    $$ = std::move($1);
  }
  | read_statement {
    $$ = std::move($1);
  }
  | readln_statement {
    $$ = std::move($1);
  }
  | exit_statement {
    $$ = std::move($1);
  }
  ;
                          
write_statement:
  WRITE LPAREN expr_list RPAREN {
    $$ = std::make_unique<WriteStmt>(std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

writeln_statement:
  WRITELN {
    $$ = std::make_unique<WritelnStmt>();
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | WRITELN LPAREN RPAREN {
    $$ = std::make_unique<WritelnStmt>();
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | WRITELN LPAREN expr_list RPAREN {
    $$ = std::make_unique<WritelnStmt>(std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;
                   
read_statement:
  READ LPAREN var_access_list RPAREN {
    $$ = std::make_unique<ReadStmt>(std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

readln_statement:
  READLN LPAREN var_access_list RPAREN {
    $$ = std::make_unique<ReadlnStmt>(std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

exit_statement:
  EXIT {
    $$ = std::make_unique<ExitStmt>();
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | EXIT LPAREN RPAREN {
    $$ = std::make_unique<ExitStmt>();
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | EXIT LPAREN expr_list RPAREN {
    $$ = std::make_unique<ExitStmt>(std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

var_access_list:
  var_access_list COMMA var_access {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | var_access {
    $$.emplace_back(std::move($1));
  }
  ;

structured_statement:
  compound_statement {
    $$ = std::move($1);
  }
  | conditional_statement {
    $$ = std::move($1);
  }
  | repetitive_statement {
    $$ = std::move($1);
  }
  ;

conditional_statement:
  if_statement {
    $$ = std::move($1);
  }
  | case_statement {
    $$ = std::move($1);
  }
  ;

if_statement:
  IF bool_expr THEN statement else_part {
    $$ = std::make_unique<IfStmt>(std::move($2), std::move($4), std::move($5));
    $$->location().begin = @1.begin;
    $$->location().end = @5.end;
  }
  ;

bool_expr:
  expr {
    $$ = std::make_unique<BoolExpr>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;

else_part:
  %empty {
    $$ = nullptr;
  }
  | ELSE statement {
    $$ = std::move($2);
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

case_statement:
  CASE expr OF case_list_elements opt_semicolon END {
    $$ = std::make_unique<CaseStmt>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @6.end;
  }
  ;

case_list_elements:
  case_list_elements SEMICOLON case_list_element {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
 | case_list_element {
    $$.emplace_back(std::move($1));
 }
 ;
                    
case_list_element:
  case_constant_list COLON statement {
    $$ = std::make_unique<CaseListElement>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

case_constant_list:
  case_constant_list COMMA constant {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | constant {
    $$.emplace_back(std::move($1));
  }
  ;
                    
opt_semicolon:
  %empty {
    /* nothing */
  }
  | SEMICOLON {
    /* nothing */
  }
  ;

repetitive_statement:
  repeat_statement {
    $$ = std::move($1);
  }
  | while_statement {
    $$ = std::move($1);
  }
  | for_statement {
    $$ = std::move($1);
  }
  ;

repeat_statement:
  REPEAT statement_list UNTIL bool_expr {
    $$ = std::make_unique<RepeatStmt>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

while_statement:
  WHILE bool_expr DO statement {
    $$ = std::make_unique<WhileStmt>(std::move($2), std::move($4));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  }
  ;

for_statement:
  FOR var_access ASSIGN expr updown expr DO statement {
    $$ = std::make_unique<ForStmt>(
      std::move($2),
      std::move($4),
      std::move($6),
      std::move($8),
      std::move($5)
    );
    $$->location().begin = @1.begin;
    $$->location().end = @8.end;
  }
  ;
                 
updown:
  TO {
    $$ = true;
  }
  | DOWNTO {
    $$ = false;
  }
  ;

expr_list:
  expr_list COMMA expr {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | expr {
    $$.emplace_back(std::move($1));
  }
  ;

expr:
  simple_expr {
    $$ = std::move($1);
  }
  | simple_expr relop simple_expr {
    $$ = std::make_unique<BinaryExpr>($2, std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | string_expr {
    $$ = std::move($1);
  }
  ;
      
string_expr:
  STR_LIT {
    $$ = std::make_unique<StringLiteral>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;
    
simple_expr:
  term {
    $$ = std::move($1);
  }
  | simple_expr addop term {
    $$ = std::make_unique<BinaryExpr>($2, std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;
             
term:
  signed_factor {
    $$ = std::move($1);
  }
  | term mulop signed_factor {
    $$ = std::make_unique<BinaryExpr>($2, std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  ;

factor:
  var_access {
    $$ = std::move($1);
  }
  | function_designator {
    $$ = std::move($1);
  }
  | LPAREN expr RPAREN  {
    $$ = std::move($2);
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  }
  | NOT factor {
    $$ = std::make_unique<UnaryExpr>(UnaryOp::NOT, std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | unsigned_constant {
    $$ = std::move($1);
  }
  ;

signed_factor:
  factor {
    $$ = std::move($1);
  }
  | PLUS signed_factor {
    $$ = std::make_unique<UnaryExpr>(UnaryOp::PLUS, std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  | MINUS signed_factor {
    $$ = std::make_unique<UnaryExpr>(UnaryOp::MINUS, std::move($2));
    $$->location().begin = @1.begin;
    $$->location().end = @2.end;
  }
  ;

function_designator:
  ID LPAREN RPAREN {
    $$ = std::make_unique<FuncCall>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @3.end;
  } 
  | ID LPAREN expr_list RPAREN {
    $$ = std::make_unique<FuncCall>(std::move($1), std::move($3));
    $$->location().begin = @1.begin;
    $$->location().end = @4.end;
  } 
  ;       
        
unsigned_constant:
  num {
    $$ = std::make_unique<UnsignedConstant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | CHAR {
    $$ = std::make_unique<UnsignedConstant>(std::move($1));
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | TRUE {
    $$ = std::make_unique<UnsignedConstant>(true);
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  | FALSE {
    $$ = std::make_unique<UnsignedConstant>(false);
    $$->location().begin = @1.begin;
    $$->location().end = @1.end;
  }
  ;

relop:
  EQ {
    $$ = BinOp::EQ;
  }
  | NE {
    $$ = BinOp::NE;
  }
  | LT {
    $$ = BinOp::LT;
  }
  | GT {
    $$ = BinOp::GT;
  }
  | LE {
    $$ = BinOp::LE;
  }
  | GE {
    $$ = BinOp::GE;
  }
  ;

addop:
  PLUS {
    $$ = BinOp::PLUS;
  }
  | MINUS {
    $$ = BinOp::MINUS;
  }
  | OR {
    $$ = BinOp::OR;
  }
  ;

mulop:
  MUL {
    $$ = BinOp::MUL;
  }
  | FDIV {
    $$ = BinOp::FDIV;
  }
  | IDIV {
    $$ = BinOp::IDIV;
  }
  | MOD {
    $$ = BinOp::MOD;
  }
  | AND {
    $$ = BinOp::AND;
  }
  ;

%%

namespace pascc::parse {

void Parser::error(const location_type &loc, const std::string &msg)
{
  std::stringstream ss;
  ss << loc << ": " << msg << '\n';
  LOG_ERROR("{}", ss.str());
}

}