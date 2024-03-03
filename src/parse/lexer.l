%{
#include <string>
#include <cstdlib>
// #include ...
%}

%option noyywrap
%option nounput
%option noinput 
%option batch
%option debug

BLANK             [ \t\r]
DIGIT             [0-9]
ID                [A-Za-z_][A-Za-z0-9_]*
INT_LIT           {DIGIT}+

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.get_location();
  // Code run each time yylex is called.
  loc.step ();
%}

{BLANK}+         loc.step();
\n+              loc.lines (yyleng); loc.step();

int              return yy::YYParser::make_INT(loc);
return           return yy::YYParser::make_RETURN(loc);
{ID}             return yy::YYParser::make_ID(yytext, loc);
{INT_LIT}        return yy::YYParser::make_INT_LIT(atoi(yytext), loc);

"{"              return yy::YYParser::make_LBRACE(loc);
"}"              return yy::YYParser::make_RBRACE(loc);
"("              return yy::YYParser::make_LPAREN(loc);
")"              return yy::YYParser::make_RPAREN(loc);
;                return yy::YYParser::make_SEMICOLON(loc);

.                {
                    throw yy::YYParser::syntax_error (loc, "illegal character: " + std::string(yytext));
                 }

<<EOF>>          return yy::YYParser::make_EOF(loc);

%%

namespace yy {
  
void
YYDriver::scan_begin ()
{
  yy_flex_debug = trace_scanning_;
  if (filename_.empty () || filename_ == "-") {
    yyin = stdin;
  }
  else if (!(yyin = fopen(filename_.c_str (), "r")))
  {
    std::cerr << "cannot open " << filename_ << ": " << strerror (errno) << '\n';
    exit (EXIT_FAILURE);
  }
}

void
YYDriver::scan_end ()
{
  fclose(yyin);
}

} // namespace yy