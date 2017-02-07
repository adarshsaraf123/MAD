/*
 Purpose: to write the main program for MAD which will use the functions defined in the other files
 Written on: 18/01/13
 Written by: Adarsh Saraf
*/

#include"definitions.h"
#include"parser.h"
#include"tasks.h"
#include"file.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define prmpt "\nMAD> "

extern int def_mat_cnt;//
extern int def_num_cnt;

extern matrix *mat_var_list;
extern number *num_var_list;

void prompt() // to print the prompt for MAD and get the command from the user to be passed on to execline()
 {
   char *line=NULL;
   size_t n=0;
   printf(prmpt);
   while(1)
    {
      getline(&line,&n,stdin);
      if(!isempty_line(line))
        break;
    }
   execline(line);
   free(line); // since line is allocated dynamically by getline
 }
 
int main(int argc, char *argv[])
{
 system("clear");
 printf("\n\n");
 mat_var_list=malloc(def_mat_cnt*sizeof(matrix));
 num_var_list=malloc(def_num_cnt*sizeof(number));
 init_help();
 if(argc==1)
  {
    while(1)
      prompt();
  }
 else
   script(argv[1]);
 printf("\n\n\n\n\n");
}
