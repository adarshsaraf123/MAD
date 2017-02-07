/*
 Purpose: to write the functions for each of the task specified for MAD
 Written on: 23/01/13 
 Written by: Adarsh Saraf
*/

#include"tasks.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

extern int def_mat_cnt;
extern int def_num_cnt;
extern int def_mat_m;
extern int def_mat_n;

extern token keywords_list[];

extern char separators_list[];

extern int mat_var_cnt;
extern int num_var_cnt;

extern matrix *mat_var_list;
extern number *num_var_list;

extern matrix *mat_temp_list;
extern int mat_temp_cnt;

int get_operands2(char * operand1, char *operand2, number **n1, number **n2, matrix **m1, matrix **m2) // to get the operands for binary operators and store them in the respective variables  
 {
   int op1,op2;
   int load=0;
   
   op1=isused_identifier(operand1);
   op2=isused_identifier(operand2);
    
   if(op1<0)
    {
      if(op1==-1)
        sscanf(operand1,"%lf",&((*n1)->value)); 
      else
       {
         free(*n1);
         (*n1)=&num_var_list[(-1*op1)-2];
       }  
      load+=1;
    }
   else if(op1>0)
    {
      if(op1==1)
       {
         int pos;
         sscanf(operand1,"%d",&pos);
         *m1=&mat_temp_list[pos];  
       }
      else
        *m1=&mat_var_list[op1-2];
      assign_indices(operand1, *m1);
      load+=2;
    }
   
   if(op2<0)
    {
      if(op2==-1)
        sscanf(operand2,"%lf",&((*n2)->value));
      else
       {
         free(*n2);
         *n2=&num_var_list[(-1*op2)-2];
       }  
      load+=10;
    }
   else if(op2>0)
    {
      if(op2==1)
       {
         int pos;
         sscanf(operand2,"%d",&pos);
         *m2=&mat_temp_list[pos];
       }
      else if(op2==op1)
       {
         *m2=malloc(sizeof(matrix));
         (*m2)->m=(*m1)->m;
         (*m2)->n=(*m1)->n;
         (*m2)->a=(*m1)->a;
         (*m2)->row_indices=malloc(((*m2)->m)*sizeof(int));
         (*m2)->col_indices=malloc(((*m2)->n)*sizeof(int));
       }
      else
        *m2=&mat_var_list[op2-2];
      assign_indices(operand2, *m2);
      load+=20;
    } 
   return load;  
 }

int get_operands1(char *operand, number **n, matrix **m) // to get the operand for unary operators and store them in the respective variable  
 {
   int op=isused_identifier(operand);
   int load=0;
   
   if(op<0)
    {
      if(op==-1)
        sscanf(operand,"%lf",&((*n)->value));
      else
       {
         free(*n);
         *n=&num_var_list[(-1*op)-2];
       }  
      load+=1;
    }
   else if(op>0)
    {
      if(op==1)
       {
         int pos;
         sscanf(operand,"%d",&pos);
         *m=&mat_temp_list[pos];
       }
      else
        *m=&mat_var_list[op-2];
      assign_indices(operand,*m);
      load+=2;
    }
   return load;
 }
 
