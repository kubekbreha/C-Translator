//
//  generator.h
//  c-translator
//
//  Created by Jakub Brehuv on 11/04/2018.
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
void write_add(void);
void write_sub(void);
void write_mul(void);
void write_div(void);
void write_ask_var(short index, char *name);
void write_string(const char *str);

#endif /* GENERATOR_H */
