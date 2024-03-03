%skeleton "lalr1.cc"
%require "3.8"
%defines
%define api.parser.class {YYParser}

%code requires { /* 这会写入头文件中 */
  #include "ast/ast.h"
  using namespace cai;
  namespace yy {
    class YYDriver;
  }
}

%param { YYDriver& drv }


%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%expect 0

%code{
  #include "parse/yy_driver.h"
}

%define api.token.prefix {TOK_}

%token EOF 0

%token
  LPAREN    "("
  RPAREN    ")"
  LBRACE    "{"
  RBRACE    "}"
  SEMICOLON ";"
;

%token
  RETURN "return"
  INT    "int"
;

%token <int> INT_LIT
%token <std::string>  ID


%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<FunctionDeclaration>> func_decl
%nterm <std::unique_ptr<Statement>> stmt
%nterm <std::unique_ptr<Expression>> expr

%start program

%%

program:
    func_decl {
      $$ = std::make_unique<Program>(std::move($1));
      drv.set_program(std::move($$));
    }
    ;

func_decl:
    INT ID LPAREN RPAREN LBRACE stmt RBRACE {
      $$ = std::make_unique<FunctionDeclaration>($2, std::move($6));
    }
    ;

stmt:
    RETURN expr SEMICOLON {
      $$ = std::make_unique<ReturnStatement>(std::move($2));
    }
    ;

expr:
    INT_LIT {
      $$ = std::make_unique<IntegerLiteral>($1);
    }
    ;

%%

void yy::YYParser::error(const location_type &l, const std::string &m)
{
  std::cerr << l << ": " << m << '\n';
}