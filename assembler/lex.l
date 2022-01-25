%{
#include <string>

#include "assembler/lex.h"
#undef YY_DECL
#define YY_DECL int lex::Lexer::yylex(lex::Parser::semantic_type* const lval, lex::Parser::location_type* loc)

using token = lex::Parser::token;

#define yyterminate() return (token::END)
%}

HEX             [[:digit:]a-f]
IDENTIFIER      [[:alpha:]_][[:alpha:]_[:digit:]]*

%option case-insensitive
%option noyywrap
%option yylineno
%option c++
%option yyclass="lex::Lexer"

%%
%{
    yylval = lval;
%}

";".*\n                 return token::COMMENT;
"0x"{HEX}{1,4}          {
    yylval->build<std::string>(yytext);
    return token::LITERAL;
}
"v"{HEX}                {
    yylval->build<std::string>(yytext);
    return token::REGISTER;
}

CLS                     return token::CLS;
RET                     return token::RET;
JP                      return token::JP;
CALL                    return token::CALL;
SE                      return token::SE;
SNE                     return token::SNE;
LD                      return token::LD;
ADD                     return token::ADD;
OR                      return token::OR;
AND                     return token::AND;
XOR                     return token::XOR;
SUB                     return token::SUB;
SHR                     return token::SHR;
SUBN                    return token::SUBN;
SHL                     return token::SHL;
RND                     return token::RND;
DRW                     return token::DRW;
SKP                     return token::SKP;
SKNP                    return token::SKNP;
BCD                     return token::BCD;
PUSH                    return token::PUSH;
POP                     return token::POP;

i                       return token::IREG;
dt                      return token::DT;
st                      return token::ST;
":"                     return token::COLON;
{IDENTIFIER}            {
    yylval->build<std::string>(yytext);
    return token::IDENTIFIER;
}


\n                      return token::NEWLINE;
","                     return token::COMMA;
[ \t]+                  // ignore all spaces/tabs
.                       return token::UNKNOWN;

%%
