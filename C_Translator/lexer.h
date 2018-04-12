//
//  lexer.h
//  C_Translator
//
//  Created by Jakub Brehuv on 12/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//

#ifndef LEXER_H
#define LEXER_H

/* Velkost tabulky identifikatorov */
#define LEX_IDS_MAX 20

/* Typy symbolov - lexikalnych jednotiek */
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

/* Vystupny symbol lexikalnej analyzy a jeho atribut.
 * Ak symbol je VALUE, atribut obsahuje jeho celociselnu hodnotu,
 * ak je to ID, atribut obsahuje index do tabulky identifikatorov. */
extern Symbol lex_symbol;
extern int lex_attr;

/* Tabulka identifikatorov */
extern char *lex_ids[LEX_IDS_MAX];
extern int lex_ids_size; // Pocet ulozenych identifikatorov

/* Inicializacia lex. analyzatora. Parametrom je vstupny retazec. */
void init_lexer(char *string);

/* Precitanie dalsieho symbolu.
 * Volanie nastavi nove hodnoty lex_symbol a lex_attr. */
void next_symbol(void);

/* Vypis vsetky lexikalnych jednotiek zo vstupu */
void print_tokens(void);

/* Nazov lexikalnej jednotky */
const char *symbol_name(Symbol symbol);

#endif /* LEXER_H */

