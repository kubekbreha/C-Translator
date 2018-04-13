//
//  lexer.c
//  C_Translator
//
//  Created by Jakub Brehuv on 12/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

/* Symboles name */
const char *SYM_NAMES[] = {
    [VAR] = "VAR",
    [WHILE] = "WHILE",
    [IF] = "IF",
    [ELSE] = "ELSE",
    [READ]="READ",
    [PRINT]="PRINT",
    [COMMA]="COMMA",
    [SEMICOLON]="SEMICOLON",
    [ID]="ID",
    [VALUE]="VALUE",
    [LPAR]="LPAR",
    [RPAR]="RPAR",
    [LCRLB] = "LCRLB",
    [RCRLB] = "RCRLB",
    [PLUS]="PLUS",
    [MINUS]="MINUS",
    [MUL]="MUL",
    [DIV]="DIV",
    [POWER]="POWER",
    [EQUALS]="EQUALS",
    [LESS]="LESS",
    [GREATER]="GREATER",
    [LESSEQ] = "LESSEQ",
    [GREATEQ] = "GREATEQ",
    [EQTO]="EQTO",
    [NOTEQ]="NOTEQ",
    [SEOF]="SEOF",
    [SERROR]="SERROR",
};



/* Global variables, "public" */
Symbol lex_symbol;
int lex_attr;
int error_position = 0;


char *lex_ids[LEX_IDS_MAX];
int lex_ids_size; // Number of saved identivicators


/* Input variables */
static char *input;     // Input string
static char c;          // Proccesed input char
static int ic;          // Index of next char in string


/* Initialization lex. analyzator. Parameter is input string. */
void init_lexer(char *string)
{
    input = string;
    ic = 0;
    lex_ids_size = 0;
}


/* Identificator saving `id` to table with identificatrs if already isnt there.
 * Return index on which identificator is saved. */
int store_id(char *id) {
    int i = 0;
    while (i < lex_ids_size) {
        if (strcmp(id, lex_ids[i]) == 0)
            return i;
        i++;
    }
    lex_ids[i] = strdup(id);
    lex_ids_size++;
    return i;
}

int get_error_position(){
    return error_position;
}



/* Read next char
   Call set new valuest to lex_symbol and lex_attr. */
void next_symbol()
{

    c = input[ic++];
    error_position = ic;
    while (isspace(c)) { // Skip spaces
        c = input[ic++];
    }
    
    switch (c) {
        case ',':  lex_symbol = COMMA;      break;
        case '+':  lex_symbol = PLUS;       break;
        case '-':  lex_symbol = MINUS;      break;
        case '*':  lex_symbol = MUL;        break;
        case '/':  lex_symbol = DIV;        break;
        case '^':  lex_symbol = POWER;      break;
        case '(':  lex_symbol = LPAR;       break;
        case ')':  lex_symbol = RPAR;       break;
        case '<':
            c = input[ic++];
            if (c == '=') {
                lex_symbol = LESSEQ;
            } else {
                lex_symbol = LESS;
                --ic;
            }
            break;
        case '>':
            c = input[ic++];
            if (c == '=') {
                lex_symbol = GREATEQ;
            } else {
                lex_symbol = GREATER;
                --ic;
            }
            break;
        case '=':
            c = input[ic++];
            if (c == '=') {
                lex_symbol = EQTO;
            } else {
                lex_symbol = EQUALS;
                --ic;
            }
            break;
        case '!':
            c = input[ic++];
            if (c == '=') {
                lex_symbol = NOTEQ;
            }
            break;
        case ';':  lex_symbol = SEMICOLON;  break;
        case '{':  lex_symbol = LCRLB;      break;
        case '}':  lex_symbol = RCRLB;      break;
        case '\0': lex_symbol = SEOF;       break; // End of string
        default:
            if (isdigit(c)) {
                lex_symbol = VALUE;
                lex_attr = 0;
                
                do {
                    lex_attr *= 10;
                    lex_attr += (c - '0');
                    
                    c = input[ic++];
                } while (isdigit(c));
                ic--;
            } else if (isalpha(c)) {
                int id_start = ic - 1; // Index od identificator begining
                do {
                    c = input[ic];
                    ic++;
                } while (isalnum(c));
                ic--; // "Return" last char
                
                // Copy identificator
                int id_len = ic - id_start;
                char *id = malloc(id_len + 1);
                memcpy(id, &input[id_start], id_len);
                id[id_len] = 0;
                
                // Check of keywords
                if (!strcmp(id, "woor")) {                  //read
                    lex_symbol = READ;
                } else if (!strcmp(id, "woop")) {           //write
                    lex_symbol = PRINT;
                } else if (!strcmp(id, "w")) {              //war
                    lex_symbol = VAR;
                } else if (!strcmp(id, "woooo")) {          //while
                    lex_symbol = WHILE;
                } else if (!strcmp(id, "woi")) {            //if
                    lex_symbol = IF;
                } else if (!strcmp(id, "woe")) {            //else
                    lex_symbol = ELSE;
                } else { // Saving to identificator table
                    lex_attr = store_id(id);
                    lex_symbol = ID;
                }
                free(id);
            } else {
                lex_symbol = SERROR;
            }
    }
}


/* Name of lexical unit */
const char *symbol_name(Symbol symbol)
{
    return SYM_NAMES[symbol];
}


/* Write all lexical units from input */
void print_tokens()
{
    printf("\nOutput of lexical analyse (string of symbols)\n");
    do {
        next_symbol();
        printf("  [%2d] %s", lex_symbol, symbol_name(lex_symbol));
        if (lex_symbol == VALUE) printf(" <%d>", lex_attr);
        if (lex_symbol == ID) printf(" <%d> -> %s", lex_attr, lex_ids[lex_attr]);
        printf("\n");
    } while (lex_symbol != SEOF);
}