void assign_indices(char *operand, matrix *m) //to assign the indices for rows and columns which are to be used for the operations into row_indices and col_indices respectively 
 {
   char *ptr,*word;
   int len,i,j,beg;
   int temp; // this will hold the current value being inserted in any of the indices temporarily
   ptr=word=NULL;
   int type; // this will contain the type of word, i.e., num or num_constant
   int *a; int index_cnt; // a holds *row_indices or *col_indices based on value of cur_index(0 and 1 respectively); similarly for index_cnt
   int cur_index; // if 0 it tells the current indices being processed is those of rows, and if 1 those of columns
   int diagonal=0; // 1 if referring to only the diagonal elements

   a=(*m).row_indices;
   index_cnt=(*m).m;
   ptr=operand;
   cur_index=0;
   
   while(cur_index<2)
    {
      if(cur_index==1)
       {
         a=(*m).col_indices;
         index_cnt=(*m).n;
       }
      if(/*strstr(ptr," ") || */(ptr=strstr(ptr,"["))==NULL)
       {
         for(i=0;i<index_cnt;i++)
           a[i]=i;
         ptr=operand; // since ptr has been made NULL
       }
      else
       {
         ptr++;
         if(*ptr==']')
          { 
            for(i=0;i<index_cnt;i++)
              a[i]=i;
          }
         else
          {
            if(*ptr=='[')
             {
               diagonal=1;
               ptr++;
             }
            index_cnt=0;
            for(i=0,beg=0;ptr[i-1]!=']';i++)
             {
               if(ptr[i]==';'||ptr[i]==']'||ptr[i]==':'||ptr[i]=='+'||ptr[i]=='-' || ptr[i]=='*' || ptr[i]=='/')
                {
                  
                  len=i-beg;
                  word=malloc((len+1)*sizeof(char));
                  for(j=0;j+beg<i;j++)
                    word[j]=ptr[j+beg];
                  word[j]='\0';
                  
                  type=isused_identifier(word);
                  
                  if(type==-1)
                   {
                     sscanf(word,"%d",&temp); 
                     temp--;  // since user's '1' row is actually the'0' row in the system
                   }
                  else if(type<-1)
                    temp=num_var_list[(-1*type)-2].value-1; // since user's '1' row is actually the'0' row in the system
                  else
                   {
                     printf("Error!!! no value specified before '%c'", ptr[i]);
                     return;
                   }
                  
                  if(m->name!=NULL && ( temp<0 || temp>=( (cur_index==0) ? (m->m) : (m->n) ) )) // to care of invalid indices
                   {
                     printf("\ninvalid index\n");
                     free(word);
                     beg=i+1;
                     continue;
                   }
                  
                  if(ptr[beg-1]==':')
                   {
                     if(index_cnt==0)
                      {
                        printf("Error!!! no value before ':'");
                        return;
                      }
                     else
                      {
                        j=index_cnt;
                        index_cnt += temp-a[j-1]; // since you are adding values to 'a' from a[k-1] to temp
                        for(;j<index_cnt;j++)
                          a[j]=a[j-1]+1;
                      }
                   }
                  else if(ptr[beg-1]=='+'||ptr[beg-1]=='-'||ptr[beg-1]=='*'||ptr[beg-1]=='/')
                   {
                     if(index_cnt==0)
                      {
                        printf("Error!!! no value before '+'");
                        return;
                      }
                     else
                      {
                        if(ptr[beg-1]=='+')
                          a[index_cnt-1]=(a[index_cnt-1]+1)+(temp+1)-1; // since temp and a[index_cnt-1] has been reduced by 1 from what the user has entered, and therefore we need the adjustment
                        
                        else if(ptr[beg-1]=='-')
                          a[index_cnt-1]=(a[index_cnt-1]+1)-(temp+1)-1;
                        
                        else if(ptr[beg-1]=='*')
                          a[index_cnt-1]=(a[index_cnt-1]+1)*(temp+1)-1;
                        else
                           a[index_cnt-1]=(a[index_cnt-1]+1)/(temp+1)-1;
                      }
                   }
                  else
                   {
                     index_cnt++;
                     a[index_cnt-1]=temp;
                   }
                 
                 free(word);  
                 beg=i+1;               
                
                } 				// endif for if(ptr[i]==';'(...))
             }					// end of for loop
          }					//end of else for if(*ptr==']')
       }					//end of else for if((ptr=strstr(ptr,"["))==NULL)
      
     // a=realloc(a,index_cnt*sizeof(int));
      if(cur_index==0)
       {
        // (*m).row_indices=a;
         ((*m).row_cnt)=index_cnt; 
         if(diagonal)
          {
            (*m).col_cnt=(*m).row_cnt;
            for(i=0;i<(*m).col_cnt;i++)
              ((*m).col_indices)[i]=((*m).row_indices)[i];
            cur_index++;
          }         
       }
      else 
       {
         ((*m).col_cnt)=index_cnt;
       }
      ptr++;
      cur_index++; // since the current cur_index has been processed
      
    }
  						// end of while
 }

void write_num(char **word, number n) // to write the value of 'n' into 'word'
 {
   char *pattern;
   
   int dec,ndec;
   dec=cnt_decimal(n.value);
   ndec=cnt_nondecimal(n.value);
           
   int len_pattern = 3+cnt_nondecimal(ndec+dec)+1+cnt_nondecimal(dec)+1;
   int len_word = ndec+1+dec;
   
   pattern=malloc((len_pattern+1)*sizeof(char));
   *word=malloc((len_word+1)*sizeof(char));
   
   sprintf(pattern,"%%%d.%dlf",ndec+dec,dec);
          /*printf("%s",pattern);
           getchar();
           getchar();*/
           //printf("\n");
   sprintf(*word,pattern,n.value);
   free(pattern);
 }

char *num_task(char **argv, int pos)
 {
   if(pos==0)
     printf("\nNot enough arguments to num!!!\n");
   else
    {
      int i=0;
      for(;i<pos;i++)
        num_func(argv[i]);
    }
 }

void num_func(char *operand) //to initialise the number defined by the user in the shell
 {
   if(!isvalid_identifier(operand))
     printf("\nIdentifier '%s' not valid\n",operand);
   else if(isused_identifier(operand))
     printf("\nIdentifier '%s' already in use\n",operand);
   else
    {
      num_var_cnt++;
      int cur=num_var_cnt-1;
      if(num_var_cnt>def_num_cnt) // realloc space for num_var_list if the count of matrix variables has exceeded default count 
       {
         def_num_cnt*=2;
         num_var_list=realloc(num_var_list,def_num_cnt*sizeof(number));
       }
      
      int len;
      for(len=0;operand[len]!='\0';len++); // to count the length of operand excluding the terminating null
    
      num_var_list[cur].name=malloc((len+1)*sizeof(char));
      sprintf(num_var_list[cur].name,"%s",operand);	//for naming the number variable
      
      num_var_list[cur].value=0;
    }
 }

