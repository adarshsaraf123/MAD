/*
 Purpose:to give the function declarations for handling scripts
 Written on: 04/03/2013
 Written by: Adarsh Saraf
*/

#ifndef _FILE_H
#define _FILE_H

void script(char *filename); // this is the main control function for scripts

void extract_lines(char *filename, char ***line, int *n); // this function is used to extract the line for execution from the script file, assigned dynamically

void exec_script(char **line, int n); // this is the function that actually executes the lines from the script which have been stored in the array of lines 'line'

int if_script(char **line, int n, int pos, int istrue); // to execute if statements in a file

int while_script(char **line, int n, int pos, int istrue); // to execute while statements in a file

int isempty_line(char *line); // to check if 'line' is empty

#endif
