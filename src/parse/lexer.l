%{
#include <string>
#include <cstdlib>
#include "parse/parser_driver.hpp"
#include "parser.hpp"
using namespace pascc::parse;
namespace {
std::string get_str_lit();
int get_int_num();
double get_real_num();
} // namespace
%}

%option noyywrap
%option nounput
%option noinput 
%option batch
%option debug
%option case-insensitive

BLANK             [ \t\r]
ID                [A-Za-z_][A-Za-z0-9_]*
STR_LIT           \'([^\\\'\n]|(\\.)){2,}\'
INT_NUM           -?\d+
REAL_NUM          -?\d+(\.\d+)?

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  location& loc = drv.location();
  // Code run each time yylex is called.
  loc.step();
%}

{BLANK}+         loc.step();
\n+              loc.lines(yyleng); loc.step();

AND              return Parser::make_AND(loc);
ARRAY            return Parser::make_ARRAY(loc);
BEGIN            return Parser::make_BEGIN(loc);
CASE             return Parser::make_CASE(loc);
CONST            return Parser::make_CONST(loc);
DIV              return Parser::make_IDIV(loc);
DO               return Parser::make_DO(loc);
DOWNTO           return Parser::make_DOWNTO(loc);
ELSE             return Parser::make_ELSE(loc);
END              return Parser::make_END(loc);
FILE             return Parser::make_FILE(loc);
FOR              return Parser::make_FOR(loc);
FUNCTION         return Parser::make_FUNCTION(loc);
GOTO             return Parser::make_GOTO(loc);
IF               return Parser::make_IF(loc);
IN               return Parser::make_IN(loc);
LABEL            return Parser::make_LABEL(loc);
MOD              return Parser::make_MOD(loc);
NIL              return Parser::make_NIL(loc);
NOT              return Parser::make_NOT(loc);
OF               return Parser::make_OF(loc);
OR               return Parser::make_OR(loc);
PACKED           return Parser::make_PACKED(loc);
PROCEDURE        return Parser::make_PROCEDURE(loc);
PROGRAM          return Parser::make_PROGRAM(loc);
RECORD           return Parser::make_RECORD(loc);
REPEAT           return Parser::make_REPEAT(loc);
SET              return Parser::make_SET(loc);
THEN             return Parser::make_THEN(loc);
TO               return Parser::make_TO(loc);
TYPE             return Parser::make_TYPE(loc);
UNTIL            return Parser::make_UNTIL(loc);
VAR              return Parser::make_VAR(loc);
WHILE            return Parser::make_WHILE(loc);
WITH             return Parser::make_WITH(loc);

READ             return Parser::make_READ(loc);
READLN           return Parser::make_READLN(loc);
WRITE            return Parser::make_WRITE(loc);
WRITELN          return Parser::make_WRITELN(loc);
TRUE             return Parser::make_TRUE(loc);
FALSE            return Parser::make_FALSE(loc);
EXIT             return Parser::make_EXIT(loc);

{ID}             return Parser::make_ID(yytext, loc);
{STR_LIT}        return Parser::make_STR_LIT(get_str_lit(), loc);
{INT_NUM}        return Parser::make_INT_NUM(get_int_num(),loc);
{REAL_NUM}       return Parser::make_REAL_NUM(get_real_num(),loc);

"+"              return Parser::make_PLUS(loc);
"-"              return Parser::make_MINUS(loc);
"*"              return Parser::make_MUL(loc);
"/"              return Parser::make_FDIV(loc);
"="              return Parser::make_EQ(loc);
">"              return Parser::make_GT(loc);
"<"              return Parser::make_LT(loc);
"<>"             return Parser::make_NE(loc);
"<="             return Parser::make_LE(loc);
">="             return Parser::make_GE(loc);
":="             return Parser::make_ASSIGN(loc);

"["              return Parser::make_LSB(loc);
"]"              return Parser::make_RSB(loc);
"("              return Parser::make_LPAREN(loc);
")"              return Parser::make_RPAREN(loc);
"."              return Parser::make_PERIOD(loc);
","              return Parser::make_COMMA(loc);
":"              return Parser::make_COLON(loc);
";"              return Parser::make_SEMICOLON(loc);
".."             return Parser::make_RANGE(loc);

.                {
                    throw Parser::syntax_error(loc, "illegal character: " + std::string(yytext));
                 }

<<EOF>>          return Parser::make_EOF(loc);

%%

namespace {

std::string get_str_lit()
{
  std::string str = std::string(yytext);
  str = str.substr(1, str.length() - 2);  
  return str;
}

int get_int_num()
{
  std::string str = std::string(yytext);
  return std::stoi(str);
}

double get_real_num()
{
  std::string str = std::string(yytext);
  return std::stod(str);
}

} // namespace

namespace pascc::parse {
  
void
ParserDriver::scan_begin()
{
  yy_flex_debug = trace_scanning_;
  if (filename_.empty() || filename_ == "-") {
    yyin = stdin;
  }
  else if (!(yyin = fopen(filename_.c_str(), "r")))
  {
    std::cerr << "cannot open " << filename_ << ": " << strerror(errno) << '\n';
    exit(EXIT_FAILURE);
  }
}

void
ParserDriver::scan_end()
{
  fclose(yyin);
}

} // namespace pascc::parse