char *mat_task(char **argv, int pos) //to initialise the matrix defined by the user in the shell
 {
   if(pos==0)
     printf("\nNot enough arguments to num!!!\n");
   else
    {
      int i=0;
      for(;i<pos;i++)
        mat_func(argv[i]);
    }
 }

void mat_func(char *operand) //to initialise the matrix defined by the user in the shell
 {
   int *row, *col, i,j;
   int m,n;
   if(!isvalid_identifier(operand))
     printf("\nIdentifier '%s' not valid\n",operand);
   else if(isused_identifier(operand))
     printf("\nIdentifier '%s' already in use\n",operand);
   else
    {
      matrix mtemp; // mtemp is a dummy matrix required to get the dimensions of the matrix to be initialised, that is, m and n
      mtemp.name=NULL;
      mtemp.m=1;
      mtemp.n=1;
      mtemp.row_indices=malloc(mtemp.m*sizeof(int));
      mtemp.col_indices=malloc(mtemp.n*sizeof(int));
      assign_indices(operand, &mtemp); // to get the value of m and n
      
      if(mtemp.row_indices[0]==0 && mtemp.col_indices[0]==0)
       {
         m=def_mat_m;
         n=def_mat_n;
       }
      else
       {
         m=mtemp.row_indices[0]+1; //since 1 less had been assigned to row_indices in assign_indices
         n=mtemp.col_indices[0]+1; //since 1 less had been assigned to row_indices in assign_indices
       }
      init_mat_var(operand,m,n);
    }
 }
 
 
void init_mat_var(char *name, int m, int n) // to initialise a matrix variable with identifier name as 'name' and size 'm' by 'n' 
 {
   mat_var_cnt++;
   int cur=mat_var_cnt-1;
   int i,j,len;
   
   if(mat_var_cnt>def_mat_cnt) // realloc space for mat_var_list iff it the count of matrix variables has exceeded default count 
    {
      def_mat_cnt*=2;
      mat_var_list=realloc(mat_var_list,def_mat_cnt*sizeof(matrix));
    }
   
   for(len=0;name[len]!='[' && name[len]!='\0';len++); // to count the length of name1 until the appearance of the first '[' in it or NULL, whichever occurs first
   
   mat_var_list[cur].name=malloc((len+1)*sizeof(char));
   for(i=0;i<len;i++)		//for naming the matrix variable
     (mat_var_list[cur].name)[i]=name[i];
   (mat_var_list[cur].name)[i]='\0';
   
   mat_var_list[cur].m=m;
   mat_var_list[cur].n=n;
   
   mat_var_list[cur].row_indices=malloc(mat_var_list[cur].m*sizeof(int));
   mat_var_list[cur].col_indices=malloc(mat_var_list[cur].n*sizeof(int));
   
   mat_var_list[cur].a=malloc(m*sizeof(double *));
   for(i=0;i<m;i++)
     (mat_var_list[cur].a)[i]=malloc(n*sizeof(double));
     
   for(i=0;i<m;i++)      // to initialise the matrix with zeroes
     for(j=0;j<n;j++)
       (mat_var_list[cur].a)[i][j]=0;
 } 

void init_mat_temp(matrix **mat, int m, int n) // to initialise a temporary matrix variable with size m and n whose address is assigned to *m
 {
   mat_temp_cnt++; // following this, mat_temp_cnt is the temporary matrix being used by this operation
   (*mat)=&mat_temp_list[mat_temp_cnt];
   
   int len=cnt_nondecimal(mat_temp_cnt)+2; 
   (*mat)->name=malloc((len+1)*sizeof(char));
   sprintf((*mat)->name,"%d m",mat_temp_cnt);
          
   (*mat)->m=m;
   (*mat)->n=n;
   
   (*mat)->row_indices=malloc(m*sizeof(int));
   (*mat)->col_indices=malloc(n*sizeof(int));
   
   int i,j;
   ((*mat)->a)=malloc(m*sizeof(double *));
   for(i=0;i<m;i++)
     ((*mat)->a)[i]=malloc(n*sizeof(double));
     
   for(i=0;i<m;i++)      // to initialise the matrix with zeroes
     for(j=0;j<n;j++)
       ((*mat)->a)[i][j]=0;
 }
 
