/*
 Purpose:to give the function definitions for handling scripts
 Written on: 04/03/2013
 Written by: Adarsh Saraf
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"file.h"

void script(char *filename) // this is the main control function for scripts
{
  char **line;
  int n;
  extract_lines(filename, &line, &n);
  if(n!=-1)
    exec_script(line, n); // now, once the lines have been obtained we start executing them 
 }
 
void extract_lines(char *filename, char ***line, int *line_n) // this function is used to extract the line for execution from the script file, assigned dynamically
 {
   FILE *fp=fopen(filename,"r");
   if(fp==NULL) 
    {
      printf("The file could not be opened!!!\n\n\n\n\n");
      *line_n=-1;
      return;
    }
   char ch;
   int cnt=0, empty=1; // 'empty' is a check for whether the current line is empty; line is not empty if it contains any non-space character
   while(1)
    {
      ch=getc(fp);
      if(ch==EOF) break;
      else if(!isspace(ch))
        empty=0;
      else if(ch=='\n' || ch=='#')
       {
         if(!empty)
          cnt++;
         if(ch=='#') // to skip all the characters until the appearance of a newline
          {
            while(ch!='\n')
              ch=getc(fp);   
          } 
         empty=1;
       }
    }
   
   // now cnt is the total number of executable lines in the file
     
   int cur=0;
   size_t n=0;
   *line=malloc(cnt*sizeof(char *));
   int i;
   fclose(fp);
   fp=fopen(filename,"r"); // since the file has to be read from the beginning once more
   char *temp_line;
   while(cur<cnt)
    {
      if(feof(fp))
        break;
      n=0;
      getline(&(*line)[cur],&n,fp);
      // now check for the presence of comments
      if(strstr((*line)[cur],"#"))
       {
         temp_line=malloc(strlen((*line)[cur])*sizeof(char));
         for(i=0;(*line)[cur][i]!='#';i++)
           temp_line[i]=(*line)[cur][i];
         temp_line[i]='\0';
         free((*line)[cur]);
         (*line)[cur]=temp_line;
       }
      if(!isempty_line((*line)[cur]))
        cur++;
      else
        free((*line)[cur]); // since line is allocated dynamically by getline
    }
   if(cur!=cnt)
     cnt=cur;
   (*line_n)=cnt; 
 }
 
void exec_script(char **line, int n) // this is the function that actually executes the lines from the script which have been stored in the array of lines 'line'
 {
   int i;
   int check;
   for(i=0;i<n;)
    {
      check=execline(line[i]);
      switch(check)
       {
         case 10:
           {
             i=if_script(line,n,i+1,0);
             break;
           }
         case 11:
           {
             i=if_script(line,n,i+1,1);
             break;
           }
         case 20:
           {
             i=while_script(line,n,i+1,0);
             break;
           }
         case 21:
           {
             i=while_script(line,n,i+1,1);
             break;
           }
         case 0:
           { 
             i++;
             break;
           }
       }
    }
 }

int if_script(char **line, int n, int pos, int istrue) // to execute if statements in a file, returns the line to be executed after the end of the if statements
 {
   int cnt_braces;
   int i,check; 
   
   if(istrue)
    {
      i=pos;
      cnt_braces=0;
      if(strstr(line[i],"{"))  // since this if's block is worried only about line immediately after if
           cnt_braces++;
      do // do while loop since atleast the first sentence after the loop has to be executed
       {
         if(strstr(line[i],"}"))
           cnt_braces--; // you don't have to check for the presence of another '{' because that can occur only coupled with if's(or else's) and while's which are taken care of during the execution of the following lines
    
         check=execline(line[i]);
         switch(check)
          {
            case 10:
              {
                i=if_script(line,n,i+1,0); //since if_script returns the next line to be executed after the execution of the if condition
                break;
              }
            case 11:
              {
                i=if_script(line,n,i+1,1);//since if_script returns the next line to be executed after the execution of the if condition
                break;
              }
            case 20:
              {
                i=while_script(line,n,i+1,0);
                break;
              }
            case 21:
              {
                i=while_script(line,n,i+1,1);
                break;
              }
            case 0:
              { 
                i++;
                break;
              }
          } // end of switch   
    
       }while((cnt_braces!=0 && i<n));//end of while
    
      // execution of the if is over, we now have to skip the else part and return the line number after else gets over; i is set to the line after the if block
     while(i<n && strstr(line[i],"else"))
      {
        i++;// for skipping the else line
        cnt_braces=0;
        do
         {
           if(strstr(line[i],"{"))
             cnt_braces++;
           if(strstr(line[i],"}"))
             cnt_braces--;
           i++;
         }while(cnt_braces!=0 && i<n);
      }
     return i; 
    }//end of if
  
  else
   {
     // first skip the lines pertaining to the if block
     i=pos;
     if(i<n)
      {
        cnt_braces=0;
        do
         {
           if(strstr(line[i],"{"))
             cnt_braces++;
           if(strstr(line[i],"}"))
             cnt_braces--;
           i++;
         }while(cnt_braces!=0 && i<n);
      }// now we have skipped the code pertaining to the if block
     
     // to start the execution of the else block
     if(i<n&&strstr(line[i],"else"))
      {
        if(strstr(line[i],"if")) // means it is an else if condition
         { 
           check=execline(line[i]);
           if(check==10)
             i=if_script(line,n,i+1,0); //since if_script returns the next line to be executed after the execution of the if condition
           else if(check==11)
             i=if_script(line,n,i+1,1); //since if_script returns the next line to be executed after the execution of the if condition
         }
       else // means it is not an else if condition, but only an else condition
         {  
           cnt_braces=0;
           i++; // to skip the line containing else
           if(strstr(line[i],"{"))
                cnt_braces++;
           do // to execute the else block
            {
              if(strstr(line[i],"}")) 
                cnt_braces--; // you don't have to check for the presence of another '{' because that can occur only coupled with if's(or else's) and while's which are taken care of during the execution of the following lines
              check=execline(line[i]);
              switch(check)
               {
                 case 10:
                   {
                     i=if_script(line,n,i+1,0); //since if_script returns the next line to be executed after the execution of the if condition
                     break;
                   }
                 case 11:
                   {
                     i=if_script(line,n,i+1,1);//since if_script returns the next line to be executed after the execution of the if condition
                     break;
                   }
                 case 20:
                   {
                     i=while_script(line,n,i+1,0);
                     break;
                   }
                 case 21:
                   {
                     i=while_script(line,n,i+1,1);
                     break;
                   }
                 case 0:
                   { 
                     i++;
                     break;
                   }
               } // end of switch   
            
            }while(cnt_braces!=0 && i<n);
         
         }// end of else for if(strstr(line[i],"if"))    
      
      }// end of if(i<n&&strstr(line[i],"else")) 
     
     return i; // since i is now the line to be executed after the execution of the else block, if at all there was one 
   
   }// end of else for if(istrue)

 }

int while_script(char **line, int n, int pos, int istrue) // to execute while statements in a file, returns the line to be executed after the end of the while statements
 {
   int cnt_braces;
   int i,check,end; 
   if(!istrue)
    {  
      i=pos;
      cnt_braces=0;
      do
       {
         if(strstr(line[i],"{"))
           cnt_braces++;
         if(strstr(line[i],"}"))
           cnt_braces--;
         i++;
       }while(cnt_braces!=0 && i<n);
     return i; 
    }
    
   else
    {
      while(istrue)
       {
         i=pos;
         cnt_braces=0;
         if(strstr(line[i],"{"))  // since this if's block is worried only about line immediately after if
           cnt_braces++;
         do // do while loop since atleast the first sentence after the loop has to be executed
          {
            if(strstr(line[i],"}"))
              cnt_braces--; // you don't have to check for the presence of another '{' because that can occur only coupled with if's(or else's) and while's which are taken care of during the execution of the following lines
    
            check=execline(line[i]);
            switch(check)
             {
               case 10:
                 {
                   i=if_script(line,n,i+1,0); //since if_script returns the next line to be executed after the execution of the if condition
                   break;
                 }
               case 11:
                 {
                   i=if_script(line,n,i+1,1);//since if_script returns the next line to be executed after the execution of the if condition
                   break;
                 }
               case 20:
                 {
                   i=while_script(line,n,i+1,0);
                   break;
                 }
               case 21:
                 {
                   i=while_script(line,n,i+1,1);
                   break;
                 }
               case 0:
                 { 
                   i++;
                   break;
                 }
             } // end of switch   
    
          }while((cnt_braces!=0 && i<n));//end of while
         end=i;
         istrue=execline(line[pos-1]); // since the while condition has to be checked for
         istrue=istrue%10; // since istrue recieves 20 or 21 of which we want only 0 and 1
       }
     return end;
   }   // execution of the while is over
 }


int isempty_line(char *line) // to check if 'line' is empty
 {
   int i=0;
   int empty=1;
   while(line[i]!='\0')
    {
      if(!isspace(line[i]))
        empty=0;
      i++;
    }
   return empty;
 }


