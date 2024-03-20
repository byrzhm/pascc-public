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
%token <char> CHAR "character"

%nterm <std::unique_ptr<Program>> program
%nterm <std::unique_ptr<ProgramHead>> program_head
%nterm <std::unique_ptr<ProgramBlock>> program_body
%nterm <std::unique_ptr<StmtBlock>> statement_block
%nterm <std::unique_ptr<Stmt>> statement
%nterm <std::unique_ptr<ProcCallStmt>> procedure_statement

%start program

%%

program: 
  program_head program_block {
    
  }
  ;

program_head:
  PROGRAM ID LPAREN id_list RPAREN SEMICOLON {

  }
  | PROGRAM ID LPAREN RPAREN SEMICOLON {

  }
  | PROGRAM ID SEMICOLON {

  }
  ;

program_block:
  block PERIOD {

  }
  ;

block:
  constant_declaration_part type_declaration_part variable_declaration_part
   subprogram_declaration_part statement_part
  {

  }
  ;

id_list:
  id_list COMMA ID {

  }
  | ID {

  }
  ;

constant_declaration_part:
  %empty
  | CONST constant_declarations SEMICOLON {

  }
  ;

constant_declarations:
  constant_declarations SEMICOLON ID EQ constant {

  }
  | ID EQ constant {

  }
  ;
                       
constant:
  PLUS ID {

  }
  | MINUS ID {

  }
  | ID {

  }
  | num {

  }
  | PLUS num {

  }
  | MINUS num {

  }
  | CHAR {

  }
  | string_literal {

  }
  ;
          
string_literal:
  STR_LIT {

  }
  ;
          
num:
  INT_NUM {

  }
  | REAL_NUM {

  }
  ;                  

type_declaration_part:
  %empty
  | TYPE type_declarations SEMICOLON {

  }
  ;

type_declarations:
  type_declarations SEMICOLON ID EQ type_denoter {

  }
  | ID EQ type_denoter {

  }
  ;
                       
type_denoter:
  type_identifier {

  }
  | ARRAY LSB periods RSB OF type_denoter {

  }
  | RECORD field_list END {

  }
  ;
              
type_identifier:
  ID {

  }
  ;
              
field_list:
  %empty
  | variable_declarations opt_semicolon {

  }
  ;
            
periods:
  periods COMMA period {

  }
  | period {

  }
  ;
         
period:
  constant RANGE constant {

  }
  ;

variable_declaration_part:
  %empty
  | VAR variable_declarations SEMICOLON {

  }
  ;
                           
variable_declarations:
  variable_declarations SEMICOLON id_list COLON type_denoter {

  }
  | id_list COLON type_denoter {

  }
  ;
                       
subprogram_declaration_part:
  %empty 
  | subprogram_declarations SEMICOLON {

  }
  ;

subprogram_declarations:
  subprogram_declarations SEMICOLON subprogram_declaration {

  }
  | subprogram_declaration {

  }
  ;
                          
subprogram_declaration:
  procedure_declaration {

  }
  | function_declaration {

  }
  ;

procedure_declaration:
  procedure_head procedure_block {

  }
  ;

procedure_head:
  PROCEDURE ID SEMICOLON {

  }
  | PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMICOLON {

  }
  ;

formal_parameter_list:
  formal_parameter_list SEMICOLON formal_parameter {

  }
  | formal_parameter {

  }
  ;
                       
formal_parameter:
  value_parameter_specification {

  }
  | variable_parameter_specification {

  }
  ;

value_parameter_specification:
  id_list COLON ID {

  }
  ;

variable_parameter_specification:
  VAR id_list COLON ID {

  }
  ;

procedure_block:
  block {

  }
  ;

function_declaration:
  function_head function_block {

  }
  ;

function_head:
  FUNCTION ID COLON type_denoter SEMICOLON {

  }
  | FUNCTION ID LPAREN formal_parameter_list RPAREN COLON type_denoter SEMICOLON {

  }
  ;

function_block:
  block {

  }
  ;

statement_part:
  compound_statement {

  }
  ;

compound_statement:
  BEGIN statement_list END {

  }
  ;

statement_list:
  statement_list SEMICOLON statement {

  }
  | statement {

  }
  ;
                
statement:
  simple_statement {

  }
  | structured_statement {

  }
  ;

