//
//  main.c
//  C_Translator
//
//  Created by Jakub Brehuv on 12/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//


#include "lexer.h"
#include "generator.h"

#include <stdio.h>
#include <string.h>

#define MAX_INPUT_SIZE 2048
#define BUFFER_SIZE 128

#define KeySet unsigned long int
#define E 1 <<

KeySet H_Operation;
KeySet H_Power;
KeySet H_Mul;
KeySet H_Addition;
KeySet H_Expr;
KeySet H_Assignment;
KeySet H_Looping;
KeySet H_Condition;
KeySet H_Statement;
KeySet H_Variable;
KeySet H_Program;

int error_counter = 0;
char error_string[MAX_INPUT_SIZE] = "";

void init_keysets() {
    H_Operation = E VALUE | E ID | E LPAR;
    H_Power = H_Operation;
    H_Mul = H_Power;
    H_Addition = H_Mul;
    H_Expr = H_Addition;
    H_Assignment = E ID;
    H_Looping = E WHILE;
    H_Condition = E IF;
    H_Statement = E READ | E PRINT | H_Assignment | H_Condition | H_Looping;
    H_Variable = E VAR;
    H_Program = H_Variable | H_Statement;
}

int variable(KeySet keySet);
void program(KeySet keySet),
statement(KeySet keySet),
condition(KeySet keySet),
looping(KeySet keySet),
assignment(KeySet keySet),
expr(KeySet keySet),
addition(KeySet keySet),
mul(KeySet keySet),
power(KeySet keySet),
operation(KeySet keySet);

void error(const char *msg, KeySet K) {
    fprintf(stderr, "ERROR: %s\n", msg);
    error_counter++;
    while (!(E lex_symbol & K))
        next_symbol();
    
    int err_pos = get_error_position();
    printf("Error on %d position.\n", err_pos);
    error_string[err_pos-1] = 94;
}

void check(const char *msg, KeySet K) {
    if (!(E lex_symbol & K)){
        error(msg, K);
    }
}


int match(const Symbol expected, KeySet K) {

    if (lex_symbol == expected) {
        int attr = lex_attr;
        next_symbol();
        return attr;
    } else {
        char *msg = malloc(100);
        snprintf(msg, 100, "Expected symbol %s, instead %s occurs",
                 symbol_name(expected), symbol_name(lex_symbol));
        error(msg, K);
        return 0;
    }
}


void program(KeySet keySet) {
    int variables_counter = 0;
    
    KeySet allKeys = keySet | H_Variable | H_Statement;
    check("PR1", allKeys);
    
    while (lex_symbol == VAR) {
        variables_counter += variable(allKeys);
        
        check("PR2", allKeys);
        

    }
    
    write_begin(variables_counter);
    
    check("PR3", keySet | H_Statement);
    
    while (lex_symbol != SEOF && lex_symbol != SERROR) {
        statement(keySet | H_Statement);
        check("PR4", keySet | H_Statement);
    }
    
    write_end();
}


int variable(KeySet keySet) {
    int counter = 1;
    
    KeySet allKeys = keySet | E ID | E COMMA | E SEMICOLON;
    
    match(VAR, allKeys);
    match(ID, allKeys);
    
    check("VAR1", allKeys);
    
    while ((E lex_symbol) & (E COMMA | E ID)) {
        match(COMMA, allKeys);
        match(ID, allKeys);
        
        ++counter;
        check ("VAR2", allKeys);
    }
    
    match(SEMICOLON, keySet);
    
    return counter;
}


void statement(KeySet keySet) {
    check ("STA1", E READ | E PRINT | H_Condition | H_Looping | H_Assignment);
    
    if (lex_symbol == READ) {
        match(READ, keySet | E ID | E SEMICOLON);
        
        int idIndex = match(ID, keySet | E SEMICOLON);
        write_ask_var(idIndex, lex_ids[idIndex]);
        
        match(SEMICOLON, keySet);
    } else if (lex_symbol == PRINT) {
        match(PRINT, keySet | H_Expr | E SEMICOLON);
        expr(keySet | E SEMICOLON);
        
        write_result();
        
        match(SEMICOLON, keySet);
    } else if (lex_symbol == IF) {
        condition(keySet);
    } else if (lex_symbol == WHILE) {
        looping(keySet);
    } else if (lex_symbol == ID) {
        assignment(keySet | E SEMICOLON);
        
        match(SEMICOLON, keySet);
    } else {
        error("STA2", keySet);
    }
}