char *inp_task(char *operand)
 {
   number *n=NULL;
   n=malloc(sizeof(number));
  
   matrix *m;
     
   int load;
   
   load=get_operands1(operand, &n, &m);
   
   switch(load)
    {
      case 0:
          printf("\nInvalid operand to inp!!!");
          return NULL;
          break;
      case 1:
         {
           printf("\n");
           scanf("%lf",&(n->value));
           break;
         }
      case 2:    
         {
           input_matrix(*m);
           break;
         }  
    }
    
   char *word=malloc((strlen(operand)+1)*sizeof(char));
   sprintf(word,"%s",operand);
   return word;
 }

void input_matrix(matrix m) // to assign the values supplied by the user m3
 {
   int i,j, row, col;
   printf("\n");
   for(i=0;i<m.row_cnt;i++)
     for(j=0;j<m.col_cnt;j++)
       {
         row=m.row_indices[i];
         col=m.col_indices[j];
         scanf("%lf",&((m.a)[row][col] ));
       }
 }

char *disp_task(char **argv, int pos)
 {
   int i=0;
   for(;i<pos;i++)
    disp_func(argv[i]);
 }

void disp_func(char *operand)
 {
   number *n;
   n=malloc(sizeof(number));
  
   matrix *m;
   // printf("%s",operand);
  // getchar();
  if(operand[0]=='"')
   {
     int i=1;
     while(operand[i]!='"'&&operand[i]!='\0')
      {
        if(operand[i]!='\\')
         printf("%c",operand[i++]);
        else
         {
           switch(operand[++i])
            {
              case 'n':
                 printf("\n");
                 i++;
                 break;
            }
         }
      }
   }
  /* if(strstr(operand,"\""))
      printf(operand+1);*/
   else
    {
     
      int load;
   
      load=get_operands1(operand, &n, &m);
     
      switch(load)
       {
         case 0:
             printf("\nInvalid operand to disp!!!");
             //return NULL;
             break;
         case 1:
            {
              char *word;
              write_num(&word,*n);
              printf("%s",word);
              break;
            }
         case 2:    
            {
              print_matrix(*m);
              break;
            }  
       }
    }  
 }

void print_matrix(matrix m) // to print the matrix m
 {
   int i,j,row,col;
   char *pattern;
   
   int dec=0,ndec=0, tdec, tndec;
   
   for(i=0;i<m.m;i++)
    for(j=0;j<m.n;j++)
     {
       tdec=cnt_decimal(m.a[i][j]);
       tndec=cnt_nondecimal(m.a[i][j]);
       if(tdec > dec) dec=tdec;
       if(tndec > ndec) ndec=tndec; 
     }
           
   int len_pattern = 3+cnt_nondecimal(ndec+dec+2)+1+cnt_nondecimal(dec)+1;
   
   pattern=malloc((len_pattern+1)*sizeof(char));
   sprintf(pattern,"%%%d.%dlf ",ndec+dec+2,dec);
   
   //printf("\nThe matrix %s is:\n", m.name);
   for(i=0;i<m.row_cnt;i++)
    {
      printf("\n");
      for(j=0;j<m.col_cnt;j++)
       {
         row=m.row_indices[i];
         col=m.col_indices[j];
         printf(pattern,(m.a)[row][col]);
       }
    }
   printf("\n");
 }
 
char *add_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
      
   int load;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '+' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '+' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '+' is not valid!!!");
         return NULL;
      case 11:
        {
          number n3;
          n3.value=n1->value+n2->value;
          write_num(&word,n3);
          return word;  
        }   
      case 12:
        {
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          addition_num_mat(*m1,*n2,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
          
        }  
      case 21:
        {
          matrix *m3;
          init_mat_temp(&m3,m2->row_cnt,m2->col_cnt);  
          
          addition_num_mat(*m2,*n1,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
          
        }  
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt!=m2->col_cnt)
           {
             printf("\naddition not compatible");
             return NULL;
           }  
          
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          addition_matrices(*m1,*m2,*m3);
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }  
    }
 }

void addition_matrices(matrix m1, matrix m2, matrix m3) // to add the matrices m1 and m2 and store the result in m3
 {
   int i,j;
   int row1,col1,row2,col2;
   for (i=0;i<m3.m;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m3.n;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         m3.a[i][j]=m1.a[row1][col1]+m2.a[row2][col2];      
       }
    }  
 }

void addition_num_mat(matrix m1, number n, matrix m2) // to add to the matrix m1 the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m2.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m2.n;j++)
       {
         col1=m1.col_indices[j];
         m2.a[i][j]=m1.a[row1][col1]+n.value;      
       }
    } 
 }
 
char *sub_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '-' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '-' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '-' is not valid!!!");
         return NULL;
      case 11:
        {
          number n3;
          n3.value=n1->value-n2->value;
          write_num(&word,n3);
          return word;  
        }   
      case 12:
        {
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          subtraction_num_mat(*m1,*n2,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
          
        }  
      case 21:
        {
          printf("\ncan't subtract matrix from a number\n");
          return NULL;
        }   
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt!=m2->col_cnt)
           {
             printf("\nsubtraction not compatible");
             return NULL;
           }  
          
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          subtraction_matrices(*m1,*m2,*m3);
          
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }  
    }
 }
 
