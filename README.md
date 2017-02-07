# MAD
MAD (Matrix Algorithm Design) tool is a scripting language for matrix operations.
Developed during Oct 2012 - Mar 2013.

## THE WHAT
MAD is a programming language that has been developed specifically for working with matrices. It is an easy-to-use language, for use by those students and teachers who don't have the necessary programming skills required for handling matrices in other more commonly used programming languages like C, C++, Java, etc. In addition, a shell has also been provided where all the functionality of the language can be used except the condtionals and looping.

To ensure an easier learning curve for the user, MAD has a very small keywords set. Also, the language incorporates most of the features of the C-language -- for example, the if-else conditionals, the while loop, and has a similar syntax -- therby making it even more easier to use for those with a basic knowledge of C. But, even the ones who have not had any programming experience, it will take barely a couple of days to be able to learn to use all the features (of course, it will take a lot of practice to be able to write very large programs for an amateur, while for those experienced with C even that will not be a big deal).

## THE WHY
A word about why I chose to develop this language. In one of the Discrete Mathematics classes, our professor mentioned that most of the real world problems are actually implemented through matrices. I had already done a course in C and knew the immense difficulties in programming matrix algortihms in C. And therefore when it came to chosing a project for our C-Lab course, I thought why not develop something that can make programming matrix algoritms easier. Hence, MAD was born.

## THE HOW

THE PROGRAM DESIGN:

After the initial pleasantries let us move on to the technical aspects of MAD as a C-program. The entire program has been divided into four files, which along with their specific purposes are mentioned below:

1. **MAD.c** : This is the file that contains the main() function and calls all other function necessary for the execution of a script, or for running the shell.

2. **file.c** : This is the file that contains the functions that are necessary for the execution of a shell script. 			      This file includes the functions for handling conditionals and loops.

3. **parser.c** : This is the file that contains all the functions that are related to 'understanding' what command has been 			      entered by the user, and thereafter calling the required functions for executing the commands. Thus, it includes functions for breaking a line into tokens, understanding what the tokens are, etc.  

4. **tasks.c** : This is the file that contains definitions for each and every task that has to be performed by MAD. Thus, 		      while parser.c was the one which took care of the user interface, tasks.c takes care of the actual commands to be executed.

Also associated with each of the above files, except MAD.c, is a corresponding header file that gives the prototype of the functions defined in the file. In addition, there is also a definitions.h file that gives the definition for the structures 'token', 'matrix' and 'number', and the enumeration 'task'.

The logic used for each of the funtion used in the program is given where the funtion has been defined only when it merits such a discussion. In other places, the logic used can easily be understood by reading the code. There are also in-line comments within the functions to explain the usage of the line, wherever necessary.

With this basic knowledge in mind, anyone who wants to understand the code can now read through the files, and understand how the entire program had been implemented. But before ventures out in this, I would suggest that he first use MAD for sometime to get a basic idea of what it does, whence he will be able to follow the line of execution in the program.

## Usage
Using MAD is just like using any other programming language. The user can type the program in a file and then pass the file as a command line argument to MAD. MAD then takes over producing the required output. Also, if no file has been passed to MAD then running MAD causes the MAD shell to start, which as mentioned earlier can be used for executing all the commands supported by MAD except the conditionals and the loops. 

The shell is actually of immense use, since it's so much like a DEBUGGER. Suppose one has typed a program, and is encountering some difficulties in the execution. He can now run the shell and run the very same program line-by-line in the shell, and see the effect of each line. In this way he can understand where he has actually gone wrong in the programming, and thereafter make the necessary corrections in the program. 

Let us now look at the keywords and operators available in MAD, and see how we can use them for writing programs.


| Keyword/Operator | Usage |
| :----------------: | ----- |
| Algebraic operators | 
|	`+` | to add two matrices, numbers, or a number to a matrix; adding a matrix to a number will give error |
|	`-`	|	to subtract two matrices, numbers, or a number from a matrix; subtract a matrix from a number will give error
|	`*`	|	to multiply two matrices, numbers, or a matrix with a number; multiplying a number with a number will give error
| `/`	|	to divide two numbers, or a matrix by a number; dividing by a matrix will give error, and by zero will give segmentaion fault 
| `=`	|	to assign to a matrix a matrix, or to a number a number, or to a matrix a number element-wise; assigning to a number a matrix will give error 	
| Relational operators |
|	 `<`, `>`, `<=`, `>=`, `==`, `!=` |	all the operators in this class are for comparing two matrices element-wise, two numbers or a matrix with a number element-wise; comparing a number with a matrix will give error; all of them return 0 if the comparison fails, else they return 1
| Boolean operators |
| `!`, `&`, `|` | these operators are the Boolean operators performing as `not`, `and`, and `or` respectively; can be used in combination with the relational operators; `!` is unary and converts 0 to 1 and vice versa; `&` is binary and returns 1 if and only if both its operands have the value 1; `|` is also binary and returns 0 if and only if both its operands have the value 0 