simple_statement:
  empty_statement {

  }
  | assignment_statement {

  }
  | procedure_call_statement {

  }
  ;

empty_statement:
  %empty
  ;

assignment_statement:
  assignable ASSIGN expr {

  }
  ;

assignable:
  ID {

  }
  | indexed_variable {

  }
  | field_designator {

  }
  ;
                     
indexed_variable:
  assignable LSB expr_list RSB {

  }
  ;

field_designator:
  assignable PERIOD ID {

  }
  ;

procedure_call_statement:
  ID {

  }
  | ID LPAREN RPAREN {

  }
  | ID LPAREN expr_list RPAREN {

  }
  | write_statement {

  }
  | writeln_statement {

  }
  | read_statement {

  }
  | readln_statement {

  }
  | exit_statement {

  }
  ;
                          
write_statement:
  WRITE LPAREN expr_list RPAREN {

  }
  ;

writeln_statement:
  WRITELN {

  }
  | WRITELN LPAREN RPAREN {

  }
  | WRITELN LPAREN expr_list RPAREN {

  }
  ;
                   
read_statement:
  READ LPAREN assignable_list RPAREN {

  }
  ;

readln_statement:
  READLN LPAREN assignable_list RPAREN {

  }
  ;

exit_statement:
  EXIT {

  }
 ｜EXIT LPAREN RPAREN {

 }
 ｜EXIT LPAREN expr RPAREN {

 }
 ;

assignable_list:
  assignable_list COMMA assignable {

  }
  | assignable {

  }
  ;

structured_statement:
  compound_statement {

  }
  | conditional_statement {

  }
  | repetitive_statement {

  }
  ;

conditional-statement:
  if_statement {

  }
  | case_statement {

  }
  ;

if_statement:
  IF bool_expr THEN statement else_part {

  }
  ;

bool_expr:
  expr {

  }
  ;

else_part:
  %empty
  | ELSE statement {

  }
  ;

case_statement:
  CASE expr OF case_list_elements opt_semicolon END {

  }
  ;

case_list_elements:
  case_list_elements SEMICOLON case_list_element {

  }
 | case_list_element {

 }
 ;
                    
case_list_element:
  case_constant_list COLON statement {

  }
  ;

case_constant_list:
  case_constant_list COMMA constant {

  }
  | constant {

  }
  ;
                    
opt_semicolon:
  %empty
  | SEMICOLON {

  }
  ;

repetitive-statement:
  repeat_statement {

  }
  | while_statement {

  }
  | for_statement {

  }
  ;

repeat_statement:
  REPEAT statement_list UNTIL bool_expr {

  }
  ;

while_statement:
  WHILE bool_expr DO statement {

  }
  ;

for_statement:
  FOR ID ASSIGN expr updown expr DO statement {

  }
  ;
                 
updown:
  TO {

  }
  | DOWNTO {

  }
  ;

expr_list:
  expr_list COMMA expr {

  }
  | expr {

  }
  ;

expr:
  simple_expr {

  }
  | simple_expr relop simple_expr {

  }
  | string_expr {

  }
  ;
      
string_expr:
  STR_LIT {

  }
  ;
    
simple_expr:
  term {

  }
 | PLUS term {

 }
 | MINUS term {

 }
 | simple_expr addop term {

 }
 ;
             
term:
  factor {

  }
  | term mulop factor {

  }
  ;

factor:
  assignable {

  }
  | function_designator {

  }
  | LPAREN expr RPAREN  {

  }
  | NOT factor {

  }
  | unsigned_constant {

  }
  ;

function_designator:
  ID LPAREN expr_list RPAREN {

  } 
  ;       
        
unsigned_constant:
  num {

  }
  | CHAR {

  }
  | TRUE {

  }
  | FALSE {

  }
  ;

relop:
  EQ {

  }
  | NE {

  }
  | LT {

  }
  | GT {

  }
  | LE {

  }
  | GE {

  }
  ;

addop:
  PLUS {

  }
  | MINUS {

  }
  | OR {

  }
  ;

mulop:
  MUL {

  }
  | FDIV {

  }
  | IDIV {

  }
  | MOD {

  }
  | AND {

  }
  ;

%%

namespace pascc::parse {

void Parser::error(const location_type &loc, const std::string &msg)
{
  std::cerr << loc << ": " << msg << '\n';
}

}