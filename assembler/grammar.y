%skeleton "lalr1.cc"
%language "c++"
%require "3.0"

%defines
%define api.namespace {lex}
%define api.parser.class {Parser}

%code requires {
    #include <memory>
    #include <string>

    namespace lex {
        class Lexer;
    }

    class CodeGenerator;

    struct instr_arg_t {
        int token;
        union {
            int reg;
            int literal;
        };
    };

    struct instruction_t {
        int token;
        instr_arg_t arg1;
        instr_arg_t arg2;
        instr_arg_t arg3;
        std::string label;
    };

    #define YY_NULLPTR nullptr
}

%parse-param { Lexer &lexer }
%parse-param { CodeGenerator &gen }

%code{
    #include <iostream>
    #include <cctype>

    #include "assembler/Generator.h"
    #undef yylex
    #define yylex lexer.yylex

    #define EMPTYSTR std::string()
    #define SINGLE_INSTR(op) { \
        gen.insertInstruction( \
            instruction_t{op, {0, 0}, {0, 0}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define LABEL_INSTR(op, id) { \
        gen.insertInstruction( \
            instruction_t{op, {0, 0}, {0, 0}, {0, 0}, id} \
        ); \
    }

    #define REG_INSTR(op, reg) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg}, {0, 0}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define SPREG_LIT_INSTR(op, spreg, lit) { \
        gen.insertInstruction( \
            instruction_t{op, {spreg, 0}, {token::LITERAL, lit}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define SPREG_REG_INSTR(op, spreg, reg) { \
        gen.insertInstruction( \
            instruction_t{op, {spreg, 0}, {token::REGISTER, reg}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define REG_LIT_INSTR(op, reg, lit) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg}, {token::LITERAL, lit}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define REG_SPREG_INSTR(op, reg, spreg) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg}, {spreg, 0}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define REG_LABEL_INSTR(op, reg, label) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg}, {0, 0}, {0, 0}, label} \
        ); \
    }

    #define REG_REG_INSTR(op, reg1, reg2) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg1}, {token::REGISTER, reg2}, {0, 0}, EMPTYSTR} \
        ); \
    }

    #define REG_REG_LIT_INSTR(op, reg1, reg2, lit) { \
        gen.insertInstruction( \
            instruction_t{op, {token::REGISTER, reg1}, {token::REGISTER, reg2}, {token::LITERAL, lit}, EMPTYSTR} \
        ); \
    }

    #define DATA(lit) { \
        gen.insertInstruction( \
            instruction_t{token::LITERAL, {0, lit}, {0, 0}, {0, 0}, EMPTYSTR} \
        ); \
    }

    int string_to_regnum(std::string reg) {
        return std::stoul(reg.substr(1, 1), nullptr, 16);
    }
}

%define api.value.type variant
%define parse.assert
%define parse.error detailed

%token                  END     0       "end of file"
%token                  NEWLINE
%token                  COMMENT
%token <std::string>    IDENTIFIER
%token                  COLON
%token                  COMMA
%token <std::string>    REGISTER
%token <std::string>    LITERAL
%token                  UNKNOWN

%token                  IREG
%token                  DT
%token                  ST
%token                  K

%token                  CLS
%token                  RET
%token                  JP
%token                  CALL
%token                  SE
%token                  SNE
%token                  LD
%token                  ADD
%token                  OR
%token                  AND
%token                  XOR
%token                  SUB
%token                  SHR
%token                  SUBN
%token                  SHL
%token                  RND
%token                  DRW
%token                  SKP
%token                  SKNP
%token                  BCD
%token                  PUSH
%token                  POP

%nterm <int>            register
%nterm <int>            literal

%locations

%%

program : END | lines END;

lines
    : line
    | line lines
    ;

line
    : item NEWLINE
    | COMMENT
    | NEWLINE
    ;

item
    : CLS                           { SINGLE_INSTR(token::CLS); }
    | RET                           { SINGLE_INSTR(token::RET); }
    | JP IDENTIFIER                 { LABEL_INSTR(token::JP, $2); }
    | JP register COMMA IDENTIFIER  { REG_LABEL_INSTR(token::JP, $2, $4); }
    | CALL IDENTIFIER               { LABEL_INSTR(token::CALL, $2); }
    | SE register COMMA literal     { REG_LIT_INSTR(token::SE, $2, $4); }
    | SE register COMMA register    { REG_REG_INSTR(token::SE, $2, $4); }
    | SNE register COMMA literal    { REG_LIT_INSTR(token::SNE, $2, $4); }
    | SNE register COMMA register   { REG_REG_INSTR(token::SNE, $2, $4); }
    | LD register COMMA literal     { REG_LIT_INSTR(token::LD, $2, $4); }
    | LD register COMMA register    { REG_REG_INSTR(token::LD, $2, $4); }
    | LD IREG COMMA literal         { SPREG_LIT_INSTR(token::LD, token::IREG, $4); }
    | LD register COMMA DT          { REG_SPREG_INSTR(token::LD, $2, token::DT); }
    | LD register COMMA K           { REG_SPREG_INSTR(token::LD, $2, token::K); }
    | LD DT COMMA register          { SPREG_REG_INSTR(token::LD, token::DT, $4); }
    | LD ST COMMA register          { SPREG_REG_INSTR(token::LD, token::ST, $4); }
    | ADD register COMMA literal    { REG_LIT_INSTR(token::ADD, $2, $4); }
    | ADD register COMMA register   { REG_REG_INSTR(token::ADD, $2, $4); }
    | ADD IREG COMMA register       { SPREG_REG_INSTR(token::ADD, token::IREG, $4); }
    | SUB register COMMA register   { REG_REG_INSTR(token::SUB, $2, $4); }
    | SUBN register COMMA register  { REG_REG_INSTR(token::SUBN, $2, $4); }
    | OR register COMMA register    { REG_REG_INSTR(token::OR, $2, $4); }
    | AND register COMMA register   { REG_REG_INSTR(token::AND, $2, $4); }
    | XOR register COMMA register   { REG_REG_INSTR(token::XOR, $2, $4); }
    | SHR register                  { REG_INSTR(token::SHR, $2); }
    | SHL register                  { REG_INSTR(token::SHR, $2); }
    | RND register COMMA literal    { REG_LIT_INSTR(token::RND, $2, $4); }
    | DRW register COMMA register COMMA literal { REG_REG_LIT_INSTR(token::DRW, $2, $4, $6); }
    | SKP register                  { REG_INSTR(token::SKP, $2); }
    | SKNP register                 { REG_INSTR(token::SKNP, $2); }
    | BCD register                  { REG_INSTR(token::BCD, $2); }
    | PUSH register                 { REG_INSTR(token::PUSH, $2); }
    | POP register                  { REG_INSTR(token::POP, $2); }
    | IDENTIFIER COLON item         { gen.insertLabel($1); } // already inserted instruction
    | IDENTIFIER COLON              { gen.insertLabel($1); }
    | literals
    ;

literals
    : literal                       { DATA($1); }
    | literals literal              { DATA($2); }
    ;

register
    : REGISTER                      { $$ = string_to_regnum($1); }
    ;

literal
    : LITERAL                       { $$ = std::stoul($1, nullptr, 16); }

%%

void lex::Parser::error(const location_type &l, const std::string &msg) {
    std::cerr << "Error: " << msg << " at " << l << std::endl;
}
