//
//  lexer.h
//  C_Translator
//
//  Created by Jakub Brehuv on 12/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//

#ifndef LEXER_H
#define LEXER_H

/* Size of identificators table */
#define LEX_IDS_MAX 20

/* Type of symbols - lexial units */
typedef enum {
    VAR, WHILE, IF, ELSE,
    READ, PRINT,
    COMMA, SEMICOLON,
    ID, VALUE,
    LPAR, RPAR, LCRLB, RCRLB,
    PLUS, MINUS, MUL, DIV, POWER,
    EQUALS,
    LESS, GREATER, LESSEQ, GREATEQ, EQTO, NOTEQ,
    SEOF, SERROR
} Symbol;

/* utput symbol of lexical analyze and its atribute.
 * If symbol is value, atribut contains its value,
 * if its ID, atribut contanins ID to identificators table. */
extern Symbol lex_symbol;
extern int lex_attr;

/* Table of identificators */
extern char *lex_ids[LEX_IDS_MAX];
extern int lex_ids_size; // Count of saved identificators

/* Initialization lex. analyzator. Parameter is input string. */
void init_lexer(char *string);

/* Next symbol reading.
 * Call set value of lex_symbol and lex_attr. */
void next_symbol(void);

/* Vrite all lexical numbers from input */
void print_tokens(void);

/* Name of lexical unit */
const char *symbol_name(Symbol symbol);

int get_error_position(void);

#endif /* LEXER_H */

