/*
 Purpose:to create the parser for MAD
 Written on: 16/01/13
 Written by: Adarsh Saraf
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"parser.h"
#include"tasks.h"
#include"file.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define cnt_keywords 32
#define cnt_seps 15

#define printargv() { for (i=0;i<argc;i++)  printf("%s ",argv[i]);   printf("\n\n");  }

token keywords_list[cnt_keywords]={{"not an operation",na,0,0},{"+",add,1,2},{"-",sub,1,2},{"*",mult,2,2},{"/",divide,2,2},{"=",equal,0,2}, {"<",less,0,2}, {">", great,0,2}, {"<=",lesse,0,2}, {">=",greate,0,2}, {"==",equality,0,2}, {"!=",nequality,0,2},{"!",not,-1,1}, {"&",and,-3,2}, {"|",or,-3,2},{"clear",clear,20,0}, {"del",del,10,0}, {"det",det,3,1}, {"disp",disp,-5,0}, {"exit",close,11,0}, {"if",if_c,-10,1}, {"else",else_c,-15,1},{"{",left_b,-3,2}, {"}",right_b,-3,2},{"inp", inp,9,1}, {"help", help,22,1}, {"num",num,8,0}, {"run",run,-15,1},  {"(",leftp,11,0}, {"mat",mat,0,0}, {")",rightp,11,0}, {"while",while_c,-10,1}};  // to add a keyword, add first to enum task and then to this keywords_list and increase the count of cnt_keywords 

char separators_list[cnt_seps]={ ',' , '+' , '/', '-' , '*' , '=', '!' , '(' , ')' , '"', '<', '>', '&', '|' ,'#'}; // to add a separator, add to this separators_list and increase the count of cnt_seps 


int def_mat_cnt=10;
int def_num_cnt=15;
int def_mat_m=3;
int def_mat_n=3;

int mat_var_cnt=0;
int num_var_cnt=0;

matrix *mat_var_list; // it is initialised in main() in MAD.c
number *num_var_list; // it is initialised in main() in MAD.c

matrix *mat_temp_list;
int mat_temp_cnt=0;

//char *temp_vars={"1t","2t","3t","4t","5t","6t"};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int execline(char *line) // to execute the command specified in line
 {
   char **argv;
   int argc;
   int i;
   int return_value=0;
   mat_temp_cnt=0;
   
   parse(line, &argv, &argc);
   //printargv();
    
   token t;
   
   infix_to_postfix(&argv,&argc);
   //printargv();
   
   mat_temp_list=malloc((mat_temp_cnt+1)*sizeof(matrix));
   mat_temp_cnt=-1; 
    
   int move,j,result=0, cnt_op=0;  //result is used to communicate with the execfile program to check for 'if's and 'while's
   char *word;
   
   while(argc>0) // this loop is what is actually responsible for executing a line
    {
      //printf("\nThe current stack is:\n");
      //printargv();
      i=0;
      t = istask(argv[i]); 
      word=NULL;
      while(t.value==na)
        {
          i++;
          if(i==argc)
            break;
          t = istask(argv[i]);  
        }
      if(i==argc) 
        break;
      cnt_op++;     
      switch(t.value)
          {
            case add:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! add");
                      word=NULL;
                    }  
                   else
                     word=add_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case sub:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! sub");
                      word=NULL;
                    } 
                   else
                   word=sub_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case mult:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! mult");
                      word=NULL;
                    } 
                   else
                     word=mult_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case divide:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! divide");
                      word=NULL;
                    } 
                   else
                     word=divide_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case equal:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! equal");
                      word=NULL;
                    } 
                   else
                     word=equal_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case less:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! less");
                      word=NULL;
                    } 
                   else
                     word=less_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case great:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! great");
                      word=NULL;
                    } 
                   else
                     word=great_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case lesse:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! lesse");
                      word=NULL;
                    } 
                   else
                     word=lesse_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case greate:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! greate");
                      word=NULL;
                    } 
                   else
                     word=greate_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case equality:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! equality");
                      word=NULL;
                    } 
                   else
                     word=equality_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case nequality:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! nequality");
                      word=NULL;
                    } 
                   else
                     word=nequality_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case not:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! not");
                      word=NULL;
                    } 
                   else
                     word=not_task(argv[i-1]);
                   break;
                 }
            case and:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! and");
                      word=NULL;
                    } 
                   else
                     word=and_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case or:
                 {
                   if(i-2<0)
                    {
                      printf("Error!!! or");
                      word=NULL;
                    } 
                   else
                     word=or_task(argv[i-2],argv[i-1]);
                   break;
                 }
            case clear:
                 {
                   system("clear");
                   break;
                 }    
            case del:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! del");
                      word=NULL;
                    }  
                   else
                     word=del_task(argv[i-1]);
                   break;
                 }
            case det:
                 {
                   if((i-1)<0)
                     printf("Error!!! det");
                   word=det_task(argv[i-1]);
                   break;
                 }
            case disp:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! disp");
                      word=NULL;
                    } 
                   else
                     disp_task(argv,i);
                   break;
                 }
            case close:
                 {
                   printf("\n\n\t\t Thank You for using MAD\n\t\tDeveloped by Adarsh Saraf\n\t\t\t Feb 2013\n\n\n\n");
                   exit(0);
                 }
            case if_c:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! if");
                      word=NULL;
                    } 
                   else
                     return_value=if_task(argv[i-1])+10;  // if the condition is true return 11, else 10
                   break;
                 }
            case else_c:
                 { 
                   break;
                 }
            case while_c:				// 1 && 2 in the ten's place in the return values in if_c and while_c are for while and if resp.
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! while");
                      word=NULL;
                    } 
                   else
                     return_value=if_task(argv[i-1])+20; // if_task is called since a while is basically an if with a loop, and the loop is taken care of execfile; if the condition is true return 21, else 20
                   break;
                 }
            case inp:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! inp");
                      word=NULL;
                    }  
                   else
                     word=inp_task(argv[i-1]);
                   //printf("\n\n%s\n\n",word);
                   break;
                 }
            case help:
                 {
                   if(i+1>=argc) // print the help for all the operators and keywords
                    {
                      int i=1;
                      for(;i<cnt_keywords;i++)
                       {
                         if(i!=(int)leftp&& i!=(int)rightp) 
                           printf("\n %s\t:   %s", keywords_list[i].name, keywords_list[i].help);
                       }
                    }
                   else // there exist an operator after help for whom we have to display help
                    {
                      token t;
                      int j=i+1;
                      for(;j<argc;j++)
                       { 
                         t = istask(argv[j]);
                         if(t.value!=na)
                           printf("\n %s\t:   %s", keywords_list[t.value].name, keywords_list[t.value].help);
                         else
                           printf("\nerror! help requires a keyword or an operator as an operand\n");
                       }
                      // to free this entire line 
                      for(j=0;j<argc;j++)
                        if(argv[j]!=NULL)
                         {
                           free(argv[j]);
                           argv[j]=NULL;
                         }
                                            
                      argc=-1; //since only help should be executed with help; therefore finish the help statement
                      word=NULL;
                    }
                   break;
                 }
            case num:
                 {
                   num_task(argv,i);
                   break;
                 }
            case run:
                 {
                   if(i-1<0)
                    {
                      printf("Error!!! run");
                      word=NULL;
                    }  
                   else
                    { 
                      // to remove the quotes from the filename
                      if(argv[i-1][0]=='\"')
                       {
                         int j;
                         for(j=1;argv[i-1][j]!='\"'&&argv[i-1][j]!='\0';j++) ; //to arrive at the closing quote, if present
                         // j started from 1 since at the 0th position we know that there is '\"' which has to be avoided
                        // if(argv[i-1][j]=='\"')// now the current j is where the closing quote is present
                          //{
                            argv[i-1][j]='\0'; // make the closing quote as NULL to terminate the filename here
                            script(argv[i-1]+1); // argv[i-1]+1 allows for the skipping of the opening quote 
                         // }
                        // else
                         //  printf("Error!!! filename not ending in quotes\n";
                       }
                      else
                        printf("Error!!! filename not in quotes\n");
                    }
                   break;
                 }
            case mat:
                 {                  
                   mat_task(argv,i);
                   word=NULL;
                   break;
                 }
          }  // switch ends
      
      move=1+t.num_operands;
      
      j=i-t.num_operands;
      
      if(word!=NULL)
       {
         move--;
         if(j<argc && argv[j]!=NULL)
          free(argv[j]);
         argv[j]=word;
         j++;
       }
      
      if(j<0)
        j=0;          
        
      
      argc-=move;
       
      for(;j<=argc;j++)
       {
         if(j<=i)
           if(argv[j]!=NULL)
             free(argv[j]);
         if(j<argc)
          {
            argv[j]=argv[j+move];
            argv[j+move]=NULL;
          }
         else
          argv[j]=NULL;
       }
      	
    } // while loop ends here
  
   // to free the dynamically allocated memory
   for(i=0;i<argc;i++)
    {
      if(argv[i]!=NULL)
        free(argv[i]); 
      argv[i]=NULL;
    }
  if(argv!=NULL)
   {
     free(argv);
     argv=NULL;
   }
  if(cnt_op==0)
    printf("\n%s ~ no operation specified!!!\n",line);
  return return_value;
   //returns after having completed the execution of the current line
 }


void parse(char *line, char ***argv, int *argc) // to parse the command contained in line and return the words in argv with count in argc, both passed by reference
 {
   (*argc)=0;
   int i,j,k,word=0,beg,cnt_square;  // beg refers to th beginning of the current word in 'line'
   token t;
   
   for(i=0;line[i]!='\0';i++)
     (*argc)+=isseparator(line[i]); // isseparator() returns the number of words the separator contributes to the line, which is zero if the character is not a separator
   
   (*argv)=malloc((*argc)*sizeof(char *));
   
   beg=0;
   for(j=0;line[j]!='\0'&&line[j]!='\n';) // '\n' that remains in lines from scripted files is taken care of since it is also a separator
    {
      while(!isseparator(line[j]))//&&line[j]!='\0')
       {
         if(line[j]=='[') // since within [ ] any occurence of a separator is taken care of by the assign_indices function
          {
            cnt_square=1;
            while(cnt_square!=0) // to allow for nesting of matrix variables 
             {
               j++;
               if(line[j]=='[') cnt_square++;
               else if(line[j]==']') cnt_square--;
             }
          }
         j++;
       }
      if(j>beg)
       {
         (*argv)[word]=malloc((j-beg+1)*sizeof(char)); // j-i gives the length of the word, and +1 to provide for NULL
         k=0;
         while(beg<j)
          {
            (*argv)[word][k]=line[beg];
            k++;
            beg++;
          }
         (*argv)[word][k]='\0';
         word++;
       } 
      // now line[j] is a separator
      if(!isspace(line[j]))
       {
         if(line[j]=='#')
          {
            break; // we are done with the parsing since beyond '#' everything in a line is a comment
          }
         if(line[j]=='\"')
          {
            if(j>beg)
            beg=j;
            j++;
            while(line[j]!='\"'&&line[j]!='\0'&&line[j]!='\n')
              j++;
            if(line[j]=='\"')
              j++; //since the closing '"' has to be included in the string
            (*argv)[word]=malloc((j-beg+1)*sizeof(char)); // j-beg gives the length of the word, and +1 to provide for NULL
            k=0;
            while(beg<j)
             {
               (*argv)[word][k]=line[beg];
               k++;
               beg++;
             }
            (*argv)[word][k]='\0';
            word++;
            //j++;
          }
         else if(line[j]=='<'&&line[j+1]=='=')
          {
            (*argv)[word]=malloc(3*sizeof(char));
            (*argv)[word][0]=line[j];
            j++;
            (*argv)[word][1]=line[j];
            (*argv)[word][2]='\0';
            word++;
            j++; // since the current character has been processed
            beg=j;
          }
         else if(line[j]=='>'&&line[j+1]=='=')
          {
            (*argv)[word]=malloc(3*sizeof(char));
            (*argv)[word][0]=line[j];
            j++;
            (*argv)[word][1]=line[j];
            (*argv)[word][2]='\0';
            word++;
            j++; // since the current character has been processed
            beg=j;
          }
         else if(line[j]=='='&&line[j+1]=='=')
          {
            (*argv)[word]=malloc(3*sizeof(char));
            (*argv)[word][0]=line[j];
            j++;
            (*argv)[word][1]=line[j];
            (*argv)[word][2]='\0';
            word++;
            j++; // since the current character has been processed
            beg=j;
          }
         else if(line[j]=='!'&&line[j+1]=='=')
          {
            (*argv)[word]=malloc(3*sizeof(char));
            (*argv)[word][0]=line[j];
            j++;
            (*argv)[word][1]=line[j];
            (*argv)[word][2]='\0';
            word++;
            j++; // since the current character has been processed
            beg=j;
          }
         else if(line[j]=='-' && isdigit(line[j+1])) 
          {
            t=istask((*argv)[word-1]);
            if(t.value!=na)
             {
               beg=j; //since this '-' has to be retained in the word
               j++;
             }
            else
             { // since now this - is an operator which has to be added as a word
               (*argv)[word]=malloc(2*sizeof(char));
               (*argv)[word][0]=line[j];
               (*argv)[word][1]='\0';
               word++;
               j++; // since the current character has been processed
               beg=j;
             }
          }  
         else
          {
            (*argv)[word]=malloc(2*sizeof(char));
            (*argv)[word][0]=line[j];
            (*argv)[word][1]='\0';
            word++;
            j++; // since the current character has been processed
            beg=j;
          }  
       }
      else
       {
         j++;
         beg=j;
       }  
    }
   if(word==0)
    {
      *argc=0;
      free(*argv);
      *argv=NULL;
    }
   if(!(word==*argc))
    {
      (*argc)=word; 
      (*argv)=realloc((*argv),(*argc)*sizeof(char *));
    }  
 }

void infix_to_postfix(char ***argv, int *argc1) // to convert the given infix expression in the shell stored in argv to postfix
 { 
   if(*argc1==0)
     return; //since there are no arguments to work with
   int argc=*argc1;
   char **tempargv1=(*argv), **tempargv2;
   
   token *stack=malloc((argc+1)*sizeof(operation)); // +1 for left parenthesis at the bottom of the stack
   
   tempargv2=malloc(argc*sizeof(char *));
   
   int a1,a2,s;
   token cur_task;
   a1=0;
   a2=-1;
   s=-1;
   
   ++s;        // pushing '(' onto the stack initially
   stack[s]=keywords_list[(int)leftp];
   
   for(a1=0;a1<=argc;a1++)
    {
      if(a1==argc)
        cur_task=keywords_list[(int)rightp];
      else
        cur_task=keywords_list[(int)(istask(tempargv1[a1]).value)];
      if(cur_task.value==na)
       {
         // add tempargv1[a1] to tempargv2[a2]
         a2++;
         tempargv2[a2]=malloc(( strlen(tempargv1[a1]) +1) * sizeof(char) );
         strcpy(tempargv2[a2],tempargv1[a1]);
       }  
      else
       {
         if(cur_task.value==leftp)
          {
            // push onto stack
            s++;
            stack[s]=cur_task;
          }
         else if(cur_task.value==rightp)
          {
            // pop from stack and add to tempargv2 until '(' is encountered which is not added
            cur_task=stack[s];  // cur_task can be used since it's current value, i. e., ')' is no longer required
            s--;
            while(cur_task.value!=leftp)
             {
               a2++;
               tempargv2[a2]=malloc(( strlen(cur_task.name) +1) * sizeof(char) );
               strcpy(tempargv2[a2],cur_task.name);
               cur_task=stack[s];
               s--;
             }
            // '(' has been popped from stack since s had been decremented after assigning it to cur_task 
          }
         else
          {
            mat_temp_cnt++;
            token t1;
            t1=stack[s];
            while(t1.value!=leftp && t1.precedence >= cur_task.precedence)
             {
               // pop from stack and add to tempargv2
               s--;
               a2++;
               tempargv2[a2]=malloc(( strlen(t1.name) +1) * sizeof(char) );
               strcpy(tempargv2[a2],t1.name);
               t1=stack[s];
             }
            // now cur_task has to be pushed onto the stack
            s++;
            stack[s]=cur_task;
          }
       }  
    }
  int i;
  for(i=0;i<argc;i++)
   {
     if(tempargv1[i]!=NULL)
      {
        free(tempargv1[i]);
        tempargv1[i]=NULL;
      }
   }
  // NOTE: tempargv2[i] for i>a2 has not been assigned any space so dont have to free it  
   
  if(stack!=NULL)
    free(stack); 
  if(tempargv1!=NULL)
    free(tempargv1);
  
  //tempargv2=realloc(tempargv2,((a2+1)*sizeof(char *)));
  
  (*argv)=tempargv2;
  *argc1=a2+1; 
 
 }
 
int isseparator(char c) // to check if c is a separator; return 0 if not, else return the number of words it contributes to the line 
 {
   int r=0;
   if (isspace(c))
     r=1;
   else 
    {
      int i=0;
      for(i=0;i<cnt_seps;i++)
        if(c==separators_list[i])
          r=2;
    }
   return r;  // r=0 as defined if it's not a space nor any of the separators
 }  
 
token istask(char *word) // to check if word is a task
 {
   int i=0;
   while(i<cnt_keywords)
    {
      if(strcmp(word,keywords_list[i].name)==0)
        return keywords_list[i];
      i++;
    }
   return keywords_list[(int)na];
 }
 
int isvalid_identifier(char *word) //to check if word is a valid identifier
 {
   char *temp=word;
   int i=0;
   if(strstr(word,"[")!=NULL)
    {
      int len;
      for(i=0;word[i]!='['&&word[i]!='\0';i++) ;
      len=i;						// to allow for the presence of [][] in the word containing the identifier name
      temp=malloc((len+1)*sizeof(char));
   
      for(i=0;i<len;i++) 
        temp[i]=word[i];
      temp[i]='\0';
    }
    
   if(! ( isalpha(temp[0]) || temp[0]=='_' ) )
     return 0;
   else
    {
      int i=1;
      while(temp[i]!='\0')
       {
         if(! ( isalnum(temp[i]) || temp[i]=='_' ) )
           return 0;
         i++;
       }
      // when the program reaches here word has been found to contain only alphabets, numbers or underscores
      return 1;
    }
 } 

int isvalid_number(char *word) //to check if word is a valid number
 {
   int i,cnt=0;  // i is the loop counter and cnt the count of '.' in the string word
   for(i=0;word[i]!='\0';i++)
    {
      if(!( (word[i]>='0' && word[i]<='9') || (word[i]=='.') || (i==0 && word[i]=='-') ))
        break;
      else if(word[i]=='.')
        cnt++;      
    }
   if(word[i]!='\0'||cnt>1)
     return 0;
   else 
     return 1;      
 }

int isused_identifier(char *word) //to check if the identifier word has already been used
 {
   char *temp=word;
   int i=0;
   if(strstr(word,"[")!=NULL)
    {
      int len;
      for(i=0;word[i]!='['&&word[i]!='\0';i++) ;
      len=i;
      temp=malloc((len+1)*sizeof(char));
   
      for(i=0;i<len;i++) 
        temp[i]=word[i];
      temp[i]='\0';
    }
   if(isvalid_number(temp))
     return -1;
   else if(strstr(temp," ")!=NULL)
     return 1;
   for(i=0;i<mat_var_cnt;i++)
     if(mat_var_list[i].name!=NULL && strcmp( mat_var_list[i].name , temp )==0)
       return i+2;
   for(i=0;i<num_var_cnt;i++)
     if(num_var_list[i].name!=NULL && strcmp( num_var_list[i].name , temp)==0)
       return (-1)*(i+2);
   return 0;
 }

void init_help()
 {
   keywords_list[add].help="to add two matrices, numbers, or a number to a matrix; adding a matrix to a number will give error; Usage: A+B, m+n, A+m\n";
   keywords_list[sub].help="to subtract two matrices, numbers, or a number from a matrix; subtract a matrix from a number will give error; Usage: A-B, m-n, A-m\n";
   keywords_list[mult].help="to multiply two matrices, numbers, or a matrix with a number; multiplying a number with a number will give error; Usage: A*B, m*n, A*m\n";
   keywords_list[divide].help="to divide two numbers, or a matrix by a number; dividing by a matrix will give error, and by zero will give segemntaion fault; Usage: A/B, m/n, A/m\n";
   keywords_list[equal].help="to assign to a matrix a matrix, or to a number a number, or to a matrix a number element-wise; assigning to a number a matrix will give error; Usage: A=B, m=n, A=m\n";
   keywords_list[less].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A<B, m<n, A<m\n";
   keywords_list[great].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A>B, m>n, A>m\n";
   keywords_list[lesse].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A<=B, m<=n, A<=m\n";
   keywords_list[greate].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A>=B, m>=n, A>=m\n";
   keywords_list[equality].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A==B, m==n, A==m\n";
   keywords_list[nequality].help="to compare two matrices element-wise, two numbers, or a matrix with a number element-wise; comparing a number with a matrix will give error; Usage: A!=B, m!=n, A!=m\n";
   keywords_list[not].help="to return 1 if operand is 0, 0 otherwise; Usage: !<operand>\n";
   keywords_list[and].help="to return 1 if both its operands are 1, 0 otherwise; Usage: <operand1> & <operand2>\n";
   keywords_list[or].help="to return 0 if both its operands are 0, 1 otherwise; Usage: <operand1> | <operand2>\n";
   keywords_list[left_b].help="to start a block of commands in programs; not to be used in the shell\n";
   keywords_list[right_b].help="to end a block of commands in programs; not to be used in the shell\n";
   keywords_list[clear].help="to clear the screen; Usage: clear\n";
   keywords_list[del].help="to delete the matrix(only the particular rows or columns, if indices have been specified) or the number specified; Usage: del A, del m, del A[2;3][4:5]\n";
   keywords_list[det].help="to return the determinant of a square matrix; specifying a number operand or a non-square matrix causes error; Usage: det A, det A[2:4][4;7;9]\n";
   keywords_list[help].help="to see help for all the operators and keywords available in the shell\n";
   keywords_list[disp].help="to display a matrix, a number or a string; can except multiple arguments; Usage: disp A m \"Sai Ram\"\n";
   keywords_list[close].help="to quit MAD; Usage: exit\n";
   keywords_list[if_c].help="to run the following block of code if and only if the condition specified with it is true; non-specification of a condition causes error; available only in programming\n";
   keywords_list[else_c].help="to run the following block of code if and only if the corresponding 'if' condition is false; available only in programming\n";
   keywords_list[inp].help="to input the values of the matrix or number specified as an argument; Usage: inp A, inp m\n";
   keywords_list[run].help="to execute a program passed as an operand; the filename must be enclosed in quotes, else there will be an error; there should not be any common variables between the shell and the program as that also would be treated as a redeclaration; Usage: run <filename>\n";
   keywords_list[num].help="to declare a number variable; can accept multiple arguments; Usage: num m n\n";
   keywords_list[mat].help="to declare a matrix variable; can accept multiple arguments; default matrix size is 3 by 3; Usage: mat A[3][4] B[10][20]\n";
   keywords_list[while_c].help="to run the following block of code WHILE the condition specified with it is true; non-specification of a condition causes error; available only in programming\n";
 } 