void subtraction_matrices(matrix m1, matrix m2, matrix m3) // to subtract the matrices m1 and m2 and store the result in m3
 {
   int i,j;
   int row1,col1,row2,col2;
   for (i=0;i<m3.m;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m3.n;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         m3.a[i][j]=m1.a[row1][col1]-m2.a[row2][col2];      
       }
    }  
 }

void subtraction_num_mat(matrix m1, number n, matrix m2) // to subtract from the matrix m1 the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m2.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m2.n;j++)
       {
         col1=m1.col_indices[j];
         m2.a[i][j]=m1.a[row1][col1]-n.value;      
       }
    } 
 }

char *mult_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '*' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '*' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '*' is not valid!!!");
         return NULL;
      case 11:
        {
          number n3;
          n3.value=(n1->value)*(n2->value);
          write_num(&word,n3);
          return word;  
        }   
      case 12:
        {
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          multiplication_num_mat(*m1,*n2,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
          
        }  
      case 21:
        {
          matrix *m3;
          init_mat_temp(&m3,m2->row_cnt,m2->col_cnt);  
          
          multiplication_num_mat(*m2,*n1,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }    
      case 22:
        {
          if(m1->col_cnt!=m2->row_cnt)
           {
             printf("\nmultiplication not compatible");
             return NULL;
           }  
          
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m2->col_cnt);  
          
          multiplication_matrices(*m1,*m2,*m3);
             
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }  
    }
 }
 
void multiplication_matrices(matrix m1, matrix m2, matrix m3) // to multiply the matrices m1 and m2 and store the result in m3
 {
   int i,j,k;
   double temp;
   int row1,col1,row2,col2;
   for (i=0;i<m3.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m3.n;j++)
       {
         col2=m2.col_indices[j];
         //printf("\n");
         temp=0.00;
         for(k=0;k<m1.col_cnt;k++)
	  {
	    col1=m1.col_indices[k];
	    row2=m2.row_indices[k];
	    temp+= (m1.a)[row1][col1]*(m2.a)[row2][col2];
	   // printf("%lf ",temp);
	  } 
         (m3.a)[i][j]=temp;
       }       
    }      
 }

void multiplication_num_mat(matrix m1, number n, matrix m2) // to multiply each element of the matrix m1 by the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m2.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m2.n;j++)
       {
         col1=m1.col_indices[j];
         m2.a[i][j]=m1.a[row1][col1]*n.value;      
       }
    } 
 }
 
char *divide_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '*' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '*' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '*' is not valid!!!");
         return NULL;
      case 11:
        {
          number n3;
          n3.value=(n1->value)/(n2->value);
          write_num(&word,n3);
          return word;  
        }   
      case 12:
        {
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m1->col_cnt);  
          
          division_num_mat(*m1,*n2,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
          
        }  
      case 21:
        {
          matrix *m3;
          init_mat_temp(&m3,m2->row_cnt,m2->col_cnt);  
          
          division_num_mat(*m2,*n1,*m3); 
           
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }    
      case 22:
        {
          if(m1->col_cnt!=m2->row_cnt)
           {
             printf("\nmultiplication not compatible");
             return NULL;
           }  
          
          matrix *m3;
          init_mat_temp(&m3,m1->row_cnt,m2->col_cnt);  
          
         division_matrices(*m1,*m2,*m3);
             
          int len=cnt_nondecimal(mat_temp_cnt)+2; 
          char *word=malloc((len+1)*sizeof(char));
          sprintf(word,"%d m",mat_temp_cnt);
          return word; 
        }  
    }
 }
 
void division_matrices(matrix m1, matrix m2, matrix m3) // to multiply the matrices m1 and m2 and store the result in m3
 {
   int i,j,k;
   double temp;
   int row1,col1,row2,col2;
   for (i=0;i<m3.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m3.n;j++)
       {
         col2=m2.col_indices[j];
         //printf("\n");
         temp=0.00;
         for(k=0;k<m1.col_cnt;k++)
	  {
	    col1=m1.col_indices[k];
	    row2=m2.row_indices[k];
	    temp+= (m1.a)[row1][col1]*(m2.a)[row2][col2];
	   // printf("%lf ",temp);
	  } 
         (m3.a)[i][j]=temp;
       }       
    }      
 }

void division_num_mat(matrix m1, number n, matrix m2) // to multiply each element of the matrix m1 by the number n and store the result in m2 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m2.m;i++)
    {
      row1=m1.row_indices[i];
      for(j=0;j<m2.n;j++)
       {
         col1=m1.col_indices[j];
         m2.a[i][j]=m1.a[row1][col1]/n.value;      
       }
    } 
 }

