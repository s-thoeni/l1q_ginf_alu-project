/*
  printers.h

  printing cpu, registers, stack, memory
*/

#include <string.h>
#include <stdio.h>


//struct cpu_operator* getop();
void print1(char *reg);
void print_stack();
void print_registers();
void print_tester();
void printm();
void print_register_fetch1();
void print_register_exec();
void print_register_exec1();