void condition(KeySet keySet) {
    match(IF, keySet | H_Expr | E LCRLB | H_Statement | E RCRLB | E ELSE);
    expr(keySet | E LCRLB | H_Statement | E RCRLB | E ELSE);
    
    write_branch_zero_to(0);
    short conditionJmpAddr = get_address() - 1;
    
    match(LCRLB, keySet | E LCRLB | H_Statement | E RCRLB | E ELSE);
    
    KeySet allKeys = keySet | E LCRLB | H_Statement | E RCRLB | E ELSE;
    
    check("CON1", allKeys);
    
    while ((E lex_symbol) & (H_Statement)) {
        statement(allKeys);
        
        check("CON2", allKeys);
    }
    
    match(RCRLB, allKeys);
    
    write_jump_to(get_address() + 2);
    short jmpToEndAddr = get_address() - 1;
    
    modify_value_on_address(conditionJmpAddr, get_address());
    
    if ((E lex_symbol) & (E ELSE | E LCRLB)) {
        match(ELSE, keySet | E LCRLB | H_Statement | E RCRLB);
        
        match(LCRLB, keySet | H_Statement | E RCRLB);
        
        check("CON3", keySet | H_Statement | E RCRLB);
        
        while ((E lex_symbol) & (H_Statement)) {
            statement(keySet | H_Statement | E RCRLB);
            
            check("CON4", keySet | H_Statement | E RCRLB);
        }
        
        match(RCRLB, keySet);
        
        modify_value_on_address(jmpToEndAddr, get_address());
    }
}

void looping(KeySet keySet) {
    match(WHILE, keySet | H_Expr | E LCRLB | H_Statement | E RCRLB);
    
    short conditionAddr = get_address();
    
    expr(keySet | E LCRLB | H_Statement | E RCRLB);
    
    write_branch_zero_to(0);
    short jmpAddr = get_address() - 1;
    
    match(LCRLB, keySet | H_Statement | E RCRLB);
    
    check("LOP1", keySet | H_Statement | E RCRLB);
    
    while ((E lex_symbol) & (H_Statement)) {
        statement(keySet | E RCRLB);
        
        check("LOP2", keySet | H_Statement | E RCRLB);
    }
    
    match(RCRLB, keySet);
    
    write_jump_to(conditionAddr);
    
    modify_value_on_address(jmpAddr, get_address());
}

/* Assignment -> ID = Expr */
void assignment(KeySet keySet) {
    int idIndex = match(ID, keySet | E EQUALS | H_Expr);
    
    match(EQUALS, keySet | H_Expr);
    
    expr(keySet);
    
    write_assignment(idIndex);
}

/* Expr -> Addition [ ( ">" | "<" | ">=" | "<=" | "==" | "!=" ) Addition ] */
void expr(KeySet keySet) {
    KeySet operationKeys = E LESS | E GREATER | E LESSEQ | E GREATEQ | E EQTO | E NOTEQ;
    
    addition(keySet | operationKeys | H_Addition);
    
    void (*write_fn)(void) = NULL;
    
    check("EXP1", keySet | operationKeys | H_Addition);
    
    if (!((E lex_symbol) & operationKeys)) return;
    
    switch(lex_symbol) {
        case GREATER:
            match(GREATER, keySet | H_Addition);
            write_fn = write_greater;
            break;
        case LESS:
            match(LESS, keySet | H_Addition);
            write_fn = write_less;
            break;
        case GREATEQ:
            match(GREATEQ, keySet | H_Addition);
            write_fn = write_greater_equal;
            break;
        case LESSEQ:
            match(LESSEQ, keySet | H_Addition);
            write_fn = write_less_equal;
            break;
        case EQTO:
            match(EQTO, keySet | H_Addition);
            write_fn = write_equals_to;
            break;
        case NOTEQ:
            match(NOTEQ, keySet | H_Addition);
            write_fn = write_not_equals_to;
            break;
        default:
            error("Expected comparison operation", keySet | H_Addition);
            break;
    }
    
    addition(keySet);
    write_fn();
}


