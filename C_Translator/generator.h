//
//  generator.h
//  C_Translator
//
//  Created by Jakub Brehuv on 12/04/2018.
//  Copyright © 2018 Jakub Brehuv. All rights reserved.
//

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

void init_generator(FILE *output);

void generate_output();
short get_address();

void write_begin(short num_vars);
void write_end();

void write_result();
void write_number(short value);
void write_var(short index);
void write_string(const char *str);

void write_ask_var(short index, char *name);

void write_assignment(short index);

void write_add();
void write_sub();
void write_mul();
void write_div();
void write_power();

void write_greater();
void write_less();
void write_greater_equal();
void write_less_equal();
void write_equals_to();
void write_not_equals_to();

void write_jump_to(short address);
void write_branch_zero_to(short address);

void modify_value_on_address(short address, short value);

#endif /* GENERATOR_H */
