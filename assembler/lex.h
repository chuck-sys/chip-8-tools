#ifndef LEX_H
#define LEX_H

#if ! defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif

#include "grammar.tab.h"
#include "location.hh"

namespace lex {
    class Lexer : public yyFlexLexer {
        public:
            Lexer(std::istream* in) : yyFlexLexer(in) {}
            virtual ~Lexer() {}

            using FlexLexer::yylex;
            virtual int yylex(
                    lex::Parser::semantic_type* const,
                    lex::Parser::location_type*);

        private:
            Parser::semantic_type* yylval = nullptr;
    };
}

#endif