void addition(KeySet keySet) {
    KeySet operationKeys = E PLUS | E MINUS;
    KeySet allKeys = keySet | operationKeys | H_Mul;
    
    mul(allKeys);
    
    void (*write_fn)(void) = NULL;
    
    check("ADD1", allKeys);
    
    while ((E lex_symbol) & (operationKeys)) {
        switch (lex_symbol) {
            case PLUS:
                match(PLUS, keySet | H_Mul);
                write_fn = write_add;
                break;
            case MINUS:
                match(MINUS, keySet | H_Mul);
                write_fn = write_sub;
                break;
            default:
                error("Expected operation + or -", keySet | H_Mul);
                return;
        }
        
        mul(allKeys);
        write_fn();
        
        check("ADD2", allKeys);
    }
}

void mul(KeySet keySet) {
    KeySet operationKeys = E MUL | E DIV;
    KeySet allKeys = keySet | operationKeys | H_Power;
    
    power(allKeys);
    
    void (*write_fn)(void) = NULL;
    
    check("POW1", allKeys);
    
    while ((E lex_symbol) & (operationKeys)) {
        switch (lex_symbol) {
            case MUL:
                match(MUL, keySet);
                write_fn = write_mul;
                break;
            case DIV:
                match(DIV, keySet);
                write_fn = write_div;
                break;
            default:
                error("Expected operation * or /", keySet | H_Power);
                return;
        }
        
        power(allKeys);
        write_fn();
        
        check("POW2", allKeys);
    }
}


void power(KeySet keySet) {
    operation(keySet | E POWER | H_Power);
    
    if (lex_symbol == POWER) {
        match(POWER, keySet | H_Power);
        
        power(keySet);
        
        write_power();
    }
}



void operation(KeySet keySet) {
    switch (lex_symbol) {
        case VALUE: {
            int value = match(VALUE, keySet);
            write_number(value);
            
            break;
        }
        case ID: {
            int idIndex = match(ID, keySet);
            write_var(idIndex);
            
            break;
        }
        case LPAR:
            match(LPAR, keySet | H_Expr | E RPAR);
            
            expr(keySet | E RPAR);
            
            match(RPAR, keySet);
            break;
        default:
            error("Expected const, variable or (", keySet);
            break;
    }
}

FILE* open_file(char *file_name, char* access) {
    FILE* file = fopen(file_name, access);
    if (file == NULL) {
        fprintf(stderr, "Error occured while trying to open %s file for writing.\n", file_name);
        exit(1);
    }
    return file;
}

int main (int argc, char **argv) {
    
    for(int i = 0; i < MAX_INPUT_SIZE; i++){
        error_string[i] = ' ';
    }
    init_keysets();
    
    //char source[MAX_INPUT_SIZE] = "woor a;woor b;woi(b>=10){woop(aˆb);}woe{woop(aˆb);}";
    //char source[MAX_INPUT_SIZE] = "woor fer; woooo(fer != 5){woop( +5); fer = fer + 1;}";
    
    char source[MAX_INPUT_SIZE] = "woope(helloworld)";
    
    //char source[MAX_INPUT_SIZE] = "woop(3ˆ3);";
    
    init_lexer(source);
    //print_tokens();
    
    FILE* output_file = open_file("program.bin", "wb");
    init_generator(output_file);
    
    next_symbol();
    program(E SEOF);
    if (lex_symbol != SEOF) {
        error("Ocakavany je koniec suboru", E SEOF);
    }
    
    generate_output();
    
    fclose(output_file);
    printf("This program compiled with %d errors.\n", error_counter);
    
    printf("This is user input: \n");
    printf("%s\n", source);
    printf("%s\n", error_string);
    
    return 0;
}
