//
//  main.c
//  c-translator
//
//  Created by Jakub Brehuv on 11/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//

#include <stdio.h>
#include "lexer.h"



int main(int argc, char** argv)
{
    int MAX_INPUT_SIZE = 10;
    // H_Term   = E VALUE | E ID | E LPAR;
    // H_Power  = H_Term;
    // H_Mul    = H_Power;
    // H_Expr   = H_Mul;
    // H_Print  = E PRINT;
    // H_Read   = E READ;
    
    // Citanie vstupneho retazca
    printf("Vstupny retazec: ");
    char source[MAX_INPUT_SIZE];
    fgets(source, MAX_INPUT_SIZE, stdin);
    init_lexer(source);
    print_tokens();
    
    
    // printf("\nZaciatok interpretacie\n");
    
    return 0;
}
