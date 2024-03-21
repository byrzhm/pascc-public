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
  #include "parse/parser_driver.hpp"
  using namespace pascc::parse;
}

%param { ParserDriver& drv }

%expect 0

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

%token <std::vector<std::string>> id_list "id_list"
%token <std::string> ID "identifier"
%token <std::string> STR_LIT "string_literal"
%token <int> INT_NUM "integer_number"
%token <double> REAL_NUM "float_number"
%token <char> CHAR "character"

%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<ProgramHead>> program_head
%nterm <std::unique_ptr<ProgramBlock>> program_block
%nterm <std::unque_ptr<Block>> block
%nterm <std::unique_ptr<ConstDeclPart>> constant_declaration_part
%nterm <std::vector<std::unique_ptr<ConstDecl>>> constant_declarations
%nterm <std::unique_ptr<Constant>> constant
%nterm <std::unique_ptr<StringLiteral>> string_literal
%nterm <std::unique_ptr<Number>> num
%nterm <std::unique_ptr<TypeDeclPart>> type_declaration_part
%nterm <std::unique_ptr<TypeDecl>> type_declarations
%nterm <std::unique_ptr<TypeDenoter>> type_denoter
%nterm <std::unique_ptr<TypeId>> type_identifier
%nterm <std::vector<std::unique_ptr<VarDecl>>> field_list
%nterm <std::vector<std::unique_ptr<Period>>> periods
%nterm <std::unique_ptr<Period>> period
%nterm <std::unique_ptr<VarDeclPart>> variable_declaration_part
%nterm <std::vector<std::unique_ptr<VarDecl>>> variable_declarations
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
%nterm <std::unique_ptr<StmtPart>> statement_part
%nterm <std::unique_ptr<Stmt>> compound_statement
%nterm <std::vector<std::unique_ptr<Stmt>>> statement_list
%nterm <std::unique_ptr<Stmt>> statement
%nterm <std::unique_ptr<Stmt>> simple_statement
%nterm <std::unique_ptr<Stmt>> empty_statement
%nterm <std::unique_ptr<Stmt>> assignment_statement
%nterm <std::unique_ptr<Assignable>> assignable
%nterm <std::unique_ptr<Assignable>> indexed_variable
%nterm <std::unique_ptr<Assignable>> field_designator
%nterm <std::unique_ptr<Stmt>>procedure_call_statement
%nterm <std::unique_ptr<Stmt>> write_statement
%nterm <std::unique_ptr<Stmt>> writeln_statement
%nterm <std::unique_ptr<Stmt>> read_statement
%nterm <std::unique_ptr<Stmt>> readln_statement
%nterm <std::unique_ptr<Stmt>> exit_statement
%nterm <std::vector<std::unique_ptr<Assignable>>> assignable_list
%nterm <std::unique_ptr<Stmt>> structured_statement
%nterm <std::unique_ptr<Stmt>> conditional_statement
%nterm <std::unique_ptr<Stmt>> if_statement
%nterm <std::unique_ptr<Expr>> bool_expr
%nterm <std::unique_ptr<Stmt>> else_part
%nterm <std::unique_ptr<Stmt>> case_statement
%nterm <std::vector<std::unique_ptr<CaseListElement>>> case_list_elements
%nterm <std::unique_ptr<CaseListElement>> case_list_element
%nterm <std::vector<std::unique_ptr<Constant>>> case_constant_list
%nterm <> opt_semicolon
%nterm <std::unique_ptr<Stmt>> repetitive-statement
%nterm <std::unique_ptr<Stmt>> repeat_statement
%nterm <std::unique_ptr<Stmt>> while_statement
%nterm <std::unique_ptr<Stmt>> for_statement
%nterm <bool> updown
%nterm <std::vector<std::unique_ptr<Expr>>> expr_list
%nterm <std::unique_ptr<Expr>> expr
%nterm <std::unique_ptr<Expr>> string_expr
%nterm <std::unique_ptr<Expr>> simple_expr
%nterm <std::unique_ptr<Expr>> term
%nterm <std::unique_ptr<Expr>> factor
%nterm <std::unique_ptr<Expr>> function_designator
%nterm <std::unique_ptr<Expr>> unsigned_constant
%nterm <BinOp> relop
%nterm <BinOp> addop
%nterm <BinOp> mulop