char *equal_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          n1->value=n2->value;
          break;
        }   
      case 12:
        {
          equate_num_mat(*m1,*n2); 
          break;        
        }  
      case 21:
        {
          if(m2->col_cnt==1 && m2->row_cnt==1)
           {
             n1->value=(m2->a)[(m2->row_indices)[0]][(m2->col_indices)[0]];
             break;
           }
          else
           {
             printf("\ncan't assign a matrix to a number\n");
             return NULL;
           }
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
           {
             printf("\nequality incompatible");
             return NULL;
           }
          equate_matrices(*m1,*m2);
          break;
        }  
    }
    
   word=malloc((strlen(operand1)+1)*sizeof(char));
   sprintf(word,"%s",operand1);
   return word; 
 } 

void equate_matrices(matrix m1,matrix m2) // to assign values from m2 to m1 
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         (m1.a)[row1][col1]=(m2.a)[row2][col2];
       }
    }
 }

void equate_num_mat(matrix m, number n) // to equate each element of the matrix m the number n (value gets reflected since m3.a is same in the calling function and here)

 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         m.a[row1][col1]=n.value;      
       }
    } 
 }

char *less_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=(n1->value < n2->value);
          break;
        }   
      case 12:
        {
          return_value=less_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=0;
          else
            return_value=less_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 
 
int less_matrices(matrix m1, matrix m2) // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         if(! ( (m1.a)[row1][col1] < (m2.a)[row2][col2] ) )
           return 0;
       }
    }
   return 1;
 }
 
int less_mat_num(matrix m, number n) // to 
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         if(! ((m.a)[row1][col1] < n.value) )
           return 0;      
       }
    } 
   return 1; // it reaches here only if there is not a single element in the matrix greater than or equal to the number
 }
 
char *lesse_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=(n1->value <= n2->value);
          break;
        }   
      case 12:
        {
          return_value=lesse_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=0;
          else
            return_value=lesse_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 
 
int lesse_matrices(matrix m1, matrix m2) // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         if(! ( (m1.a)[row1][col1]<=(m2.a)[row2][col2] ) )
           return 0;
       }
    }
   return 1;
 }
 
int lesse_mat_num(matrix m, number n) // to 
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         if(! (m.a[row1][col1]<=n.value) )
           return 0;      
       }
    } 
   return 1; // it reaches here only if there is not a single element in the matrix greater than or equal to the number
 }
 
char *great_task(char *operand1, char *operand2)
  {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=(n1->value > n2->value);
          break;
        }   
      case 12:
        {
          return_value=great_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=0;
          else
            return_value=great_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 
 
int great_matrices(matrix m1, matrix m2) // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         if(! ( (m1.a)[row1][col1]>(m2.a)[row2][col2] ) )
           return 0;
       }
    }
   return 1;
 }
 
int great_mat_num(matrix m, number n) // to 
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         if(! (m.a[row1][col1]>n.value) )
           return 0;      
       }
    } 
   return 1; // it reaches here only if there is not a single element in the matrix greater than or equal to the number
 }
 
char *greate_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=(n1->value >= n2->value);
          break;
        }   
      case 12:
        {
          return_value=greate_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=0;
          else
            return_value=greate_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 
 
int greate_matrices(matrix m1, matrix m2) // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         if(! ( (m1.a)[row1][col1]>=(m2.a)[row2][col2] ) )
           return 0;
       }
    }
   return 1;
 }
 
int greate_mat_num(matrix m, number n) // to 
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         if(! (m.a[row1][col1]>=n.value) )
           return 0;      
       }
    } 
   return 1; // it reaches here only if there is not a single element in the matrix greater than or equal to the number
 }

