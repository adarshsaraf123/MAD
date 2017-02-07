/*
 Purpose:to create the parser for MAD
 Written on: 16/01/13
 Written by: Adarsh Saraf
*/

#ifndef _PARSER_H
#define _PARSER_H
#include"definitions.h"

void prompt();

int execline(char *line); //to execute the command specified in line

void infix_to_postfix(char ***argv, int *argc); // to convert the given infix expression in the shell stored in argv to postfix

void parse(char *line, char ***argv, int *argc); // to parse the command contained in line and return the words in argv and their count in argc, both passed by reference

int isseparator(char c); // to check if c is a separator; return 0 if not, else return the number of words it contributes to the line

token istask(char *word); // to check if word is a task

int isvalid_identifier(char *word); //to check if word is a valid identifier

int isvalid_number(char *word); //to check if word is a valid number

int isused_identifier(char *word); //to check if the identifier word has already been used

void init_help(); // to initialise the help for each of the keywords/operators used; called in main() at the beginning of the execution of the program

#endif
