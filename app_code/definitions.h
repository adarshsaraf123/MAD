/*
 Purpose:to define all the variables and provide declarations for the basic functions of MAD
 Written on:21/01/13
 Written by: Adarsh Saraf
*/

#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

enum task{na=0,add, sub, mult, divide, equal, less, great, lesse, greate, equality, nequality, not, and, or, clear, del, det, disp, close, if_c, else_c, left_b, right_b, inp, help, num, run, leftp, mat, rightp, while_c};
typedef enum task task;

typedef struct token
 {
   char *name;
   task value;
   int precedence;
   int num_operands;
   char *help;
 }token, operation;

typedef struct matrix
 {
   char *name;
   int m;
   int n;
   double **a;
   int *row_indices;
   int *col_indices;
   int row_cnt;
   int col_cnt;
 }matrix;

typedef struct number
 {
   char *name;
   double value;
 }number;


#endif