%start program

%%

program: 
  program_head program_block {
    // std::make_unique<T>(...) 将会生成 std::unique_ptr<T>, 其中 T 会使用 ... 提供的参数进行构造 
    $$ = std::make_unique<Program>(std::move($1), std::move($2));
    drv.program_ = std::move($$);
  }
  ;

program_head:
  PROGRAM ID LPAREN id_list RPAREN SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2), std::move($4));
  }
  | PROGRAM ID LPAREN RPAREN SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
  }
  | PROGRAM ID SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
  }
  ;

program_block:
  block PERIOD {
    $$ = std::make_unique<ProgramBlock>(std::move($1));
  }
  ;

block:
  constant_declaration_part type_declaration_part variable_declaration_part
   subprogram_declaration_part statement_part
  {
    $$ = std::make_unique<Block>(std::move($1),std::move($2),std::move($3),std::move($4),std::move($5))
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
  %empty
  | CONST constant_declarations SEMICOLON {
    $$ = std::make_unique<ConstDeclPart>(std::move($2));
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
  }
  | MINUS ID {
    $$ = std::make_unique<Constant>(std::move($2),-1);
  }
  | ID {
    $$ = std::make_unique<Constant>(std::move($1));
  }
  | num {
    $$ = std::make_unique<Constant>(std::move($1));
  }
  | PLUS num {
    $$ = std::make_unique<Constant>(std::move($2));
  }
  | MINUS num {
    $$ = std::make_unique<Constant>(std::move($2),-1);
  }
  | CHAR {
    $$ = std::make_unique<Constant>(std::move($1));
  }
  | string_literal {
    $$ = std::make_unique<Constant>(std::move($1));
  }
  ;
          
string_literal:
  STR_LIT {
    $$ = std::make_unique<StringLiteral>(std::move($1));
  }
  ;
          
num:
  INT_NUM {
    $$ = std::make_unique<Number>(std::move($1));
  }
  | REAL_NUM {
    $$ = std::make_unique<Number>(std::move($1));
  }
  ;                  

type_declaration_part:
  %empty
  | TYPE type_declarations SEMICOLON {
    $$ = make_unique<TypeDeclPart>(std::move($2));
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
    $$ = std::make_unique<TypeId>(std::move($1));
  }
  | ARRAY LSB periods RSB OF type_denoter {
    $$ = std::make_unique<ArrayType>(std::move($6), std::move($3));
  }
  | RECORD field_list END {
    $$ = std::make_unique<RecordType>(std::move($2));
  }
  ;
              
type_identifier:
  ID {
    $$ = std::move($1);
  }
  ;
              
field_list:
  %empty
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
    $$ = std::make_unique<Period>(std::move($1),std::move($3));
  }
  ;

variable_declaration_part:
  %empty
  | VAR variable_declarations SEMICOLON {
    $$.swap($2);
  }
  ;
                           
variable_declarations:
  variable_declarations SEMICOLON id_list COLON type_denoter {
    $$.swap($1);
    $$.emplace_back(std::make_unique<VarDecl>(std::move($3),std::move($5)));
  }
  | id_list COLON type_denoter {
    $$.emplace_back(std::make_unique<VarDecl>(std::move($1),std::move($3)));
  }
  ;
                       
subprogram_declaration_part:
  %empty 
  | subprogram_declarations SEMICOLON {
    $$.swap($1);
  }
  ;

subprogram_declarations:
  subprogram_declarations SEMICOLON subprogram_declaration {
    $$.swap($1);
    $$.emplace_back(std::make_unique<SubprogDecl>(std::move($3)));
  }
  | subprogram_declaration {
    $$.emplace_back(std::make_unique<SubprogDecl>(std::move($1)));
  }
  ;
                          
