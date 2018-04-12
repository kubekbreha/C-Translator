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

void generate_output(void);
short get_address(void);

void write_begin(short num_vars);
void write_end(void);

void write_result(void);
void write_number(short value);
void write_var(short index);
void write_string(const char *str);

void write_ask_var(short index, char *name);

void write_assignment(short index);

void write_add(void);
void write_sub(void);
void write_mul(void);
void write_div(void);
void write_power(void);

void write_greater(void);
void write_less(void);
void write_greater_equal(void);
void write_less_equal(void);
void write_equals_to(void);
void write_not_equals_to(void);

void write_jump_to(short address);
void write_branch_zero_to(short address);

void modify_value_on_address(short address, short value);

#endif /* GENERATOR_H */
