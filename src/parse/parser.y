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
  PROGRAM   "program"
  BEGIN     "begin"
  END       "end"
  WRITELN   "writeln"
  SEMICOLON ";"
  PERIOD    "."
  LPAREN    "("
  RPAREN    ")"
;

%token <std::string> ID "identifier"

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
  }
  ;

program_head:
  PROGRAM ID SEMICOLON {
    $$ = std::make_unique<ProgramHead>(std::move($2));
  }
  ;

program_body:
  statement_block {
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