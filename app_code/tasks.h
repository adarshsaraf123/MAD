/*
 Purpose: to define the functions to carry out the various tasks for MAD
 Written on: 25/01/13
 Written by: Adarsh Saraf
*/

#ifndef _TASKS_H
#define _TASKS_H
#include"definitions.h"
#include"parser.h"

// any operation_task() is to execute operation

char *add_task(char *,char *); 
char *sub_task(char *,char *);
char *mult_task(char *,char *);
char *divide_task(char *,char *);
char *equal_task(char *,char *);
char *adj_task(char *); 
//char *col_task(char *,char *);
char *det_task(char *);
char *disp_task(char **, int);
void disp_func(char *);
char *inp_task(char *);
char *del_task(char *operand); // to delete the operand
char *inv_task(char *);
int if_task(char *);
char *less_task(char *,char *);
char *lesse_task(char *,char *);
char *great_task(char *,char *);
char *greate_task(char *,char *);
char *equality_task(char *,char *);
char *nequality_task(char *,char *);
char *not_task(char *);
char *and_task(char *,char *);
char *or_task(char *,char *);
char *lar_task(char *);
char *mat_task(char **, int);
void mat_func(char *);
char *num_task(char **, int);
void num_func(char *);
//char *row_task(char *,char *);
char *sml_task(char *);

///////////////////////////////////



void init_mat_var(char *, int ,int); // to initialise a matrix variable with identifier name as 'name' and size 'm' by 'n'

void init_mat_temp(matrix **mat, int m, int n); // to initialise the variables in a temporary matrix created by the program for intermediate operations

void input_matrix(matrix m); // to assign the values supplied by the user m3

void addition_matrices(matrix m1, matrix m2, matrix m3); // to add the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)

void addition_num_mat(matrix m1, number n, matrix m2); // to add to the matrix m1 the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)
 
void subtraction_matrices(matrix m1, matrix m2, matrix m3); // to subtract the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)

void subtraction_num_mat(matrix m1, number n, matrix m2); // to subtract from the matrix m1 the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)

void multiplication_matrices(matrix m1, matrix m2, matrix m3); // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)

void multiplication_num_mat(matrix m1, number n, matrix m2); // to multiply each element of the matrix m1 by the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)

void division_matrices(matrix m1, matrix m2, matrix m3); // to divide the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)

void division_num_mat(matrix m1, number n, matrix m2); // to divide each element of the matrix m1 by the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)


void equate_matrices(matrix m1, matrix m2); // to equate the matrices m1 with m2 (value gets reflected since m3.a is same in the calling function and here)

void equate_num_mat(matrix m, number n); // to equate each element of the matrix m the number n (value gets reflected since m3.a is same in the calling function and here)

int value_operand(char *); // this is used to give the value of the operand for use in conditional statements

int less_matrices(matrix m1, matrix m2); // to 
int less_mat_num(matrix m, number n); // to 

int lesse_matrices(matrix m1, matrix m2); // to 
int lesse_mat_num(matrix m, number n); // to 

int great_matrices(matrix m1, matrix m2); // to 
int great_mat_num(matrix m, number n); // to 

int greate_matrices(matrix m1, matrix m2); // to 
int greate_mat_num(matrix m, number n); // to 

int equality_matrices(matrix m1, matrix m2); // to 
int equality_mat_num(matrix m, number n); // to 

void delete_matrix(matrix *m); // to delete the matrix

void print_matrix(matrix m); // to print the matrix m

int get_operands2(char * operand1, char *operand2, number **n1, number **n2, matrix **m1, matrix **m2); // to get the operands for binary operators and store them in the respective variables  

int get_operands1(char *operand, number **n, matrix **m); // to get the operand for unary operators and store them in the respective variable  

void assign_indices(char *operand, matrix *m); //to assign the indices for rows and columns which are to be used for the operations into row_indices and col_indices respectively

void write_num(char **word, number n); // to write the value of 'n' into 'word'

double determinant_matrix(matrix m); // to return the determinant of matrix m 
/*
int smallest_matrix(matrix m); // to return the smallest element in the matrix m

int largest_matrix(matrix m); // to return the largest element in the matrix m

char *column_matrix(matrix m, int col_num); // to extract the col_num column from the matrix m and store the result in the temporary variable whose name it returns as a string pointer

char *row_matrix(matrix m, int row_num); // to extract the row_num row from the matrix m and store the result in the temporary variable whose name it returns as a string pointer 

char *inverse_matrix(matrix m); // to add the matrices m1 and m2 and store the result in the temporary variable whose name it returns as a string pointer */

int cnt_nondecimal(double d); // to return the number of digits before the decimal point in d
int cnt_decimal(double d); // to return the number of digits after the decimal point in d

#endif
