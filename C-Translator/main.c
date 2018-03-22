#include <stdio.h>
#include "lexer.h"





int main(int argc, char** argv)
{
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
