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

%token <std::string> ID "identifier"
%token <std::string> STR_LIT "string_literal"
%token <int> INT_NUM "integer_number"
%token <double> REAL_NUM "float_number"

%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<ProgramHead>> program_head
%nterm <std::unique_ptr<ProgramBlock>> program_body
%nterm <std::unique_ptr<StmtBlock>> statement_block
%nterm <std::unique_ptr<Stmt>> statement
%nterm <std::unique_ptr<ProcCallStmt>> procedure_statement

%start program

%%

program:
  program_head program_body {
    $$ = std::make_unique<Program>(std::move(*$1), std::move(*$2));
    drv.program_ = std::move($$);
  }
  ;

program_head:
  PROGRAM ID SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
  }
  ;

program_body:
  statement_block PERIOD {
    $$ = std::make_unique<ProgramBlock>(std::move(*$1));
  }
  ;

statement_block:
  BEGIN statement END {
    std::vector<std::unique_ptr<Stmt>> stmts;
    stmts.emplace_back(std::move($2));
    $$ = std::make_unique<StmtBlock>(std::move(stmts));
  }
  ;

statement:
  procedure_statement {
    $$ = std::move($1);
  }
  ;

procedure_statement:
  WRITELN LPAREN RPAREN {
    $$ = std::make_unique<WritelnStmt>();
  }
  ;

%%

namespace pascc::parse {

void Parser::error(const location_type &loc, const std::string &msg)
{
  std::cerr << loc << ": " << msg << '\n';
}

}