char *equality_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=(n1->value == n2->value);
          break;
        }   
      case 12:
        {
          return_value=equality_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=0;
          else
            return_value=equality_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 
 
int equality_matrices(matrix m1, matrix m2) // to multiply the matrices m1 and m2 and store the result in m3 (value gets reflected since m3.a is same in the calling function and here)
 {
   int i,j;
   int row1,col1,row2,col2;
   for(i=0;i<m1.row_cnt;i++)
    {
      row1=m1.row_indices[i];
      row2=m2.row_indices[i];
      for(j=0;j<m1.col_cnt;j++)
       {
         col1=m1.col_indices[j];
         col2=m2.col_indices[j];
         if(! ( (m1.a)[row1][col1]==(m2.a)[row2][col2] ) )
           return 0;
       }
    }
   return 1;
 }
 
int equality_mat_num(matrix m, number n) // to 
 {
   int i,j;
   int row1,col1;
   for (i=0;i<m.row_cnt;i++)
    {
      row1=m.row_indices[i];
      for(j=0;j<m.col_cnt;j++)
       {
         col1=m.col_indices[j];
         if(! (m.a[row1][col1]==n.value) )
           return 0;      
       }
    } 
   return 1; // it reaches here only if there is not a single element in the matrix greater than or equal to the number
 }

char *nequality_task(char *operand1, char *operand2)
 {
   matrix *m1,*m2;
   number *n1,*n2;
   
   n1=malloc(sizeof(number));
   n2=malloc(sizeof(number));
   
   int load;
   int return_value;
      
   load=get_operands2(operand1, operand2, &n1, &n2, &m1, &m2);
   
   char *word;
   
   switch(load)
    {
      case 0:
         printf("Both the operands for '!=' are not valid!!!");
         return NULL;
      case 10:
      case 20:
         printf("First operand for '!=' is not valid!!!");
         return NULL;
      case 1:
      case 2:
         printf("Second operand for '!=' is not valid!!!");
         return NULL;
      case 11:
        {
          return_value=!(n1->value == n2->value);
          break;
        }   
      case 12:
        {
          return_value=!equality_mat_num(*m1,*n2); 
          break; 
        }  
      case 21:
        {
          printf("\ncan't compare a number with a matrix\n");
          return NULL;
        }     
      case 22:
        {
          if(m1->row_cnt!=m2->row_cnt || m1->col_cnt != m2->col_cnt)
            return_value=1;
          else
            return_value=!equality_matrices(*m1,*m2);
          break;
        }  
    }
   number n3; // dummy n3 to only write return_value to word
   n3.value=return_value;
   write_num(&word,n3);
   return word; 
 } 

int value_operand(char *operand) // this is used to give the value of the operand for use in conditional statements
 {
   number *n;
   n=malloc(sizeof(number));
  
   matrix *m;
     
   int load;
   
   load=get_operands1(operand, &n, &m);
   
   switch(load)
    {
      case 0:
          return 0; // since the operand doen't doesn't exist as a variable
      case 1:
         {
           return !(n->value==0);
           break;
         }
      case 2:    
         {
           if(m->row_cnt==m->m && m->col_cnt==m->n)
             return 1;  // this is for the case when the user only wants to know if the matrix exists, he types only the matrix name for such a check
           // else
           number n2;
           n2.value=0;
           return !(equality_mat_num(*m, n2)); // the idea is that the value of a matrix operand is 1 if any one of the values it contains is non-zero
           break;
         }  
    }
 }

char *not_task(char *operand)
 {
   int val;
   number n3; // dummy n3 to only write return_value to word
   char *word;
   
   val=value_operand(operand);
   
   n3.value=(!val);
   write_num(&word,n3);
   return word; 
 }

char *and_task(char *operand1, char *operand2)
 {
   int val1, val2;
   number n3; // dummy n3 to only write return_value to word
   char *word;
   
   val1=value_operand(operand1);
   val2=value_operand(operand2);

   n3.value=(val1&&val2);
   write_num(&word,n3);
   return word; 
 } 
 
char *or_task(char *operand1, char *operand2)
  {
   int val1, val2;
   number n3; // dummy n3 to only write return_value to word
   char *word;
   
   val1=value_operand(operand1);
   val2=value_operand(operand2);

   n3.value=(val1||val2);
   write_num(&word,n3);
   return word; 
 } 
 
int if_task(char *operand)
 {
   return value_operand(operand);
 }
 
char *del_task(char *operand)
 {
   number *n;
   n=malloc(sizeof(number));
  
   matrix *m;
     
   int load;
   
   load=get_operands1(operand, &n, &m);
   
   switch(load)
    {
      case 0:
          printf("\nInvalid operand to del!!!");
          return NULL;
          break;
      case 1:
         {
           n->name=NULL;
           break;
         }
      case 2:    
         {
           delete_matrix(m);
           break;
         }  
    }
    
   char *word=malloc((strlen(operand)+1)*sizeof(char));
   sprintf(word,"%s",operand);
   return word;
 }

void delete_matrix(matrix *m) // to delete the rows and columns from the matrix m based on the indices specified by the user
 {
   if((m->row_cnt)==(m->m) && (m->col_cnt)==(m->n))
    {
      (m->name)=NULL;
      free(m->row_indices);
      free(m->col_indices);
      free(m->a);
      return;
    } 
   // this means the entire matrix need not be deleted 
   int i,j,k,row,col;
   if((m->row_cnt)!=(m->m)) // ortherwise no specific rows to be deleted have been specified, while some columns have been specified
    {
      for(i=0;i<(m->row_cnt);i++)
       {
         row=(m->row_indices)[i];
         free((m->a)[row]);
         (m->a)[row]=NULL;
       }
      for(i=(m->m)-1;i>=0;i--)
       {
         if((m->a)[i]==NULL)
          {
            j=i;
            while(j<((m->m)-1))
             {
               (m->a)[j]=(m->a)[j+1];
               j++;
             }
            (m->a)[j]=NULL;
            (m->m)--;
          }
       } // now all the rows to be deleted have been deleted
    }
  // we now delete the columns, if any of them have been specified
  
   if((m->col_cnt)!=(m->n)) // ortherwise no specific columns to be deleted have been specified, while some rows have been specified
    {
      for(j=(m->col_cnt)-1;j>=0;j--)
       {
         col=(m->col_indices)[j];
         for(i=0;i<(m->m);i++)
          {
            k=col;
            while(k<(m->n))
             {
               (m->a)[i][k]=(m->a)[i][k+1];
               k++;
             }
          }
         (m->n)--; // since a column has been removed 
       }
    }   
  // all the columns to be deleted have been removed
 }
 
char *det_task(char *operand)
 {
   int op=isused_identifier(operand);
   matrix m;
   char *word;
       
   if(op==0)
    {
      printf("'%s' is not a valid identifier",operand);
      return NULL;
    }
   else if(op<0)
    {
      printf("\ndet requires a matrix operand!!!");
      return NULL;
    }
   else
    {
      if(op==1)
       {
         int pos;
         sscanf(operand,"%d",&pos);
         m=mat_temp_list[pos];
       }
      else
        m=mat_var_list[op-2];
    }
   if(m.row_cnt!=m.col_cnt)
    {
      printf("\nerror! square matrix required for det\n"); 
      return NULL;
    }
   double det=determinant_matrix(m);
   number n;  // n required to write det into word
   n.value=det; 
   write_num(&word,n);
   return word;  
 }
 
double determinant_matrix(matrix m) // to return the determinant of matrix m 
 {
   matrix *mtemp;
   init_mat_temp(&mtemp,m.row_cnt,m.col_cnt); 
   
   int i,j,row,col; // to copy the matrix m to mtemp
   for(i=0;i<mtemp->m;i++)
    {
      row=m.row_indices[i];
      for(j=0;j< mtemp->n;j++)
       {
         col=m.col_indices[j];
         (mtemp->a)[i][j]=m.a[row][col];
       }
    }
   // now the temporary matrix to be used for finding the determinant is ready 
   // we now find the determinant of this matrix
    
  /* double det=1.00,ratio=1.00,temp;
   for(j=0;i<(mtemp->n-1);j++)
    {
      for(i=j;i<mtemp->m;i++)
        if(a[i][j]!=0) break;
      if(i==(mtemp->n-1))
        return 0; // since all the elements in this column of the lower triangle are zero, and therefore the determinant is zero
      else
       {
         cur_row=i;
         // swap the rows j and cur_row
         for(k=j;k<mtemp->n;k++)
          ;
          
       }  
      for(i=j;i<mtemp->m;i++)
       {
         if(i!=cur_row)
          {
            ratio=a[i][j]/a[cur_row][j];
            for(k=j;k<mtemp->n;k++)
             {
               a[k][j]
             }
          }
       }
    }*/
    
   int n=mtemp->m; 
   int k, swap_row;
   int negative=1;
   double det=1.0, ratio, temp;
   double **a=mtemp->a;
   for(j=0;j<n;j++) // to transform the matrix into upper triangular
    {
     
      // to ensure that a[j][j] is not zero
      for(i=j;i<n;i++) 
        if(a[i][j]!=0) 
          break;
      if(i==n)
        return 0; // since all the elements in this column of the lower triangle are zero, and therefore the determinant is zero
      else
       {
         if(i!=j) // perform swapping of the rows
          {
            swap_row=i;
            // swap the rows j and cur_row
            for(k=j;k<n;k++)
             {
               temp=a[j][k];
               a[j][k]=a[swap_row][k];
               a[swap_row][k]=temp;
             }
            negative*=-1; 
         }
       }  
      // now a[j][j] is non-zero 
      
      for(i=j+1;i<n;i++)
       {
         ratio=(a[i][j])/(a[j][j]);
         a[i][j]=0;
         for(k=j+1;k<n;k++)
           a[i][k]=a[i][k]-ratio*a[j][k];
       }
      det = det*a[j][j];
    }
   det *= negative; //since row interchange had been performed //det * ((a[n-2][n-2] * a[n-1][n-1]) - (a[n-2][n-1] * a[n-1][n-2])); 
   return det; 
 }


int cnt_nondecimal(double d) // to return the total number of digits in d
 {
   int cnt=0;
   while(d>=1.00 || d<=-1.00)
    {
      cnt++;
      d/=10;
    }
   return cnt;
 } 

int cnt_decimal(double d)// to return the number of digits after the decimal point in d
 {
   char *word=malloc(30*sizeof(char));
   sprintf(word,"%lf ",d);
   int i,cnt=0,z=0;
   for(;*word!='.';word++) ;
   word++;
   for(i=0;word[i]!=' ';i++)
    {
      if(word[i]=='0')
        z++;
      else
        z=0;
    }
   return i-z;  
 }
 