subprogram_declaration:
  procedure_declaration {
    $$ = std::make_unique<ProcDecl>(std::move($1));
  }
  | function_declaration {
    $$ = std::make_unique<FuncDecl>(std::move($1));
  }
  ;

procedure_declaration:
  procedure_head procedure_block {
    $$ = std::make_unique<ProcDecl>(std::move($1),std::move($2));
  }
  ;

procedure_head:
  PROCEDURE ID SEMICOLON {
    $$ = std::make_unique<ProcHead>(std::move($2));
  }
  | PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMICOLON {
    $$ = std::make_unique<ProcHead>(std::move($2),std::move($4));
  }
  ;

formal_parameter_list:
  formal_parameter_list SEMICOLON formal_parameter {
    $$.swap($1);
    $$.emplace_back(make_unique<FormalParam>(std::move($3)));
  }
  | formal_parameter {
    $$.emplace_back(make_unique<FormalParam>(std::move($1)));
  }
  ;
                       
formal_parameter:
  value_parameter_specification {
    $$ = std::make_unique<ValueParamSpec>(std::move($1));
  }
  | variable_parameter_specification {
    $$ = std::make_unique<VarParamSpec>(std::move($1));
  }
  ;

value_parameter_specification:
  id_list COLON ID {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  ;

variable_parameter_specification:
  VAR id_list COLON ID {
    $$.swap($2);
    $$.emplace_back(std::move($4));
  }
  ;

procedure_block:
  block {
    $$ = std::make_unique<ProcBlock>(std::move($1));
  }
  ;

function_declaration:
  function_head function_block {
    $$ = std::make_unique<FuncDecl>(std::move($1),std::move($2));
  }
  ;

function_head:
  FUNCTION ID COLON type_denoter SEMICOLON {
    $$ = std::make_unique<FuncHead>(std::move($2),std::move($4));
  }
  | FUNCTION ID LPAREN formal_parameter_list RPAREN COLON type_denoter SEMICOLON {
    
  }
  ;

function_block:
  block {
    $$ = std::make_unique<FuncBlock>(std::move($1));
  }
  ;

statement_part:
  compound_statement {
    $$ = std::make_unique<StmtPart>(stsd::move($1));
  }
  ;

compound_statement:
  BEGIN statement_list END {
    $$.swap($2);
  }
  ;

statement_list:
  statement_list SEMICOLON statement {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | statement {
    $$.emplace_back(std::move($1));
  }
  ;
                
statement:
  simple_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | structured_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;

simple_statement:
  empty_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | assignment_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | procedure_call_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;

empty_statement:
  %empty
  ;

assignment_statement:
  assignable ASSIGN expr {
    $$ = std::make_unique<Stmt>(std::move($1),std::move($2));
  }
  ;

assignable:
  ID {
    $$ = std::make_unique<Assignable>(std::move($1));
  }
  | indexed_variable {
    $$ = std::make_unique<Assignable>(std::move($1));
  }
  | field_designator {
    $$ = std::make_unique<Assignable>(std::move($1));
  }
  ;
                     
indexed_variable:
  assignable LSB expr_list RSB {
    $$ = std::make_unique<Assignable>(std::move($1),std::move($3));
  }
  ;

field_designator:
  assignable PERIOD ID {
    $$ = std::make_unique<Assignable>(std::move($1),std::move($3));
  }
  ;

procedure_call_statement:
  ID {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | ID LPAREN RPAREN {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | ID LPAREN expr_list RPAREN {
    $$ = std::make_unique<Stmt>(std::move($1),std::move($3));
  }
  | write_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | writeln_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | read_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | readln_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | exit_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;
                          
write_statement:
  WRITE LPAREN expr_list RPAREN {
    $$ = std::make_unique<Stmt>(std::move($3));
  }
  ;

writeln_statement:
  WRITELN {

  }
  | WRITELN LPAREN RPAREN {

  }
  | WRITELN LPAREN expr_list RPAREN {
    $$ = std::make_unique<Stmt>(std::move($3));
  }
  ;
                   
read_statement:
  READ LPAREN assignable_list RPAREN {

  }
  ;

readln_statement:
  READLN LPAREN assignable_list RPAREN {
    $$ = std::make_unique<Stmt>(std::move($3));
  }
  ;

exit_statement:
  EXIT {
    $$ = std::make_unique<ExitStmt>();
  }
 ｜EXIT LPAREN RPAREN {
    $$ = std::make_unique<ExitStmt>();
 }
 ｜EXIT LPAREN expr RPAREN {
  $$ = std::make_unique<Stmt>(std::move($3));
 }
 ;

assignable_list:
  assignable_list COMMA assignable {
    $$.swap($1);
    $$.emplace_back(std::move($3));
  }
  | assignable {
    $$.emplace_back(std::move($1));
  }
  ;

structured_statement:
  compound_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | conditional_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | repetitive_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;

conditional-statement:
  if_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | case_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;

if_statement:
  IF bool_expr THEN statement else_part {
    $$ = std::make_unique<Stmt>(std::move($2),std::move($4),std::move($5));
  }
  ;

bool_expr:
  expr {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  ;

else_part:
  %empty
  | ELSE statement {
    $$ = std::make_unique<Stmt>(std::move($2));
  }
  ;

case_statement:
  CASE expr OF case_list_elements opt_semicolon END {
    $$ = std::make_unique<Stmt>(std::move($2),std::move($4));
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
    $$ = std::make_unique<CaseListElement>(std::move($1),std::move($3));
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
  %empty
  | SEMICOLON {

  }
  ;

repetitive-statement:
  repeat_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | while_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  | for_statement {
    $$ = std::make_unique<Stmt>(std::move($1));
  }
  ;

repeat_statement:
  REPEAT statement_list UNTIL bool_expr {
    $$ = std::make_unique<RepeatStmt>(std::move($2),std::move($4));
  }
  ;

while_statement:
  WHILE bool_expr DO statement {
    $$ = std::make_unique<WhileStmt>(std::move($2),std::move($4));
  }
  ;

for_statement:
  FOR ID ASSIGN expr updown expr DO statement {
    $$ = std::make_unique<ForStmt>(std::move($2),std::move($4),std::move($5),std::move($6),std::move($8));
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
    $$ = std::make_unique<Expr>(std::move(simple_expr));
  }
  | simple_expr relop simple_expr {
    $$ = std::make_unique<Expr>(std::move($2),std::move($1),std::move($3));
  }
  | string_expr {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  ;
      
string_expr:
  STR_LIT {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  ;
    
simple_expr:
  term {
    $$ = std::make_unique<Expr>(std::move($1));
  }
 | PLUS term {
    $$ = std::make_unique<Expr>(UnaryOp::PLUS,std::move($2));
 }
 | MINUS term {
    $$ = std::make_unique<Expr>(UnaryOp::MINUS, std::move($2));
 }
 | simple_expr addop term {
    $$ = std::make_unique<Expr>($2, std::move($1), std::move($3));
 }
 ;
             
term:
  factor {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  | term mulop factor {
    $$ = std::make_unique<Expr>($2, std::move($1), std::move($3));
  }
  ;

factor:
  assignable {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  | function_designator {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  | LPAREN expr RPAREN  {
    $$ = std::make_unique<Expr>(std::move($2));
  }
  | NOT factor {
    $$ = std::make_unique<Expr>(UnaryOp::NOT, std::move($2));
  }
  | unsigned_constant {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  ;

function_designator:
  ID LPAREN expr_list RPAREN {
    $$ = std::make_unique<Expr>(std::move($1), std::move($3));
  } 
  ;       
        
unsigned_constant:
  num {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  | CHAR {
    $$ = std::make_unique<Expr>(std::move($1));
  }
  | TRUE {
    $$ = std::make_unique<Expr>(true);
  }
  | FALSE {
    $$ = std::make_unique<Expr>(false);
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
  std::cerr << loc << ": " << msg << '\n';
}

}