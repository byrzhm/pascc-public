%{
#include <string>
#include <cstdlib>
#include "parse/parser_driver.hpp"
#include "parser.hpp"
using namespace pascc::parse;
namespace {
std::string get_str_lit();
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

BEGIN            return Parser::make_BEGIN(loc);
END              return Parser::make_END(loc);
PROGRAM          return Parser::make_PROGRAM(loc);
WRITELN          return Parser::make_WRITELN(loc);
{ID}             return Parser::make_ID(yytext, loc);
{STR_LIT}        return Parser::make_STR_LIT(get_str_lit(), loc);

"("              return Parser::make_LPAREN(loc);
")"              return Parser::make_RPAREN(loc);
";"              return Parser::make_SEMICOLON(loc);
"."              return Parser::make_PERIOD(loc);
"<"              return Parser::make_LESS(loc);

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